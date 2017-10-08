#include "PlanetSelectPopup.hpp"
#include "PlanetSelectPopupDP.hpp"
#include "../MyButton.h"
#include "../ScoreUI.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/PlanetDataManager.hpp"
#include "../../DataManager/UserDataManager.h"
#include "../../GameObject/ObjectManager.h"
#include "../../SDKBOX/SDKBoxHeaders.h"

using namespace cocos2d;
using namespace cocos2d::ui;

CPlanetSelectPopup* CPlanetSelectPopup::create()
{
    CPlanetSelectPopup *pRet = new(std::nothrow) CPlanetSelectPopup();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

CPopup* CPlanetSelectPopup::show(Node* parent/* = nullptr*/, int zOrder/* = 0*/)
{
    Size layerSize = this->getContentSize();
    
    auto bg = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080.f, 1920.f);
    if (bg != nullptr){
        bg->setIgnoreAnchorPointForPosition(false);
        bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        bg->setPosition(layerSize / 2);
        this->addChild(bg);
    }
    
    // create the main sroll view
    m_ScrollView = createPageView(Size(layerSize.width, layerSize.height * 0.5f),
                                  Vec2(layerSize.width * 0.5f, layerSize.height * 0.5f));
    bg->addChild(m_ScrollView);
    
    m_CurrentData    = CPlanetDataManager::Instance()->getPlanetByIndex(m_PlanetIndex);
    auto planetList  = CPlanetDataManager::Instance()->getPlanetList();
    auto jumpIndex   = 0;
    auto index       = 0;
    
    for (auto iter : planetList)
    {
        auto planet = iter.second;
//        if(!planet->_enable) continue;
        
        auto contentBG = Widget::create();
        contentBG->setContentSize(Size(layerSize.width, layerSize.height * 0.5f));
        contentBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        contentBG->setCascadeOpacityEnabled(true);
        m_ScrollView->pushBackCustomItem(contentBG);
        
        auto planetDP = CPlanetSelectPopupDP::create(planet);
        planetDP->setPosition(contentBG->getContentSize() / 2);
        planetDP->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        planetDP->setCascadeOpacityEnabled(true);
        contentBG->addChild(planetDP, 0, 100);
        
        if(planet->_index == m_CurrentData->_index){
            jumpIndex = index;
        }
        index++;
    }
    
    m_PlanetName = Label::createWithSystemFont(TRANSLATE(m_CurrentData->_name), FONT::MALGUNBD, 80);
    if (m_PlanetName != nullptr)
    {
        m_PlanetName->setPosition(Vec2(layerSize.width * 0.5f, layerSize.height * 0.8f));
        m_PlanetName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_PlanetName->setOpacity(0);
        this->addChild(m_PlanetName);
    }
    
    m_SelectButton = CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->select();
    })
    ->setButtonNormalImage("planetInfoBG.png")
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(Vec2(layerSize.width * 0.5f, layerSize.height * 0.15f))
//    ->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
    ->setEnableSound(false)
    ->show(this);
    
    auto createScoreUI = [=](int& score, std::string iconImg, Vec2 pos, Vec2 anchor,
                             int fontSize, int distance, std::string font,
                             float iconPosY = 0.5f, Vec2 iconAnchor = Vec2::ANCHOR_MIDDLE){
        auto scoreUI = CScoreUI::create(score)
        ->setFont(font, fontSize, Color3B::WHITE, 0)
        ->setIcon(iconImg, iconPosY, iconAnchor)
        ->setScoreDistance(distance)
        ->setScoreAnchorPoint(anchor)
        ->show(m_SelectButton);
        scoreUI->setPosition(pos);
        scoreUI->setOpacity(0);
        return scoreUI;
    };
    
    auto scoreAction = [=](Node* node, float delayTime1, float delayTime2){
        if(!CPlayManager::Instance()->IsLoggedIn()) return;
        
        auto delay1   = DelayTime::create(delayTime1);
        auto fadeIn   = FadeIn::create(0.5f);
        auto delay2   = DelayTime::create(1.5f);
        auto fadeOut  = FadeTo::create(0.5f, 0);
        auto delay3   = DelayTime::create(delayTime2);
        auto seq      = Sequence::create(delay1, fadeIn, delay2, fadeOut, delay3, NULL);
        node->runAction(RepeatForever::create(seq));
    };
   
    m_PlanetScore = createScoreUI(m_CurrentPlanetScore, "googleLeaderboardsIcon_s.png",
                                  Vec2(m_SelectButton->getContentSize().width * 0.5f,
                                       m_SelectButton->getContentSize().height * 0.3f),
                                  Vec2::ANCHOR_MIDDLE,
                                  45, 50, FONT::MALGUN);
    scoreAction(m_PlanetScore, 0.f, 2.5f);
    
    m_PlanetRank = createScoreUI(m_CurrentPlanetRank, "googleLeaderboardsIcon_s.png",
                                 Vec2(m_SelectButton->getContentSize().width * 0.5f,
                                      m_SelectButton->getContentSize().height * 0.3f),
                                 Vec2::ANCHOR_MIDDLE,
                                 45, 50, FONT::MALGUN);
    scoreAction(m_PlanetRank, 2.5f, 0.f);
    
    m_PlanetCurrentLevel = createScoreUI(m_CurrentPlanetLevel, "stageLevelIcon_s.png",
                                         Vec2(m_SelectButton->getContentSize().width * 0.58f,
                                              m_SelectButton->getContentSize().height * 0.75f),
                                         Vec2::ANCHOR_MIDDLE_RIGHT,
                                         80, 40, FONT::MALGUNBD,
                                         0.32f, Vec2::ANCHOR_MIDDLE_BOTTOM);
    
    m_PlanetMaxLevel = Label::createWithTTF("", FONT::MALGUN, 45);
    m_PlanetMaxLevel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    m_PlanetMaxLevel->setPosition(Vec2(m_SelectButton->getContentSize().width * 0.525f,
                                       m_SelectButton->getContentSize().height * 0.7f));
    m_SelectButton->addChild(m_PlanetMaxLevel);
    
    // planet button disable.
