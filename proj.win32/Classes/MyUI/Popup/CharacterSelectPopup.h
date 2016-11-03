#pragma once
#include "../Popup.h"
#include "ui/UIScrollView.h"

class CMyButton;
class CUserCoinButton;
class CCharacterSelectPopupDP;
class CCharacterSelectPopup : public CSpecificPopupBase
{
public:
    static CCharacterSelectPopup* create();
    
protected:
    virtual bool initVariable() override;
    
private:
	void End(Node* sender);
	void Select(Node* sender);
    void ScrollCallback(cocos2d::Ref* ref, cocos2d::ui::ScrollView::EventType type);
    
    
    CCharacterSelectPopup()
    : m_btnSelect(nullptr)
    , m_btnEnd(nullptr)
	, m_btnUserCoin(nullptr)
    , m_BG(nullptr)
    , m_ScrollBack(nullptr)
    , m_ScrollView(nullptr)
    , m_CenterDP(nullptr)
    , m_CenterCharacterNameLabel(nullptr){};
    virtual ~CCharacterSelectPopup(){};
    
private:
    CMyButton* m_btnSelect;
    CMyButton* m_btnEnd;
    LayerColor* m_BG;
    LayerColor* m_ScrollBack;
	CUserCoinButton* m_btnUserCoin;
    cocos2d::ui::ScrollView* m_ScrollView;
    CCharacterSelectPopupDP* m_CenterDP;
    Label* m_CenterCharacterNameLabel;
};