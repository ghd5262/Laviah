#pragma once
#include "cocos2d.h"
#include "../../DataManager/CharacterDataManager.h"

USING_NS_CC;

class CCharacterSelectPopupDP : public cocos2d::ui::Widget
{
public:
	static CCharacterSelectPopupDP* create(const sCHARACTER_PARAM* character);
    
    void DeSelect();
	void Select();
    void Center();
	void Buy();

    //setter & getter
    const sCHARACTER_PARAM* getCharacterParam() const { return m_Character; }
    
    CC_SYNTHESIZE(int, m_DPIndex, DPIndex);
    CC_SYNTHESIZE(std::function<void(int)>, m_SelectDPListener, SelectDPListener)
    
protected:
    virtual bool init() override;
    
private:
    
	CCharacterSelectPopupDP(const sCHARACTER_PARAM* character)
    : m_Character(character)
    , m_CharacterImg(nullptr)
    , m_SelectDPListener(nullptr)
    , m_DPIndex(0){};
    virtual ~CCharacterSelectPopupDP(){};
    
private:
    const sCHARACTER_PARAM* m_Character;
    Sprite* m_CharacterImg;
    Label* m_CostLabel;
};
