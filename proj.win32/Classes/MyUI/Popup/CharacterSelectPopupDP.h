#pragma once
#include "cocos2d.h"
#include "../../DataManager/CharacterDataManager.h"

USING_NS_CC;

class CCharacterSelectPopupDP : public cocos2d::ui::Widget
{
public:
	static CCharacterSelectPopupDP* create(const sCHARACTER_PARAM character);
    
    void DeSelect();
	void Select();
    void Center();
	void Buy();

    //setter & getter
    const sCHARACTER_PARAM getCharacterParam() const { return m_Character; }
    
protected:
    virtual bool init() override;
    bool initVariable();
    
private:
    
	CCharacterSelectPopupDP(const sCHARACTER_PARAM character)
    : m_Character(character)
    , m_CharacterImg(nullptr)
    , m_DPBack(nullptr){};
    virtual ~CCharacterSelectPopupDP(){};
    
private:
    const sCHARACTER_PARAM m_Character;
    LayerColor* m_DPBack;
    Sprite* m_CharacterImg;
};
