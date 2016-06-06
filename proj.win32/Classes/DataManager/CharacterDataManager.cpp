#include "CharacterDataManager.h"
#include "../Common/HSHUtility.h"
//#include "../../cocos2d/external/json/rapidjson.h"
//#include "../../cocos2d/external/json/document.h"
//#include "../../cocos2d/external/json/filestream.h"
#include "../json/json.h"

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


	for (int i = 0; i < 5; i++)
	{
		sCHARACTER_PARAM character;

		character._idx = i;
		character._openLevel = 10;
		character._cost = 100;
		character._health = 10;
		character._starItemTime = 1.f;
		character._coinItemTime = 2.f;
		character._bonusItemTime = 3.f;
		character._magnetItemTime = 4.f;
		character._giantItemTime = 5.f;
		character._textureName = "player.png";
		character._name = MakeString("character_%d", i);
		character._skillName = MakeString("skill_%d", i);;
		character._story = MakeString("story_%d", i);

		m_CharacterList.emplace_back(character);
	}
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
    return m_CharacterList.at(index);
}