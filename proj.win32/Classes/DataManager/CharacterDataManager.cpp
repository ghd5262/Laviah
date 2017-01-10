#include "CharacterDataManager.h"
#include "../Common/HSHUtility.h"
#include "../json/json.h"
#include "../SDKUtil/SDKUtil.h"
#include "../DataManager/UserDataManager.h"

using namespace cocos2d;

CCharacterDataManager::CCharacterDataManager()
{
	initWithJson(m_CharacterList, "characterList.json");
}

CCharacterDataManager::~CCharacterDataManager()
{
	auto cleanList = [=](CHARACTER_LIST &list){
		for (auto data : list)
		{
			delete data;
			data = nullptr;
		}
		list.clear();
	};

	cleanList(m_CharacterList);
}

CCharacterDataManager* CCharacterDataManager::Instance()
{
    static CCharacterDataManager instance;
    return &instance;
}

void CCharacterDataManager::initWithJson(CHARACTER_LIST &list, std::string fileName)
{
	Json::Value root;
	Json::Reader reader;

	// patternList.json 파일 읽음
	std::string strCharacterList = FileUtils::getInstance()->fullPathForFilename(fileName);
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

		sCHARACTER_PARAM* param = new sCHARACTER_PARAM();

		param->_idx = itemCount;
		param->_openLevel = valueItem["openLevel"].asInt();
		param->_cost = valueItem["cost"].asInt();
		param->_health = valueItem["health"].asInt();
		param->_starItemTime = valueItem["defaultStarItemTime"].asInt();
		param->_coinItemTime = valueItem["defaultCoinItemTime"].asInt();
		param->_bonusItemTime = valueItem["defaultBonusItemTime"].asInt();
		param->_magnetItemTime = valueItem["defaultMagnetItemTime"].asInt();
		param->_giantItemTime = valueItem["defaultGiantItemTime"].asInt();
		param->_magnetItemSize = valueItem["defaultMagnetItemSize"].asInt();
		param->_aliveTextureName = valueItem["alive"].asString();
		param->_normalTextureName = valueItem["normal"].asString();
		param->_giantTextureName = valueItem["giant"].asString();
		param->_planetTextureName = valueItem["planet"].asString();
		param->_normalBulletTextureName = valueItem["normalBullet"].asString();
		param->_stickBulletTextureName = valueItem["stickBullet"].asString();
		param->_normalMissileTextureName = valueItem["normalMissile"].asString();
		param->_aimingMissileTextureName = valueItem["aimingMissile"].asString();
		param->_deadParticleTextureName = valueItem["deadParticle"].asString();
		param->_missileParticleTextureName = valueItem["missileParticle"].asString();
		param->_name = valueItem["name"].asString();
		param->_story = valueItem["story"].asString();
		param->_texturePackName = valueItem["texturePackName"].asString();


		if (param->_texturePackName != ""){
			std::string texturepackPNG = MakeString("%s.png", param->_texturePackName.c_str());
			std::string texturepackPLIST = MakeString("%s.plist", param->_texturePackName.c_str());

			auto util = FileUtils::getInstance();
			auto spriteFrameCache = SpriteFrameCache::getInstance();

			if (util->isFileExist(texturepackPNG) && util->isFileExist(texturepackPLIST))
			{
				if (!spriteFrameCache->isSpriteFramesWithFileLoaded(texturepackPLIST)){
					spriteFrameCache->addSpriteFramesWithFile(texturepackPLIST, texturepackPNG);
				}
			}
		}

		list.emplace_back(param);
	}
}

const sCHARACTER_PARAM* CCharacterDataManager::getCharacterByIndex(int index) const
{
	if (m_CharacterList.size() <= index) {
		CCLOG("Wrong character index : %d", index);
		CCASSERT(false, "Wrong character index");
		return nullptr;
	}
	return m_CharacterList.at(index);
} 

