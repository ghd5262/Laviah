#pragma once
#include "../Popup.h"

class CItemShopPopup : public CPopup
{
public:
    static CItemShopPopup* create();
    
protected:
    virtual bool init() override;
    
private:
    void End(cocos2d::Node* sender);
    
    CItemShopPopup(){};
    virtual ~CItemShopPopup(){};
};
