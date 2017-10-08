#pragma once
#include "../Popup.h"
#include "ui/UIPageView.h"

struct PLANET;
class CMyButton;
class CScoreUI;
class CPlanetSelectPopup : public CPopup
{
public:
    static CPlanetSelectPopup* create();
    CPlanetSelectPopup* setPlanet(int index);
    CPopup* show(Node* parent = nullptr, int zOrder = 0);
    
    CC_SYNTHESIZE(int, m_CurrentPlanetRank, CurrentPlanetRank);
    CC_SYNTHESIZE(int, m_CurrentPlanetScore, CurrentPlanetScore);
    CC_SYNTHESIZE(int, m_CurrentPlanetLevel, CurrentPlanetLevel);

private:
    void scrollCallback(cocos2d::Ref* ref, cocos2d::ui::PageView::EventType type);
    cocos2d::ui::PageView* createPageView(Size size, Vec2 pos);
    void select();
    
    CPlanetSelectPopup()
    : m_SelectButton(nullptr)
    , m_ScrollView(nullptr)
    , m_CurrentData(nullptr)
    , m_PlanetName(nullptr)
    , m_PlanetRank(nullptr)
    , m_PlanetScore(nullptr)
    , m_PlanetCurrentLevel(nullptr)
    , m_PlanetMaxLevel(nullptr)
    , m_PlanetIndex(0)
    , m_CurrentPlanetRank(0)
    , m_CurrentPlanetScore(0)
    , m_CurrentPlanetLevel(0){};
    virtual ~CPlanetSelectPopup(){};
    
private:
    CMyButton* m_SelectButton;
    cocos2d::ui::PageView* m_ScrollView;
    const PLANET* m_CurrentData;
    cocos2d::Label* m_PlanetName;
    cocos2d::Label* m_PlanetMaxLevel;
    CScoreUI* m_PlanetCurrentLevel;
    CScoreUI* m_PlanetRank;
    CScoreUI* m_PlanetScore;
    int m_PlanetIndex;
};
