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
    std::string _normalTextureName;
	std::string _giantTextureName;
	std::string _planetTextureName;
	std::string _normalBulletTextureName;
	std::string _stickBulletTextureName;
	std::string _normalMissileLine;
	std::string _aimingMissileLine;
	std::string _normalMissileTextureName;
	std::string _aimingMissileTextureName;
	std::string _normalMissilePattern;
	std::string _aimingMissilePattern;
    std::string _name;
    std::string _story;
	std::string _texturePackName;
};


class CCharacterDataManager
{
public:
    static CCharacterDataManager* Instance();
    
    void InitWithCharacter();
    
    //getter & setter
    sCHARACTER_PARAM getCharacterInfoByIndex(int index) const;
    std::vector<sCHARACTER_PARAM> getCharacterList(){ return m_CharacterList; };
	void PrintCharacterInfo(int index);
    
private:
    CCharacterDataManager();
    virtual ~CCharacterDataManager();
    
private:
    std::vector<sCHARACTER_PARAM> m_CharacterList;
	std::unique_ptr<cocos2d::network::Downloader> m_Downloader;
};