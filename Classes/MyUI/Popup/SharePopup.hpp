#pragma once
#include "../Popup.h"

class CSharePopup : public CPopup
{
public:
    static CSharePopup* create();
    
protected:
    virtual bool init() override;
    
private:
    void End();
    
    CSharePopup(){};
    virtual ~CSharePopup(){};
};