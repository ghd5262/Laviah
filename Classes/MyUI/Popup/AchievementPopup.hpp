#pragma once
#include "../Popup.h"
#include "ui/UIListView.h"

class CAchievementPopup : public CPopup
{
public:
    static CAchievementPopup* create();
    
protected:
    virtual bool init() override;
    
private:
    void End(cocos2d::Node* sender);
    
    CAchievementPopup(){};
    virtual ~CAchievementPopup(){};
};