const sCHARACTER_PARAM* CCharacterDataManager::getNewRandomCharacter(int level, bool below)
{
	auto newList = getNonCollectedCharacterList(level, below);
	return getNewRandomCharacterFromList(newList);
}

CHARACTER_LIST CCharacterDataManager::getListByFunc(const CHARACTER_PICK &func, CHARACTER_LIST list)
{
	for (auto iter = list.begin(); iter != list.end();)
	{
		auto &item = (*iter);
		if (item != nullptr){

			if (!func(item)) iter = list.erase(iter);
			else             iter++;
		}
	}

	return list;
}

const sCHARACTER_PARAM* CCharacterDataManager::getNewRandomCharacterFromList(CHARACTER_LIST &list)
{
	auto size = list.size();
	if (size <= 0) {
		CCLOG("There is no character anymore.");
		return nullptr;
	}
	const sCHARACTER_PARAM* picked;
	auto randomIdx = random<int>(0, int(size) - 1);
	picked = list.at(randomIdx);

	CCLOG("Pick a character :: idx %d name %s level %d",
		picked->_idx,
		picked->_name.c_str(),
		picked->_openLevel);

	return picked;
}

CHARACTER_LIST CCharacterDataManager::getNonCollectedCharacterList(int level, bool below)
{
	auto userDataMng = CUserDataManager::Instance();

	return getListByFunc([=](const sCHARACTER_PARAM* data){

		if (userDataMng->getUserData_IsItemHave(USERDATA_KEY::CHARACTER_LIST, data->_idx)) return false;

		if (below)           { if (data->_openLevel > level) return false; }
		else                 { if (data->_openLevel != level) return false; }

		return true;
	}, m_CharacterList);
}


void CCharacterDataManager::PrintCharacterInfo(int index)
{
	auto character = m_CharacterList.at(index);
	CCLOG("=======================Character Info=======================");
	CCLOG("Index                            : %d", character->_idx);
	CCLOG("Open Level                       : %d", character->_openLevel);
	CCLOG("Cost                             : %d", character->_cost);
	CCLOG("Health                           : %d", character->_health);
	CCLOG("Star Item Time                   : %f", character->_starItemTime);
	CCLOG("Coin Item Time                   : %f", character->_coinItemTime);
	CCLOG("Magnet Item Time                 : %f", character->_magnetItemTime);
	CCLOG("Bonus Item Time                  : %f", character->_bonusItemTime);
	CCLOG("Giant Item Time                  : %f", character->_giantItemTime);
	CCLOG("Magnet Item Size                 : %f", character->_magnetItemSize);
    CCLOG("Alive Texture Name               : %s", character->_aliveTextureName.c_str());
	CCLOG("Normal Texture Name              : %s", character->_normalTextureName.c_str());
	CCLOG("Giant Texture Name               : %s", character->_giantTextureName.c_str());
	CCLOG("Planet Texture Name              : %s", character->_planetTextureName.c_str());
	CCLOG("Normal Bullet Texture Name       : %s", character->_normalBulletTextureName.c_str());
	CCLOG("Stick Bullet Texture Name        : %s", character->_stickBulletTextureName.c_str());
	CCLOG("Normal Missile Line Texture Name : %s", character->_normalMissileLine.c_str());
	CCLOG("Aiming Missile Line Texture Name : %s", character->_aimingMissileLine.c_str());
	CCLOG("Normal Missile Texture Name      : %s", character->_normalMissileTextureName.c_str());
	CCLOG("Aiming Missile Texture Name      : %s", character->_aimingMissileTextureName.c_str());
    CCLOG("Dead Particle Texture Name       : %s", character->_deadParticleTextureName.c_str());
    CCLOG("Missile Particle Texture Name    : %s", character->_missileParticleTextureName.c_str());
	CCLOG("Name                             : %s", character->_name.c_str());
	CCLOG("Story                            : %s", character->_story.c_str());
    CCLOG("Texture Pack Name                : %s", character->_texturePackName.c_str());
	CCLOG("============================================================");
}