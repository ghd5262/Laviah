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
    : m_BGMVolume(0.f)
    , m_EffectVolume(0.f){};
    virtual ~COptionMusicPopup(){};
    
private:
    float m_BGMVolume;
    float m_EffectVolume;
};
