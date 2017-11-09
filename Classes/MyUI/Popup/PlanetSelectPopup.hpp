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
    
    cocos2d::Node* createIconLayer(std::string iconName,
                                   cocos2d::Label** labelTitle);
    
    void createScoreLayer(std::string iconName,
                          cocos2d::Label** labelTitle,
                          cocos2d::Label** labelValue);
    
    void createTotalScoreLayer(std::string text,
                               std::string value,
                               cocos2d::Label** label);
    void labelUpdate();
    void createChangeLabelAction(cocos2d::Label* label,
                                 std::string text1,
                                 std::string text2);
    void select();
    void end();
    
    CPlanetSelectPopup()
    : m_SelectButton(nullptr)
    , m_ScrollView(nullptr)
    , m_CurrentData(nullptr)
    , m_CenterPlanet(nullptr)
    , m_PlanetName(nullptr)
    , m_PercentLabel(nullptr)
    , m_AllRankLabel(nullptr)
    , m_WeekRankLabel(nullptr)
    , m_TodayRankLabel(nullptr)
    , m_BestScoreLabel(nullptr)
    , m_PercentTitleLabel(nullptr)
    , m_AllTitleLabel(nullptr)
    , m_WeekTitleLabel(nullptr)
    , m_TodayTitleLabel(nullptr)
//    , m_PlanetRank(nullptr)
//    , m_PlanetScore(nullptr)
//    , m_PlanetCurrentLevel(nullptr)
//    , m_PlanetMaxLevel(nullptr)
    , m_PlanetIndex(0)
    , m_CurrentPlanetRank(0)
    , m_CurrentPlanetScore(0)
    , m_CurrentPlanetLevel(0){
        m_ScoreLayerList.clear();
    };
    virtual ~CPlanetSelectPopup(){};
    
private:
    CMyButton* m_SelectButton;
    cocos2d::ui::PageView* m_ScrollView;
    const PLANET* m_CurrentData;
    cocos2d::Node* m_CenterPlanet;
    cocos2d::Label* m_PlanetName;
    
    cocos2d::Label* m_PercentLabel;
    cocos2d::Label* m_AllRankLabel;
    cocos2d::Label* m_WeekRankLabel;
    cocos2d::Label* m_TodayRankLabel;
    
    cocos2d::Label* m_BestScoreLabel;
    cocos2d::Label* m_PercentTitleLabel;
    cocos2d::Label* m_AllTitleLabel;
    cocos2d::Label* m_WeekTitleLabel;
    cocos2d::Label* m_TodayTitleLabel;
    
    std::vector<Node*> m_ScoreLayerList;

//    cocos2d::Label* m_PlanetMaxLevel;
//    CScoreUI* m_PlanetCurrentLevel;
//    CScoreUI* m_PlanetRank;
//    CScoreUI* m_PlanetScore;
    int m_PlanetIndex;
};