//    CMyButton::create()
//    ->addEventListener([=](Node* sender){
//        m_SelectButton->setTouchEnable(false);
//    }, eMYBUTTON_STATE::BEGIN)
//    ->setEnableSound(false)
//    ->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, layerSize.width, layerSize.height * 0.8f))
//    ->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
//    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
//    ->setButtonPosition(Vec2(layerSize.width * 0.5f, layerSize.height * 0.5f))
//    ->show(this)
//    ->setSwallowTouches(false);
    
    this->setOpenAnimation([=](Node* sender){
        auto action = [=](Node* owner){
            auto fade  = FadeIn::create(0.5f);
            owner->setOpacity(0);
            owner->runAction(fade);
        };
        
        action(m_PlanetName);
        action(m_PlanetCurrentLevel);
        if(!CPlayManager::Instance()->IsLoggedIn())
            action(m_PlanetScore);
        m_ScrollView->jumpToItem(jumpIndex, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
    }, 1.f);
    
    this->setCloseAnimation([=](Node* sender){});
    
    return CPopup::show(parent, zOrder);
}

CPlanetSelectPopup* CPlanetSelectPopup::setPlanet(int index)
{
    m_PlanetIndex = index;
    return this;
}

// When touch the select or buy button
void CPlanetSelectPopup::select()
{
    auto manager = CUserDataManager::Instance();
    auto index   = m_CurrentData->_index;
    
//    if(manager->getUserData_IsItemExist(USERDATA_KEY::PLANET_LIST, index)) {
        CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::PLANET, index);
        CObjectManager::Instance()->ChangePlanet();
//    }
}

cocos2d::ui::PageView* CPlanetSelectPopup::createPageView(Size size, Vec2 pos)
{
    auto pageView = PageView::create();
    pageView->setDirection(cocos2d::ui::PageView::Direction::HORIZONTAL);
    pageView->setBounceEnabled(true);
    pageView->setContentSize(size);
    pageView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    pageView->setPosition(pos);
    pageView->setCascadeOpacityEnabled(true);
    pageView->addEventListener((PageView::ccPageViewCallback)CC_CALLBACK_2(CPlanetSelectPopup::scrollCallback, this));
    
    return pageView;
}

void CPlanetSelectPopup::scrollCallback(cocos2d::Ref* ref, PageView::EventType type)
{
    PageView* pageView = dynamic_cast<PageView*>(ref);
    if (pageView == nullptr || type != PageView::EventType::TURNING) return;
    
    // Get center dp
    auto center        = pageView->getCenterItemInCurrentView();
    auto centerIdx     = pageView->getIndex(center);
    auto centerChild   = pageView->getChildren().at(centerIdx);
    auto centerBG      = dynamic_cast<Widget*>(centerChild);
    auto centerContent = dynamic_cast<CPlanetSelectPopupDP*>(centerBG->getChildByTag(100));
    if (centerContent == nullptr) return;
    
    // update button
    m_CurrentData      = centerContent->getPlanetParam();
    m_PlanetIndex      = m_CurrentData->_index;
    
    auto userDataMng   = CUserDataManager::Instance();
//    bool selectable    = userDataMng->getUserData_IsItemExist(USERDATA_KEY::PLANET_LIST,
//                                                              m_CurrentData->_index);
    
    // update label
    int bestScoreKey = PARAM_PLANET::STAGE_WORLD_SCORE;
    if(!CPlayManager::Instance()->IsLoggedIn())
        bestScoreKey = PARAM_PLANET::STAGE_BEST_SCORE;
    
    m_CurrentPlanetLevel = userDataMng->getUserData_ParamData(USERDATA_KEY::PLANET_LIST, m_PlanetIndex,
                                                              PARAM_PLANET::STAGE_BEST_LEVEL, 0);
    m_CurrentPlanetScore = userDataMng->getUserData_ParamData(USERDATA_KEY::PLANET_LIST, m_PlanetIndex,
                                                              bestScoreKey, 0);
    m_CurrentPlanetRank  = userDataMng->getUserData_ParamData(USERDATA_KEY::PLANET_LIST, m_PlanetIndex,
                                                              PARAM_PLANET::STAGE_BEST_RANK, 0);
    auto level = CStageDataManager::Instance()->getStageMaxLevel(m_PlanetIndex);
    auto maxLevel = StringUtils::format("/ %d", level);
    m_PlanetMaxLevel->setString(maxLevel);
    
//    m_SelectButton->setTouchEnable(selectable);
}
