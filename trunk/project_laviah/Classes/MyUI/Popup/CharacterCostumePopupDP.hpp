#pragma once
#include "cocos2d.h"
#include "ui/UIWidget.h"
USING_NS_CC;

struct COSTUME;
class CCharacterCostumePopupDP : public cocos2d::ui::Widget
{
public:
    static CCharacterCostumePopupDP* create(const COSTUME* costume);
        
    //setter & getter
    const COSTUME* getCostumeParam() const { return m_Costume; }
    
protected:
    virtual bool init() override;
    
private:
    
    CCharacterCostumePopupDP(const COSTUME* costume)
    : m_Costume(costume)
    , m_CostumeImg(nullptr){};
    virtual ~CCharacterCostumePopupDP(){};
    
private:
    const COSTUME* m_Costume;
    Sprite* m_CostumeImg;
};
