#pragma once
#include "../Popup.h"

class CDownloadPopup : public CPopup
{
public:
    static CDownloadPopup* create();
    
protected:
    virtual bool init() override;
    
private:
    void end();
    
    CDownloadPopup(){};
    virtual ~CDownloadPopup(){};
};
