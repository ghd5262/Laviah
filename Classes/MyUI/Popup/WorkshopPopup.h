#pragma once
#include "../Popup.h"

class CWorkshopPopup : public CPopup
{
public:
    static CWorkshopPopup* create();
    
protected:
	virtual bool init() override;
    
private:
	void End(cocos2d::Node* sender);
    
    CWorkshopPopup(){};
    virtual ~CWorkshopPopup(){};
};