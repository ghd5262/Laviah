#pragma once
#include "network/CCDownloader.h"
#include "../Common/HSHUtility.h"
#include "../json/json.h"
#include <vector>
#include <map>

namespace CHARACTER_TEXTUREPACK {
    static const std::string R_PLANET 			= "rare_planet_%d.png";
    static const std::string R_BULLET_NORMAL 	= "rare_bullet_normal_%d.png";
    static const std::string R_BULLET_STICK 	= "rare_bullet_stick_%d.png";
    static const std::string R_CHARACTER_NORMAL = "rare_character_normal_%d.png";
    static const std::string R_CHARACTER_GIANT 	= "rare_character_giant_%d.png";
    static const std::string R_MISSILE_NORMAL 	= "rare_missile_normal_%d.png";
    static const std::string R_MISSILE_AIMING 	= "rare_missile_aiming_%d.png";
    static const std::string R_TEXTUREPACK      = "rareTexturePack_%d";
    
    static const std::string N_PLANET           = "normal_planet_%d.png";
    static const std::string N_BULLET_NORMAL    = "normal_bullet_normal_%d.png";
    static const std::string N_BULLET_STICK     = "normal_bullet_stick_%d.png";
    static const std::string N_CHARACTER_NORMAL = "normal_character_normal_%d.png";
    static const std::string N_CHARACTER_GIANT  = "normal_character_normal_%d.png";
    static const std::string N_MISSILE_NORMAL   = "normal_missile_normal_%d.png";
    static const std::string N_MISSILE_AIMING   = "normal_missile_aiming_%d.png";
    static const std::string N_TEXTUREPACK      = "defaultTexturePack_%d";
}

namespace CHARACTER_DEFINE {
    static const int GRADE_NORMAL = 1;
    static const int GRADE_RARE   = 2;
	static const std::string NAME  = "CHARACTER_NAME_%d";
}


struct sCHARACTER_PARAM{
    int _idx;
    int _grade;
    bool _random;
    bool _visible;
	bool _prepared;
    float _starItemTime;
    float _coinItemTime;
    float _bonusItemTime;
    float _magnetItemTime;
    float _giantItemTime;
    float _magnetItemSize;
    std::string _normalTextureName;
    std::string _giantTextureName;
    std::string _planetTextureName;
    std::string _normalBulletTextureName;
    std::string _stickBulletTextureName;
    std::string _normalMissileTextureName;
    std::string _aimingMissileTextureName;
    std::string _name;
    std::string _texturePackName;
    
    sCHARACTER_PARAM()
    : _idx(0)
    , _grade(0)
    , _random(false)
	, _visible(false)
	, _prepared(false)
    , _starItemTime(0)
    , _coinItemTime(0)
    , _bonusItemTime(0)
    , _magnetItemTime(0)
    , _giantItemTime(0)
    , _magnetItemSize(0)
    , _normalTextureName("")
    , _giantTextureName("")
    , _planetTextureName("")
    , _normalBulletTextureName("")
    , _stickBulletTextureName("")
    , _normalMissileTextureName("")
    , _aimingMissileTextureName("")
    , _name("")
    , _texturePackName(""){}
};

typedef std::map<int, const sCHARACTER_PARAM*> CHARACTER_LIST;
typedef std::function<bool(const sCHARACTER_PARAM*)> CHARACTER_PICK;
class CCharacterDataManager
{
public:
    static CCharacterDataManager* Instance();
        
    //getter & setter
	CHARACTER_LIST getCharacterList(){ return m_CharacterList; };
	void PrintCharacterInfo(int index);
	bool NonCollectedCharacterExist(int level, bool below);

	//getter & setter
	const sCHARACTER_PARAM* getCharacterByIndex(int index) const;
	const sCHARACTER_PARAM* getNewRandomCharacter(int level, bool below);
	static CHARACTER_LIST getListByFunc(const CHARACTER_PICK &func, CHARACTER_LIST list);

private:
    void initWithJson(CHARACTER_LIST &list, std::string fileName);
    void addNormalCharacterTexturePack(const Json::Value& array);
    void addTexturePackToCache(std::string fileName);
    void setTextureNameByGrade(sCHARACTER_PARAM* data);
    void addCharacterToList(const Json::Value& data);
    const Json::Value initCharacterWithDefaultValue(int grade, std::string key, const Json::Value data);
	const sCHARACTER_PARAM* getNewRandomCharacterFromList(CHARACTER_LIST &list);
	CHARACTER_LIST getNonCollectedCharacterList(int level, bool below);

    CCharacterDataManager();
    virtual ~CCharacterDataManager();
    
private:
	CHARACTER_LIST m_CharacterList;
    Json::Value m_NormalCharacterDefaultSet;
    Json::Value m_RareCharacterDefaultSet;
    int m_DefaultTexturePackCount;
};