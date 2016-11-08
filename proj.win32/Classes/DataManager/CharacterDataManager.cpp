#include "CharacterDataManager.h"
#include "../Common/HSHUtility.h"
//#include "../../cocos2d/external/json/rapidjson.h"
//#include "../../cocos2d/external/json/document.h"
//#include "../../cocos2d/external/json/filestream.h"
#include "../json/json.h"
#include "../SDKUtil/SDKUtil.h"

CCharacterDataManager::CCharacterDataManager()
{
	Json::Value root;
	Json::Reader reader;

	// patternList.json 파일 읽음
	std::string strCharacterList = FileUtils::getInstance()->fullPathForFilename("characterList.json");
	std::string characterListClearData = FileUtils::getInstance()->getStringFromFile(strCharacterList);
	size_t pos = characterListClearData.rfind("}");
	characterListClearData = characterListClearData.substr(0, pos + 1);

	// patternList.json log출력
	bool parsingSuccessful = reader.parse(characterListClearData, root);
	if (!parsingSuccessful)
	{
		CCASSERT(false, MakeString("parser failed : \n %s", characterListClearData.c_str()).c_str());
		return;
	}
	CCLOG("strCharacterList JSON : \n %s\n", characterListClearData.c_str());


	// stage는 배열이다.
	const Json::Value itemArray = root["characters"];

	for (unsigned int itemCount = 0; itemCount < itemArray.size(); ++itemCount)
	{
		const Json::Value valueItem = itemArray[itemCount];

		sCHARACTER_PARAM param;

		param._idx = itemCount;
		param._openLevel = valueItem["openLevel"].asInt();
		param._cost = valueItem["cost"].asInt();
		param._health = valueItem["health"].asInt();
		param._starItemTime = valueItem["defaultStarItemTime"].asInt();
		param._coinItemTime = valueItem["defaultCoinItemTime"].asInt();
		param._bonusItemTime = valueItem["defaultBonusItemTime"].asInt();
		param._magnetItemTime = valueItem["defaultMagnetItemTime"].asInt();
		param._giantItemTime = valueItem["defaultGiantItemTime"].asInt();
		param._magnetItemSize = valueItem["defaultMagnetItemSize"].asInt();
		param._aliveTextureName = valueItem["alive"].asString();
		param._normalTextureName = valueItem["normal"].asString();
		param._giantTextureName = valueItem["giant"].asString();
		param._planetTextureName = valueItem["planet"].asString();
		param._normalBulletTextureName = valueItem["normalBullet"].asString();
		param._stickBulletTextureName = valueItem["stickBullet"].asString();\
		param._normalMissileTextureName = valueItem["normalMissile"].asString();
		param._aimingMissileTextureName = valueItem["aimingMissile"].asString();
		param._normalMissilePattern = valueItem["normalMissilePattern"].asString();
		param._aimingMissilePattern = valueItem["aimingMissilePattern"].asString();
		param._deadParticleTextureName = valueItem["deadParticle"].asString();
		param._missileParticleTextureName = valueItem["missileParticle"].asString();
		param._name = valueItem["name"].asString();
		param._story = valueItem["story"].asString();
		param._texturePackName = valueItem["texturePackName"].asString();


		if (param._texturePackName != ""){
			std::string texturepackPNG = MakeString("%s.png", param._texturePackName.c_str());
			std::string texturepackPLIST = MakeString("%s.plist", param._texturePackName.c_str());

			auto util = FileUtils::getInstance();
			auto spriteFrameCache = SpriteFrameCache::getInstance();
			//
			//		// if file already exist, remove it
			if (util->isFileExist(texturepackPNG) && util->isFileExist(texturepackPLIST))
			{
				if (!spriteFrameCache->isSpriteFramesWithFileLoaded(texturepackPLIST)){
					spriteFrameCache->addSpriteFramesWithFile(texturepackPLIST, texturepackPNG);
				}
			}
		}

		m_CharacterList.emplace_back(param);
	}
    
	//for (int i = 0; i < 3; i++)
	//{
	//	sCHARACTER_PARAM character;

	//	character._idx = i;
	//	character._openLevel = 10;
	//	character._cost = 100;
	//	character._health = 1;
	//	character._starItemTime = 10.f;
	//	character._coinItemTime = 10.f;
	//	character._bonusItemTime = 10.f;
	//	character._magnetItemTime = 10.f;
	//	character._magnetItemSize = 200.f;
	//	character._giantItemTime = 10.f;
 //       character._aliveTextureName = MakeString("player%d_alive.png", i % 5);
	//	character._normalTextureName = MakeString("player%d.png", i % 5);
	//	character._giantTextureName = MakeString("player%d_big.png", i % 5);
	//	character._planetTextureName = MakeString("planet_%d.png", i % 5);
	//	character._normalBulletTextureName = MakeString("bullet_%d.png", i % 5);
	//	character._stickBulletTextureName = MakeString("stickBullet_%d.png", i % 5);
	//	character._normalMissileLine = MakeString("normalMissileLine_%d", i % 5);
	//	character._aimingMissileLine = MakeString("aimingMissileLine_%d", i % 5);
	//	character._normalMissileTextureName = MakeString("normalMissile_%d.png", i % 5);
	//	character._aimingMissileTextureName = MakeString("aimingMissile_%d.png", i % 5);
	//	character._normalMissilePattern = MakeString("normalMissile%d_Pattern", i % 5);
	//	character._aimingMissilePattern = MakeString("aimingMissile%d_Pattern", i % 5);
 //       character._deadParticleTextureName = MakeString("deadParticle_%d.png", i % 5);
 //       character._missileParticleTextureName = MakeString("missileParticle_%d.png", i % 5);
	//	character._name = MakeString("character_%d", i % 5);
	//	character._story = MakeString("story_%d", i % 5);
	//	character._texturePackName = MakeString("characterTexturePack_%d", i % 5);

 //       
 //       if(character._texturePackName != ""){
 //           std::string texturepackPNG = MakeString("%s.png", character._texturePackName.c_str());
 //           std::string texturepackPLIST = MakeString("%s.plist", character._texturePackName.c_str());
 //           
 //           auto util = FileUtils::getInstance();
 //           auto spriteFrameCache = SpriteFrameCache::getInstance();
 //           //
 //           //		// if file already exist, remove it
 //           if (util->isFileExist(texturepackPNG) && util->isFileExist(texturepackPLIST))
 //           {
 //               if(!spriteFrameCache->isSpriteFramesWithFileLoaded(texturepackPLIST)){
 //                   spriteFrameCache->addSpriteFramesWithFile(texturepackPLIST, texturepackPNG);
 //               }
 //           }
 //       }
 //       
 //       m_CharacterList.emplace_back(character);
	//}
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
	CCLOG("Index                            : %d", character._idx);
	CCLOG("Open Level                       : %d", character._openLevel);
	CCLOG("Cost                             : %d", character._cost);
	CCLOG("Health                           : %d", character._health);
	CCLOG("Star Item Time                   : %f", character._starItemTime);
	CCLOG("Coin Item Time                   : %f", character._coinItemTime);
	CCLOG("Magnet Item Time                 : %f", character._magnetItemTime);
	CCLOG("Bonus Item Time                  : %f", character._bonusItemTime);
	CCLOG("Giant Item Time                  : %f", character._giantItemTime);
	CCLOG("Magnet Item Size                 : %f", character._magnetItemSize);
    CCLOG("Alive Texture Name               : %s", character._aliveTextureName.c_str());
	CCLOG("Normal Texture Name              : %s", character._normalTextureName.c_str());
	CCLOG("Giant Texture Name               : %s", character._giantTextureName.c_str());
	CCLOG("Planet Texture Name              : %s", character._planetTextureName.c_str());
	CCLOG("Normal Bullet Texture Name       : %s", character._normalBulletTextureName.c_str());
	CCLOG("Stick Bullet Texture Name        : %s", character._stickBulletTextureName.c_str());
	CCLOG("Normal Missile Line Texture Name : %s", character._normalMissileLine.c_str());
	CCLOG("Aiming Missile Line Texture Name : %s", character._aimingMissileLine.c_str());
	CCLOG("Normal Missile Texture Name      : %s", character._normalMissileTextureName.c_str());
	CCLOG("Aiming Missile Texture Name      : %s", character._aimingMissileTextureName.c_str());
	CCLOG("Normal Missile Pattern Name      : %s", character._normalMissilePattern.c_str());
	CCLOG("Aiming Missile Pattern Name      : %s", character._aimingMissilePattern.c_str());
    CCLOG("Dead Particle Texture Name       : %s", character._deadParticleTextureName.c_str());
    CCLOG("Missile Particle Texture Name    : %s", character._missileParticleTextureName.c_str());
	CCLOG("Name                             : %s", character._name.c_str());
	CCLOG("Story                            : %s", character._story.c_str());
    CCLOG("Texture Pack Name                : %s", character._texturePackName.c_str());
	CCLOG("============================================================");
}