#pragma once
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
    std::string _textureName;
    std::string _name;
    std::string _skillName;
    std::string _story;
};


class CCharacterDataManager
{
public:
    static CCharacterDataManager* Instance();
    
    //getter & setter
    sCHARACTER_PARAM getCharacterInfoByIndex(int index) const;
    std::vector<sCHARACTER_PARAM> getCharacterList(){ return m_CharacterList; };
    
private:
    CCharacterDataManager();
    virtual ~CCharacterDataManager();
    
private:
    std::vector<sCHARACTER_PARAM> m_CharacterList;
};