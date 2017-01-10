#pragma once
#include "../Common/HSHUtility.h"
#include "network/CCDownloader.h"
#include <vector>

struct sCHARACTER_PARAM{
    int _idx;
    int _openLevel;
    int _cost;
    int _health;
    float _starItemTime;
    float _coinItemTime;
    float _bonusItemTime;
    float _magnetItemTime;
    float _giantItemTime;
	float _magnetItemSize;
    std::string _aliveTextureName;
    std::string _normalTextureName;
	std::string _giantTextureName;
	std::string _planetTextureName;
	std::string _normalBulletTextureName;
	std::string _stickBulletTextureName;
	std::string _normalMissileLine;
	std::string _aimingMissileLine;
	std::string _normalMissileTextureName;
	std::string _aimingMissileTextureName;
    std::string _deadParticleTextureName;
    std::string _missileParticleTextureName;
    std::string _name;
    std::string _story;
	std::string _texturePackName;
};

typedef std::vector<const sCHARACTER_PARAM*> CHARACTER_LIST;
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
	const sCHARACTER_PARAM* getNewRandomCharacterFromList(CHARACTER_LIST &list);
	CHARACTER_LIST getNonCollectedCharacterList(int level, bool below);

    CCharacterDataManager();
    virtual ~CCharacterDataManager();
    
private:
	CHARACTER_LIST m_CharacterList;
};