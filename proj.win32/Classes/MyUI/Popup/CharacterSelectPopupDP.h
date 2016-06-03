#pragma once
#include "cocos2d.h"

USING_NS_CC;

class CCharacterSelectPopupDP : public cocos2d::Layer
{
public:
    static CCharacterSelectPopupDP* create(int CharacterIdx);
    
protected:
    virtual bool init() override;
    bool initVariable();
    
private:
    void Select();
    
    CCharacterSelectPopupDP(int CharacterIdx)
    : m_CharacterIdx(CharacterIdx){};
    virtual ~CCharacterSelectPopupDP(){};
    
private:
    int m_CharacterIdx;
};
