#pragma once
#include "../Popup.h"

class CMyButton;
class CUserCoinButton;
class CWorkshopPopup : public CSpecificPopupBase
{
public:
    static CWorkshopPopup* create();
    
protected:
    virtual bool initVariable() override;
    
private:
    void End();
    void Select(cocos2d::Ref* dp);
    
    CWorkshopPopup()
    : m_btnEnd(nullptr)
	, m_btnUserCoin(nullptr)
    , m_BG(nullptr)
    , m_ScrollBack(nullptr){};
    virtual ~CWorkshopPopup(){};
    
private:
    CMyButton* m_btnEnd;
    LayerColor* m_BG;
    LayerColor* m_ScrollBack;
	CUserCoinButton* m_btnUserCoin;
};