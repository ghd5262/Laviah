#pragma once
#include "../Popup.h"

namespace GAME_END_TITLE {
    static const std::string END_TITLE = "END_TITLE_%d";
    static const std::string END_TIP   = "END_TIP_%d";
}

class CMyButton;
class CGameEndPopup : public CPopup
{
public:
    static CGameEndPopup* create();
    
protected:
    virtual bool init() override;
    
private:
    void End(cocos2d::Node* sender);
    
    CGameEndPopup(){};
    virtual ~CGameEndPopup(){};
};
