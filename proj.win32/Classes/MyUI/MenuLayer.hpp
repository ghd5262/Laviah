#pragma once
#include "Popup.h"

class CMyButton;
class CMenuLayer : public CPopup
{
public:
    static CMenuLayer* create();
    
protected:
    virtual bool init() override;
    
private:
    
    CMenuLayer(){};
    virtual ~CMenuLayer(){};

};