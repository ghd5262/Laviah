#pragma once
#include "cocos2d.h"

USING_NS_CC;

class CCharacterSelectPopupDP : public cocos2d::Layer
{
public:
	static CCharacterSelectPopupDP* create(int CharacterIdx, const std::function<void(cocos2d::Ref*)> &func);
	void DeSelect();

protected:
    virtual bool init() override;
    bool initVariable();
    
private:
    void Select();
    
	CCharacterSelectPopupDP(int CharacterIdx, const std::function<void(cocos2d::Ref*)> &func)
    : m_CharacterIdx(CharacterIdx)
	, m_SelectFunc(func){};
    virtual ~CCharacterSelectPopupDP(){};
    
private:
    int m_CharacterIdx;
	std::function<void(cocos2d::Ref*)> m_SelectFunc;
};
