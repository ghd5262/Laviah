#include "CharacterDataManager.h"
#include "../Common/HSHUtility.h"
//#include "../../cocos2d/external/json/rapidjson.h"
//#include "../../cocos2d/external/json/document.h"
//#include "../../cocos2d/external/json/filestream.h"
#include "../json/json.h"
#include "../SDKUtil/SDKUtil.h"

CCharacterDataManager::CCharacterDataManager()
{
//    Json::Value root;
//    Json::Reader reader;
//    m_CharacterList = new std::vector<sSHOOTER_PARAM>();
//    
//    
//    // stageListIndex.json 파일 읽음
//    std::string strStageListIndex = CCFileUtils::sharedFileUtils()->fullPathForFilename("jsonRes/stageList/stageListIndex.json");
//    ssize_t bufferSize = 0;
//    unsigned char* stageListIndexJson = CCFileUtils::sharedFileUtils()->getFileData(strStageListIndex.c_str(), "rb", &bufferSize);
//    std::string stageListIdxClearData((const char*)stageListIndexJson);
//    size_t pos = stageListIdxClearData.rfind("}");
//    stageListIdxClearData = stageListIdxClearData.substr(0, pos + 1);
//    
//    
//    bool parsingSuccessful = reader.parse(stageListIdxClearData, root);
//    if (! parsingSuccessful)
//    {
//        CCASSERT(false, MakeString("parser failed : \n %s", stageListIdxClearData.c_str()).c_str());
//        return ;
//    }
//    // stageListIndex.json log출력
//    CCLOG("strStageListIndex JSON : \n %s\n", stageListIdxClearData.c_str());
//    
//    int ForLOG = 0;
//    
//    //array
//    Json::Value stageListArray = root["stageListIndex"];
//    for (unsigned int stageListCount = 0; stageListCount < stageListArray.size(); ++stageListCount)
//    {
//        
//        std::string stageListFileName = stageListArray[stageListCount].asString();
//        CCLOG("%s, ", stageListFileName.c_str());
//        
//        // patternList.json 파일 읽음
//        std::string strStageList = CCFileUtils::sharedFileUtils()->fullPathForFilename(MakeString("jsonRes/stageList/%s.json", stageListFileName.c_str()));
//        bufferSize = 0;
//        unsigned char* stageListJson = CCFileUtils::sharedFileUtils()->getFileData(strStageList.c_str(), "rb", &bufferSize);
//        std::string stageListClearData((const char*)stageListJson);
//        pos = stageListClearData.rfind("}");
//        stageListClearData = stageListClearData.substr(0, pos + 1);
//        
//        // patternList.json log출력
//        parsingSuccessful = reader.parse(stageListClearData, stageListArray[stageListCount]);
//        if (! parsingSuccessful)
//        {
//            CCASSERT(false, MakeString("parser failed : \n %s", stageListClearData.c_str()).c_str());
//            return ;
//        }
//        CCLOG("strStageList JSON : \n %s\n", stageListClearData.c_str());
//        
//        
//        // stage는 배열이다.
//        const Json::Value stageArray = stageListArray[stageListCount]["stage"];
//        
//        for (unsigned int stageCount = 0; stageCount < stageArray.size(); ++stageCount)
//        {
//            const Json::Value valueStage = stageArray[stageCount];
//            
//            sSHOOTER_PARAM param;
//            
//            param._ShooterName = valueStage["ShooterName"].asString();
//            param._PatternName = valueStage["PatternName"].asString();
//            param._randomShootSymbol = (valueStage["Symbol"].asString())[0];
//            param._fStartTime = valueStage["StartTime"].asDouble();
//            param._fEndTime = valueStage["EndTime"].asDouble();
//            param._fSpeed = valueStage["BulletSpeed"].asDouble();
//            param._isAngleRandom = valueStage["AngleRandom"].asBool();
//            param._fAngle = valueStage["BulletAngle"].asDouble();
//            param._isOneShoot = valueStage["OneShoot"].asBool();
//            param._fInterval = valueStage["Interval"].asDouble();
//            param._nBulletCountAtOneShoot = valueStage["BulletOneShoot"].asInt();
//            param._isFly = valueStage["Fly"].asBool();
//            
//            
//            m_CharacterList->emplace_back(param);
//            ForLOG++;
//        }
//    }
//    
//    CCLOG("The Loaded Stage Size is %d AND the Last Idx of Stage Is %d", static_cast<int>(m_CharacterList->size()), ForLOG);
	m_Downloader.reset(new network::Downloader());
    
	for (int i = 0; i < 3; i++)
	{
		sCHARACTER_PARAM character;

		character._idx = i;
		character._openLevel = 10;
		character._cost = 100;
		character._health = 1;
		character._starItemTime = 10.f;
		character._coinItemTime = 10.f;
		character._bonusItemTime = 10.f;
		character._magnetItemTime = 10.f;
		character._magnetItemSize = 200.f;
		character._giantItemTime = 10.f;
		character._normalTextureName = MakeString("player%d.png", i % 5);
		character._giantTextureName = MakeString("player%d_big.png", i % 5);
		character._planetTextureName = MakeString("planet_%d.png", i % 5);
		character._normalBulletTextureName = MakeString("bullet_%d.png", i % 5);
		character._stickBulletTextureName = MakeString("stickBullet_%d.png", i % 5);
		character._normalMissileLine = MakeString("normalMissileLine_%d", i % 5);
		character._aimingMissileLine = MakeString("aimingMissileLine_%d", i % 5);
		character._normalMissileTextureName = MakeString("normalMissile_%d.png", i % 5);
		character._aimingMissileTextureName = MakeString("aimingMissile_%d.png", i % 5);
		character._normalMissilePattern = MakeString("normalMissile%d_Pattern", i % 5);
		character._aimingMissilePattern = MakeString("aimingMissile%d_Pattern", i % 5);
		character._name = MakeString("character_%d", i % 5);
		character._story = MakeString("story_%d", i % 5);
		character._texturePackName = MakeString("characterTexturePack_%d", i % 5);

		std::string texturepackPNG = MakeString("%s.png", character._texturePackName.c_str());
		std::string texturepackPLIST = MakeString("%s.plist", character._texturePackName.c_str());
		std::string downloadedPath = FileUtils::getInstance()->getWritablePath() + "Download/Character/";
		std::string downloadPath = "http://www.nowtrade.co.kr/Resources/imageRes/";
		auto util = FileUtils::getInstance();
        
		// if file already exist, remove it
		if (util->isFileExist(texturepackPNG) && util->isFileExist(texturepackPLIST))
		{
			CCLOG("User local file %s", texturepackPNG.c_str());
			CCLOG("User local file %s", texturepackPLIST.c_str());
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile(texturepackPLIST, texturepackPNG);
//			m_CharacterList.emplace_back(character);

			/*if (false == util->removeFile(coTask._fileName))
			{
				coTask._errCode = DownloadTask::ERROR_FILE_OP_FAILED;
				coTask._errCodeInternal = 0;
				coTask._errDescription = "Can't remove old file: ";
				coTask._errDescription.append(coTask._fileName);
				break;
			}*/
		}
		else if (util->isFileExist(downloadedPath + texturepackPNG) && util->isFileExist(downloadedPath + texturepackPLIST))
		{
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile(downloadedPath + texturepackPLIST, downloadedPath + texturepackPNG);
//			m_CharacterList.emplace_back(character);
		}
		else
		{
			m_Downloader->createDownloadFileTask(downloadPath + texturepackPNG, downloadedPath + texturepackPNG, texturepackPNG);
			m_Downloader->createDownloadFileTask(downloadPath + texturepackPLIST, downloadedPath + texturepackPLIST, texturepackPLIST);
			m_Downloader->onFileTaskSuccess = [this, util, character, downloadedPath, texturepackPNG, texturepackPLIST](const cocos2d::network::DownloadTask& task)
			{
				// Load the non-encrypted atlas
				if (util->isFileExist(downloadedPath + texturepackPNG) && util->isFileExist(downloadedPath + texturepackPLIST))
				{
					SpriteFrameCache::getInstance()->addSpriteFramesWithFile(downloadedPath + texturepackPLIST, downloadedPath + texturepackPNG);
//					m_CharacterList.emplace_back(character);
				}
			};

			m_Downloader->onTaskError = [this](const cocos2d::network::DownloadTask& task,
				int errorCode,
				int errorCodeInternal,
				const std::string& errorStr)
			{
				std::string warnning = MakeString("Failed to download : %s, identifier(%s) error code(%d), internal error code(%d) desc(%s)"
					, task.requestURL.c_str()
					, task.identifier.c_str()
					, errorCode
					, errorCodeInternal
					, errorStr.c_str());
				CCLOG("WARNNING - %s", warnning.c_str());
				CSDKUtil::Instance()->Toast(warnning);
			};
		}
        m_CharacterList.emplace_back(character);
	}
}

