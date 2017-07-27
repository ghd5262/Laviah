#pragma once
#include "../Popup.h"
#include "ui/UIListView.h"
#include "ui/UIPageView.h"

struct CHARACTER;
class CMyButton;
class CCharacterPopup : public CPopup
{
public:
    static CCharacterPopup* create();
    
protected:
    virtual bool init() override;
    
private:
    void end();
    void select();
    void costume();
    
    void TitleScrollCallback(cocos2d::Ref* ref, cocos2d::ui::ScrollView::EventType type);
    void ContentScrollCallback(cocos2d::Ref* ref, cocos2d::ui::PageView::EventType type);
    cocos2d::ui::ListView* createListView(Size size, size_t distance, Vec2 pos);
    cocos2d::ui::PageView* createPageView(Size size, Vec2 pos);
    
    CCharacterPopup()
    : m_TitleScrollView(nullptr)
    , m_ContentScrollView(nullptr)
    , m_ExplainLabel(nullptr)
    , m_SelectButton(nullptr)
    , m_CostumeButton(nullptr)
    , m_ExitButton(nullptr)
    , m_CurrentData(nullptr){};
    virtual ~CCharacterPopup(){};
    
private:
    cocos2d::ui::ListView* m_TitleScrollView;
    cocos2d::ui::PageView* m_ContentScrollView;
    cocos2d::Label* m_ExplainLabel;
    CMyButton* m_SelectButton;
    CMyButton* m_CostumeButton;
    CMyButton* m_ExitButton;
    const CHARACTER* m_CurrentData;
};
