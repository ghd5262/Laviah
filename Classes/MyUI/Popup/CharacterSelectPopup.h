#pragma once
#include "../Popup.h"
#include "ui/UIListView.h"

class CMyButton;
class CUserCoinButton;
class CCharacterSelectPopupDP;
class CCharacterSelectPopup : public CPopup
{
public:
    static CCharacterSelectPopup* create();
    
protected:
	virtual bool init() override;
    
private:
	void End(cocos2d::Node* sender);
	void Select(cocos2d::Node* sender);
    void ScrollCallback(cocos2d::Ref* ref, cocos2d::ui::ScrollView::EventType type);
    
    CCharacterSelectPopup()
    : m_btnSelect(nullptr)
    , m_CenterDP(nullptr)
    , m_CenterCharacterNameLabel(nullptr){};
    virtual ~CCharacterSelectPopup(){};
    
private:
	CMyButton* m_btnSelect;
    CCharacterSelectPopupDP* m_CenterDP;
    cocos2d::Label* m_CenterCharacterNameLabel;
};