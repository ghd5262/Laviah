#pragma once
#include "../../Popup.h"

class COptionDataSavePopup : public CPopup
{
public:
    static COptionDataSavePopup* create();
    
protected:
    virtual bool init() override;
    
private:
    COptionDataSavePopup(){};
    virtual ~COptionDataSavePopup(){};
};