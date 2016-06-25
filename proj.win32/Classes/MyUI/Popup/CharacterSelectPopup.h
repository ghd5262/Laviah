#pragma once
#include "../Popup.h"
#include "ui/UIScrollView.h"
#include "ui/UIImageView.h"
#include "ui/UIPageView.h"

class CMyButton;
class CUserCoinButton;
class CCharacterSelectPopup : public CSpecificPopupBase
{
public:
    static CCharacterSelectPopup* create();
    
protected:
    virtual bool initVariable() override;
    
private:
    void End();
	void Select(cocos2d::Ref* dp);
    
    CCharacterSelectPopup()
    : m_btnEnd(nullptr)
	, m_btnUserCoin(nullptr)
    , m_BG(nullptr)
    , m_ScrollBack(nullptr)
    , m_ScrollView(nullptr){};
    virtual ~CCharacterSelectPopup(){};
    
private:
    CMyButton* m_btnEnd;
    LayerColor* m_BG;
    LayerColor* m_ScrollBack;
	CUserCoinButton* m_btnUserCoin;
    cocos2d::ui::ScrollView* m_ScrollView;
};