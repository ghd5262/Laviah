#pragma once
#include "../Popup.h"
#include "ui/UIListView.h"

class CMyButton;
class CFacebookRankPopupDP;
class CFacebookRankPopup : public CPopup
{
public:
    static CFacebookRankPopup* create();
    
protected:
    virtual bool init() override;
    
private:
    void End(cocos2d::Node* sender);
    void ScrollCallback(cocos2d::Ref* ref, cocos2d::ui::ScrollView::EventType type);
    
    CFacebookRankPopup()
    : m_btnSelect(nullptr)
    , m_CenterDP(nullptr){};
    virtual ~CFacebookRankPopup(){};
    
private:
    CMyButton* m_btnSelect;
    CFacebookRankPopupDP* m_CenterDP;
};