void CCharacterDataManager::InitWithCharacter()
{
    
}

CCharacterDataManager::~CCharacterDataManager()
{
}

CCharacterDataManager* CCharacterDataManager::Instance()
{
    static CCharacterDataManager instance;
    return &instance;
}

sCHARACTER_PARAM CCharacterDataManager::getCharacterInfoByIndex(int index) const
{
	if (m_CharacterList.size() <= index){
		CCLOG("WARNNING - There is no item with index %d", index);
		return m_CharacterList.at(0);
	}
    return m_CharacterList.at(index);
} 

void CCharacterDataManager::PrintCharacterInfo(int index)
{
	auto character = m_CharacterList.at(index);
	CCLOG("=======================Character Info=======================");
	CCLOG("Index						: %d", character._idx);
	CCLOG("Open Level					: %d", character._openLevel);
	CCLOG("Cost							: %d", character._cost);
	CCLOG("Health						: %d", character._health);
	CCLOG("Star Item Time				: %f", character._starItemTime);
	CCLOG("Coin Item Time				: %f", character._coinItemTime);
	CCLOG("Magnet Item Time				: %f", character._magnetItemTime);
	CCLOG("Bonus Item Time				: %f", character._bonusItemTime);
	CCLOG("Giant Item Time				: %f", character._giantItemTime);
	CCLOG("Magnet Item Size				: %f", character._magnetItemSize);
	CCLOG("Normal Texture Name			: %s", character._normalTextureName.c_str());
	CCLOG("Giant Texture Name			: %s", character._giantTextureName.c_str());
	CCLOG("Planet Texture Name			: %s", character._planetTextureName.c_str());
	CCLOG("Normal Bullet Texture Name	: %s", character._normalBulletTextureName.c_str());
	CCLOG("Stick Bullet Texture Name	: %s", character._stickBulletTextureName.c_str());
	CCLOG("Normal Missile Line Texture Name	: %s", character._normalMissileLine.c_str());
	CCLOG("Aiming Missile Line Texture Name	: %s", character._aimingMissileLine.c_str());
	CCLOG("Normal Missile Texture Name	: %s", character._normalMissileTextureName.c_str());
	CCLOG("Aiming Missile Texture Name	: %s", character._aimingMissileTextureName.c_str());
	CCLOG("Normal Missile Pattern Name	: %s", character._normalMissilePattern.c_str());
	CCLOG("Aiming Missile Pattern Name	: %s", character._aimingMissilePattern.c_str());
	CCLOG("Name							: %s", character._name.c_str());
	CCLOG("Story						: %s", character._story.c_str());
	CCLOG("============================================================");
}