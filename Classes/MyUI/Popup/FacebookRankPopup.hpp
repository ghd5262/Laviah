#pragma once
#include "../Popup.h"
#include "ui/UIListView.h"

class CFacebookRankPopup : public CPopup
{
public:
    static CFacebookRankPopup* create();
    
protected:
    virtual bool init() override;
    
private:
    void End(cocos2d::Node* sender);
    
    CFacebookRankPopup(){};
    virtual ~CFacebookRankPopup(){};
};