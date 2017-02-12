#pragma once
#include "../../Popup.h"
#include "ui/UIListView.h"

class COptionDeveloperPopup : public CPopup
{
public:
    static COptionDeveloperPopup* create();
    
protected:
    virtual bool init() override;
    
private:
    void createDeveloperDP(std::string name,
                           std::string job,
                           cocos2d::Vec2 pos);
    
    COptionDeveloperPopup(){};
    virtual ~COptionDeveloperPopup(){};
};