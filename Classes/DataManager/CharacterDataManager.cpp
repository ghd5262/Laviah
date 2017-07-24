#include "CharacterDataManager.h"
#include "UserDataManager.h"
#include "DataManagerUtils.h"

using namespace cocos2d;

CCharacterDataManager::CCharacterDataManager()
{
	initWithJson(m_CharacterList, "characterList.json");
}

CCharacterDataManager::~CCharacterDataManager()
{
    DATA_MANAGER_UTILS::mapDeleteAndClean(m_CharacterList);
}

CCharacterDataManager* CCharacterDataManager::Instance()
{
    static CCharacterDataManager instance;
    return &instance;
}

void CCharacterDataManager::initWithJson(CHARACTER_LIST &list, std::string fileName)
{
	Json::Value        root;
	Json::Reader       reader;
	auto jsonString  = FileUtils::getInstance()->fullPathForFilename(fileName);
	auto clearString = FileUtils::getInstance()->getStringFromFile(jsonString);
	auto pos         = clearString.rfind("}");
	clearString      = clearString.substr(0, pos + 1);
	bool succeed     = reader.parse(clearString, root);
	if (!succeed)
	{
		CCASSERT(false, StringUtils::format("parser failed : \n %s", clearString.c_str()).c_str());
		return;
	}
	CCLOG("Character JSON : \n %s\n", clearString.c_str());
    
	const Json::Value array = root["characters"];
    for(auto character : array)
        this->addCharacterToList(character);
}

void CCharacterDataManager::addCharacterToList(const Json::Value& json)
{
    auto data      = new CHARACTER();
    
    data->_index       = json["index"].asInt();
    data->_level       = json["openLevel"].asInt();
    data->_name        = StringUtils::format(CHARACTER_DEFINE::NAME.c_str(),    data->_index);
    data->_texture     = StringUtils::format(CHARACTER_DEFINE::TEXTURE.c_str(), data->_index);
    data->_texturePack = StringUtils::format(CHARACTER_DEFINE::TEXTURE_PACK.c_str(), data->_index);
    
    this->addSkillToCharacter(data->_skillList, json["defaultSkill"]);
    this->addTexturePackToCache(data->_texturePack);
    
    m_CharacterList.emplace(std::pair<int, const CHARACTER*>(data->_index, data));
}

void CCharacterDataManager::addSkillToCharacter(SKILL_LIST& list, const Json::Value& json)
{
    for(auto skillData : json)
    {
        CHARACTER_SKILL skill;
        
        skill._skillIndex   = skillData["skillindex"].asInt();
        skill._value        = skillData["value"].asDouble();
        
        list.emplace(std::pair<int, CHARACTER_SKILL>(skill._skillIndex, skill));
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

const CHARACTER* CCharacterDataManager::getCharacterByIndex(int index) const
{
    auto data = m_CharacterList.find(index);
    if(data == m_CharacterList.end()) {
        CCLOG("Wrong character index : %d", index);
		CCASSERT(false, "Wrong character index");
		return nullptr;
	}
	return data->second;
} 

const CHARACTER* CCharacterDataManager::getNewRandomCharacter()
{
	auto newList = getNonCollectedCharacterList();
	return getNewRandomCharacterFromList(newList);
}

float CCharacterDataManager::getDefaultValueBySkillIndex(int index, int skillIdx)
{
    auto characterData = this->getCharacterByIndex(index);
    auto skillList     = characterData->_skillList;
    
    auto data = skillList.find(skillIdx);
    if(data == skillList.end()) {
        CCLOG("Wrong skill index : %d, character index : %d", skillIdx, index);
        CCASSERT(false, "Wrong skill index");
        return 0.f;
    }
    return (data->second)._value;
}

const CHARACTER* CCharacterDataManager::getNewRandomCharacterFromList(CHARACTER_LIST &list)
{
	auto size = list.size();
	if (size <= 0) {
		CCLOG("There is no character anymore.");
		return nullptr;
	}
    auto picked = list.begin();
    std::advance(picked, random<int>(0, int(list.size()-1)));
    
    CCLOG("Pick a character :: idx %d name %s",
          (picked->second)->_index,
          (picked->second)->_name.c_str());
    
    return (picked->second);
}

CHARACTER_LIST CCharacterDataManager::getNonCollectedCharacterList()
{
	auto userDataMng = CUserDataManager::Instance();

	return DATA_MANAGER_UTILS::getMapByFunc([=](const CHARACTER* data){

		if (userDataMng->getUserData_IsItemHave(USERDATA_KEY::CHARACTER_LIST, data->_index)) return false;
        
		return true;
	}, m_CharacterList);
}
