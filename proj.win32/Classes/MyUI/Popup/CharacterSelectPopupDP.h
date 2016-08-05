#pragma once
#include "cocos2d.h"
#include "../../DataManager/CharacterDataManager.h"

USING_NS_CC;

class CCharacterSelectPopupDP : public cocos2d::Layer
{
public:
	static CCharacterSelectPopupDP* create(const sCHARACTER_PARAM character, const std::function<void(cocos2d::Ref*)> &func);
	void DeSelect();
	void Select();
	void Buy();

protected:
    virtual bool init() override;
    bool initVariable();
    
private:
    
	CCharacterSelectPopupDP(const sCHARACTER_PARAM character, const std::function<void(cocos2d::Ref*)> &func)
    : m_Character(character)
	, m_SelectFunc(func)
    , m_ItemBack(nullptr){};
    virtual ~CCharacterSelectPopupDP(){};
    
private:
    const sCHARACTER_PARAM m_Character;
	std::function<void(cocos2d::Ref*)> m_SelectFunc;
    LayerColor* m_ItemBack;
};
