#pragma once
#include "../../Popup.h"

class COptionMusicPopup : public CPopup
{
public:
    static COptionMusicPopup* create();
    
protected:
    virtual bool init() override;
    
private:
    COptionMusicPopup()
    : m_BGMVolume(0)
    , m_EffectVolume(0){};
    virtual ~COptionMusicPopup(){};
    
private:
    int m_BGMVolume;
    int m_EffectVolume;
};