#pragma once
#include "cocos2d.h"
#include "../../DataManager/CharacterDataManager.h"

USING_NS_CC;

class CCharacterPopupDP : public cocos2d::ui::Widget
{
public:
    static CCharacterPopupDP* create(const CHARACTER* character);
    
    void Buy();
    
    //setter & getter
    const CHARACTER* getCharacterParam() const { return m_Character; }
    
protected:
    virtual bool init() override;
    
private:
    
    CCharacterPopupDP(const CHARACTER* character)
    : m_Character(character)
    , m_CharacterImg(nullptr){};
    virtual ~CCharacterPopupDP(){};
    
private:
    const CHARACTER* m_Character;
    Sprite* m_CharacterImg;
};
