#pragma once
#include "../Popup.h"
#include "ui/UIPageView.h"

typedef std::function<void()> VOID_CALLBACK;

struct COSTUME;
class CMyButton;
class CCharacterCostumePopup : public CPopup
{
public:
    static CCharacterCostumePopup* create();
    CCharacterCostumePopup* setCharacter(int index);
    CCharacterCostumePopup* setExitCallback(const VOID_CALLBACK &callback);
    CPopup* show(Node* parent = nullptr, int zOrder = 0);
    
private:
    void end();
    void select();
    void share();
    void scrollCallback(cocos2d::Ref* ref, cocos2d::ui::PageView::EventType type);
    cocos2d::ui::PageView* createPageView(Size size, Vec2 pos);

    CCharacterCostumePopup()
    : m_SelectButton(nullptr)
    , m_ShareButton(nullptr)
    , m_ScrollView(nullptr)
    , m_CurrentData(nullptr)
    , m_ExitCallback(nullptr)
    , m_FingerIcon(nullptr)
    , m_CapturedNode(nullptr)
    , m_CharacterIndex(0){};
    virtual ~CCharacterCostumePopup(){};
    
private:
    CMyButton* m_SelectButton;
    CMyButton* m_ShareButton;
    cocos2d::ui::PageView* m_ScrollView;
    const COSTUME* m_CurrentData;
    VOID_CALLBACK m_ExitCallback;
    cocos2d::Sprite* m_FingerIcon;
    cocos2d::Sprite* m_CapturedNode;
    int m_CharacterIndex;
};
