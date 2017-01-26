#include "CharacterDataManager.h"
#include "../Common/HSHUtility.h"
#include "../SDKUtil/SDKUtil.h"
#include "../DataManager/UserDataManager.h"

using namespace cocos2d;
using namespace CHARACTER_TEXTUREPACK;

CCharacterDataManager::CCharacterDataManager()
{
	initWithJson(m_CharacterList, "characterList.json");
}

CCharacterDataManager::~CCharacterDataManager()
{
	auto cleanList = [=](CHARACTER_LIST &list){
		for (auto data : list)
		{
			delete data.second;
			data.second = nullptr;
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

	std::string strCharacterList = FileUtils::getInstance()->fullPathForFilename(fileName);
	std::string characterListClearData = FileUtils::getInstance()->getStringFromFile(strCharacterList);
	size_t pos = characterListClearData.rfind("}");
	characterListClearData = characterListClearData.substr(0, pos + 1);
	bool parsingSuccessful = reader.parse(characterListClearData, root);
	if (!parsingSuccessful)
	{
		CCASSERT(false, MakeString("parser failed : \n %s", characterListClearData.c_str()).c_str());
		return;
	}
	CCLOG("strCharacterList JSON : \n %s\n", characterListClearData.c_str());

    const Json::Value normalCharacterArray = root["normalCharacterPackList"];
    this->addNormalCharacterTexturePack(normalCharacterArray);
    
    const Json::Value defaultPackArray = root["defaultTexturePackList"];
    m_DefaultTexturePackCount = defaultPackArray.size();
    this->addNormalCharacterTexturePack(defaultPackArray);
    
    m_NormalCharacterDefaultSet = root["normalCharacterDefaultSet"];
    m_RareCharacterDefaultSet = root["rareCharacterDefaultSet"];
    
	const Json::Value itemArray = root["characters"];
	for (unsigned int itemCount = 0; itemCount < itemArray.size(); ++itemCount)
	{
		const Json::Value valueItem = itemArray[itemCount];
        this->addCharacterToList(valueItem);
	}
}

void CCharacterDataManager::addNormalCharacterTexturePack(const Json::Value& array)
{
    for (unsigned int itemCount = 0; itemCount < array.size(); ++itemCount)
    {
        auto normalTexturePackName = array[itemCount].asString();
        this->addTexturePackToCache(normalTexturePackName);
    }
}

void CCharacterDataManager::addTexturePackToCache(std::string fileName)
{
    if(fileName == "") return;
    
    std::string texturepackPNG = MakeString("%s.png", fileName.c_str());
    std::string texturepackPLIST = MakeString("%s.plist", fileName.c_str());
    
    auto util = FileUtils::getInstance();
    auto spriteFrameCache = SpriteFrameCache::getInstance();
    
    if (util->isFileExist(texturepackPNG) && util->isFileExist(texturepackPLIST))
    {
        if (!spriteFrameCache->isSpriteFramesWithFileLoaded(texturepackPLIST)){
            spriteFrameCache->addSpriteFramesWithFile(texturepackPLIST, texturepackPNG);
        }
    }
}

void CCharacterDataManager::setTextureNameByGrade(sCHARACTER_PARAM* data)
{
    auto getName = [=](std::string key, int index){
        return StringUtils::format(key.c_str(), index);
    };
    
    if(data->_grade > 1)
    {
        data->_planetTextureName        = getName(R_PLANET,           data->_idx);
        data->_normalBulletTextureName  = getName(R_BULLET_NORMAL,    data->_idx);
        data->_stickBulletTextureName   = getName(R_BULLET_STICK,     data->_idx);
        data->_normalTextureName        = getName(R_CHARACTER_NORMAL, data->_idx);
        data->_giantTextureName         = getName(R_CHARACTER_GIANT,  data->_idx);
        data->_normalMissileTextureName = getName(R_MISSILE_NORMAL,   data->_idx);
        data->_aimingMissileTextureName = getName(R_MISSILE_AIMING,   data->_idx);
        data->_texturePackName          = getName(R_TEXTUREPACK,      data->_idx);
        
        this->addTexturePackToCache(data->_texturePackName);
    }
    else{
        auto index = random<int>(0, m_DefaultTexturePackCount-1);
        data->_planetTextureName        = getName(N_PLANET,           index);
        data->_normalBulletTextureName  = getName(N_BULLET_NORMAL,    index);
        data->_stickBulletTextureName   = getName(N_BULLET_STICK,     index);
        data->_normalTextureName        = getName(N_CHARACTER_NORMAL, data->_idx);
        data->_giantTextureName         = getName(N_CHARACTER_GIANT,  data->_idx);
        data->_normalMissileTextureName = getName(N_MISSILE_NORMAL,   index);
        data->_aimingMissileTextureName = getName(N_MISSILE_AIMING,   index);
        data->_texturePackName          = getName(N_TEXTUREPACK,      index);
    }
}

void CCharacterDataManager::addCharacterToList(const Json::Value& data)
{
    sCHARACTER_PARAM* param = new sCHARACTER_PARAM();
    
    param->_idx             = data["idx"].asInt();
    param->_grade           = data["grade"].asInt();
    
    auto initData = [=](std::string key){
        return this->initCharacterWithDefaultValue(param->_grade, key, data[key.c_str()]);
    };

    param->_openLevel       = initData("openLevel").asInt();
    param->_cost            = initData("cost").asInt();
    param->_health          = initData("health").asInt();
    param->_starItemTime    = initData("defaultStarItemTime").asInt();
    param->_coinItemTime    = initData("defaultCoinItemTime").asInt();
    param->_bonusItemTime   = initData("defaultBonusItemTime").asInt();
    param->_magnetItemTime  = initData("defaultMagnetItemTime").asInt();
    param->_giantItemTime   = initData("defaultGiantItemTime").asInt();
    param->_magnetItemSize  = initData("defaultMagnetItemSize").asInt();
    
    param->_random          = initData("random").asBool();
    param->_coinType        = initData("coinType").asBool();
    
	param->_name			= StringUtils::format(CHARACTER_DEFINE::NAME.c_str(), param->_idx);
	param->_story			= StringUtils::format(CHARACTER_DEFINE::STORY.c_str(), param->_idx);
    
    this->setTextureNameByGrade(param);
    m_CharacterList.emplace(std::pair<int, const sCHARACTER_PARAM*>(param->_idx, param));
}

const Json::Value CCharacterDataManager::initCharacterWithDefaultValue(int grade,
                                                                       std::string key,
                                                                       const Json::Value data)
{
    if(data.isNull())
    {
        if(grade == 1) return m_NormalCharacterDefaultSet[key.c_str()];
        else           return m_RareCharacterDefaultSet[key.c_str()];
    }
    return data;
}

const sCHARACTER_PARAM* CCharacterDataManager::getCharacterByIndex(int index) const
{
    auto data = m_CharacterList.find(index);
    if(data == m_CharacterList.end()) {
        CCLOG("Wrong character index : %d", index);
		CCASSERT(false, "Wrong character index");
		return nullptr;
	}
	return data->second;
} 

const sCHARACTER_PARAM* CCharacterDataManager::getNewRandomCharacter(int level, bool below)
{
	auto newList = getNonCollectedCharacterList(level, below);
	return getNewRandomCharacterFromList(newList);
}

CHARACTER_LIST CCharacterDataManager::getListByFunc(const CHARACTER_PICK &func,
                                                    CHARACTER_LIST list)
{
	for (auto iter = list.begin(); iter != list.end();)
	{
		auto &item = (*iter).second;
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
		TRANSLATE(picked->_name).c_str(),
		picked->_openLevel);

	return picked;
}

CHARACTER_LIST CCharacterDataManager::getNonCollectedCharacterList(int level, bool below)
{
	auto userDataMng = CUserDataManager::Instance();

	return getListByFunc([=](const sCHARACTER_PARAM* data){

		if (userDataMng->getUserData_IsItemHave(USERDATA_KEY::CHARACTER_LIST, data->_idx)) return false;
        
        if (!data->_random) return false;
        
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
    CCLOG("Grade                            : %d", character->_grade);
    CCLOG("Cost                             : %d", character->_cost);
    CCLOG("Random                           : %s", character->_random ? "random" : "non-random");
    CCLOG("Coin type                        : %s", character->_coinType ? "coin" : "cash");
	CCLOG("Health                           : %d", character->_health);
	CCLOG("Star Item Time                   : %f", character->_starItemTime);
	CCLOG("Coin Item Time                   : %f", character->_coinItemTime);
	CCLOG("Magnet Item Time                 : %f", character->_magnetItemTime);
	CCLOG("Bonus Item Time                  : %f", character->_bonusItemTime);
	CCLOG("Giant Item Time                  : %f", character->_giantItemTime);
	CCLOG("Magnet Item Size                 : %f", character->_magnetItemSize);
	CCLOG("Normal Texture Name              : %s", character->_normalTextureName.c_str());
	CCLOG("Giant Texture Name               : %s", character->_giantTextureName.c_str());
	CCLOG("Planet Texture Name              : %s", character->_planetTextureName.c_str());
	CCLOG("Normal Bullet Texture Name       : %s", character->_normalBulletTextureName.c_str());
	CCLOG("Stick Bullet Texture Name        : %s", character->_stickBulletTextureName.c_str());
	CCLOG("Normal Missile Texture Name      : %s", character->_normalMissileTextureName.c_str());
	CCLOG("Aiming Missile Texture Name      : %s", character->_aimingMissileTextureName.c_str());
	CCLOG("Name                             : %s", TRANSLATE(character->_name).c_str());
	CCLOG("Story                            : %s", TRANSLATE(character->_story).c_str());
    CCLOG("Texture Pack Name                : %s", character->_texturePackName.c_str());
	CCLOG("============================================================");
}