#pragma once
#include "../../Popup.h"
#include "ui/UIListView.h"
#include "ui/UIPageView.h"
class COptionPopupDP;
class COptionPopup : public CPopup
{
public:
    static COptionPopup* create();
    
protected:
    virtual bool init() override;
    
private:
    void End(cocos2d::Node* sender);
    void TitleScrollCallback(cocos2d::Ref* ref, cocos2d::ui::ScrollView::EventType type);
    void ContentScrollCallback(cocos2d::Ref* ref, cocos2d::ui::PageView::EventType type);
    cocos2d::ui::ListView* createListView(Size size, size_t distance, Vec2 pos);
    cocos2d::ui::PageView* createPageView(Size size, Vec2 pos);
    
    COptionPopup()
    : m_TitleScrollView(nullptr)
    , m_ContentScrollView(nullptr){};
    virtual ~COptionPopup(){};
    
private:
    cocos2d::ui::ListView* m_TitleScrollView;
    cocos2d::ui::PageView* m_ContentScrollView;
};