#pragma once
#include "../Common/HSHUtility.h"
#include "../json/json.h"
#include <vector>
#include <map>

namespace CHARACTER_DEFINE {
	static const std::string NAME         = "CHARACTER_NAME_%d";
    static const std::string TEXTURE      = "character_texture_%d.png";
    static const std::string TEXTURE_600  = "character_texture_600_%d.png";
    static const std::string TEXTURE_PACK = "character_texture_pack_%d";
}

struct CHARACTER_SKILL{
    int _skillIndex;
    float _value;
    
    CHARACTER_SKILL()
    : _skillIndex(-1)
    , _value(0.f){};
};

typedef std::map<int, CHARACTER_SKILL> SKILL_LIST;
struct CHARACTER{
    int _index;
    int _level;
    std::string _name;
    std::string _texture;
    std::string _texture_600;
    std::string _texturePack;
    SKILL_LIST _skillList;
    
    CHARACTER()
    : _index(-1)
    , _level(0)
    , _name("")
    , _texture("")
    , _texture_600("")
    , _texturePack(""){
        _skillList.clear();
    };
};

typedef std::map<int, const CHARACTER*> CHARACTER_LIST;
typedef std::function<bool(const CHARACTER*)> CHARACTER_PICK;
class CCharacterDataManager
{
public:
    static CCharacterDataManager* Instance();
        
    CHARACTER_LIST getCharacterList(){ return m_CharacterList; };
    const CHARACTER* getCharacterByIndex(int index) const;
	const CHARACTER* getNewRandomCharacter();
    const CHARACTER* getCurCharacter();

    float getDefaultValueBySkillIndex(int index, int skillIdx);
    
    static void addTexturePackToCache(std::string fileName);
    
private:
    void initWithJson(CHARACTER_LIST &list, std::string fileName);
    void addCharacterToList(const Json::Value& json);
    void addSkillToCharacter(SKILL_LIST& list, const Json::Value& json);
    
	const CHARACTER* getNewRandomCharacterFromList(CHARACTER_LIST &list);
	CHARACTER_LIST getNonCollectedCharacterList();

    CCharacterDataManager();
    virtual ~CCharacterDataManager();
    
private:
	CHARACTER_LIST m_CharacterList;
};
