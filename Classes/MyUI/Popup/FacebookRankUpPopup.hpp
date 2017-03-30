#pragma once
#include "../Popup.h"

class CFacebookRankUpPopup : public CPopup
{
public:
    static CFacebookRankUpPopup* create();
    
protected:
    virtual bool init() override;
    
private:
    void End(cocos2d::Node* sender);

    CFacebookRankUpPopup(){};
    virtual ~CFacebookRankUpPopup(){};
};