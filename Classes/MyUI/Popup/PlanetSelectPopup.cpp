#include "PlanetSelectPopup.hpp"
#include "PlanetSelectPopupDP.hpp"
#include "../MyButton.h"
#include "../ScoreUI.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/PlanetDataManager.hpp"
#include "../../DataManager/UserDataManager.h"
#include "../../GameObject/ObjectManager.h"
#include "../../SDKBOX/SDKBoxHeaders.h"
#include "../../Common/StringUtility.h"
#include <array>

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
    
    m_CurrentData    = CPlanetDataManager::Instance()->getCurPlanet();
    m_PlanetIndex    = m_CurrentData->_index;
    auto planetList  = CPlanetDataManager::Instance()->getPlanetList();
    auto jumpIndex   = 0;
    auto index       = 0;
    auto planetAngle = CObjectManager::Instance()->getPlanet()->getRotation();
    for (auto iter : planetList)
    {
        auto planet = iter.second;
        if(!planet->_enable) continue;
        
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
        
        auto image    = planetDP->getPlanetImg();
        auto rotation = RotateBy::create(120.f, 360);
        auto repeat   = RepeatForever::create(rotation);
        image->setRotation(planetAngle);
        image->runAction(repeat);
        
        if(planet->_index == m_CurrentData->_index){
            jumpIndex = index;
            m_CenterPlanet = planetDP;
            m_CenterPlanet->setVisible(false);
        }
        index++;
    }
    
    m_PlanetName = Label::createWithSystemFont(m_CurrentData->_name, FONT::MALGUNBD, 80);
    if (m_PlanetName != nullptr)
    {
        m_PlanetName->setPosition(Vec2(layerSize.width * 0.5f, layerSize.height * 0.8f));
        m_PlanetName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_PlanetName->setOpacity(0);
        this->addChild(m_PlanetName);
    }
    
    
    std::array<Vec2, 4> posDown  = {
        Vec2(layerSize.width * 0.5f, layerSize.height * -.3f),
        Vec2(layerSize.width * 0.5f, layerSize.height * -.4f),
        Vec2(layerSize.width * 0.5f, layerSize.height * -.5f),
        Vec2(layerSize.width * 0.5f, layerSize.height * -.6f),
//        Vec2(layerSize.width * 0.5f, layerSize.height * -.7f),
    };
    
    std::array<Vec2, 4> posArray = {
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.35f),
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.3f),
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.25f),
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.2f),
//        Vec2(layerSize.width * 0.5f, layerSize.height * 0.15f),
    };
    
    std::array<std::string, 4> resultIcon = {
        "progressIcon.png",
        "allRankIcon.png",
        "weeklyRankIcon.png",
        "dailyRankIcon.png"
    };
    
    this->createScoreLayer(resultIcon[0], &m_PercentTitleLabel, &m_PercentLabel);
    this->createScoreLayer(resultIcon[1], &m_AllTitleLabel, &m_AllRankLabel);
    this->createScoreLayer(resultIcon[2], &m_WeekTitleLabel, &m_WeekRankLabel);
    this->createScoreLayer(resultIcon[3], &m_TodayTitleLabel, &m_TodayRankLabel);
//    this->createTotalScoreLayer("최고 점수", "", &m_BestScoreLabel);
    
    for(int index = 0; index < posArray.size() ; index++)
    {
        auto layer = m_ScoreLayerList.at(index);
        layer->setPosition(posArray[index]);
    }
    m_ScoreLayerList.emplace_back(m_PlanetName);

    this->labelUpdate();
    
    auto createButton = [=](const std::function<void(Node*)> &callback, Node* parent,
                            std::string name, Vec2 pos, bool visible, bool use = true){
        auto button = CMyButton::create()
        ->addEventListener(callback)
        ->setButtonSingleUse(use)
        ->setButtonNormalImage(name)
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->setButtonPosition(pos)
        ->show(parent);
        button->setVisible(visible);
        m_ScoreLayerList.emplace_back(button);
        return button;
    };
    
    m_SelectButton = createButton([=](Node* sender){
        this->select();
    }, this, "playIcon.png", Vec2(layerSize.width * 0.92f, layerSize.height * 0.05f), true);
    m_SelectButton->setOpacity(0);
    
    // planet button disable.
    CMyButton::create()
    ->addEventListener([=](Node* sender){
        m_SelectButton->setTouchEnable(false);
    }, eMYBUTTON_STATE::BEGIN)
    ->setEnableSound(false)
    ->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, layerSize.width, layerSize.height * 0.8f))
    ->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(Vec2(layerSize.width * 0.5f, layerSize.height * 0.5f))
    ->show(this)
    ->setSwallowTouches(false);
    
    this->setOpenAnimation([=](Node* sender){
        auto action = [=](Node* owner){
            auto delay = DelayTime::create(1.f);
            auto fade  = FadeIn::create(0.5f);
            auto sequence = Sequence::createWithTwoActions(delay, fade);
            owner->setOpacity(0);
            owner->runAction(sequence);
        };
        
        for(auto node : m_ScoreLayerList)
            action(node);
        
        auto planet = CObjectManager::Instance()->getPlanet();
        auto delay  = DelayTime::create(1.2f);
        auto func   = CallFunc::create([=](){
            planet->setVisible(false);
            if(m_CenterPlanet)
                m_CenterPlanet->setVisible(true);
        });
        planet->runAction(Sequence::createWithTwoActions(delay, func));
        
        
        m_ScrollView->jumpToItem(jumpIndex, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
    }, 1.f);
    
    this->setCloseAnimation([=](Node* sender){
        
        m_CenterPlanet->setVisible(false);
        CObjectManager::Instance()->getPlanet()->setVisible(true);

        for(auto node : m_ScoreLayerList)
            node->runAction(FadeTo::create(0.3f, 0));
    });
    
    this->setDefaultCallback([=](Node* sender){
        this->end();
    });
    
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
    auto origin  = CPlanetDataManager::Instance()->getCurPlanet()->_index;
    
    if(index != origin)
    {
        manager->setUserData_Number(USERDATA_KEY::PLANET, index);
        CObjectManager::Instance()->ChangePlanet();
    }
    this->end();
}

void CPlanetSelectPopup::end(){
//    CAudioManager::Instance()->StopBGM();
    AudioEngine::stopAll();
    CObjectManager::Instance()->ZoomMoveMiddle();
    CGameScene::getGameScene()->MenuFadeIn();
    this->popupClose(1.3f);
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
    m_CenterPlanet     = centerContent;
    this->labelUpdate();

    bool selectable    = CPlanetDataManager::Instance()->IsPlanetOpened(m_CurrentData->_index);
    m_SelectButton->setTouchEnable(selectable);
    if(selectable) m_SelectButton->changeButtonImage("playIcon.png");
    else           m_SelectButton->changeButtonImage("lockIcon.png");
    
    AudioEngine::stopAll();
    auto sound = StringUtils::format("sounds/stageBGM_%d.mp3", m_PlanetIndex);
    CAudioManager::Instance()->PlayBGM(sound, false, false);

    // update label
//    int bestScoreKey = PARAM_PLANET::STAGE_WORLD_SCORE;
//    if(!CPlayManager::Instance()->IsLoggedIn())
//        bestScoreKey = PARAM_PLANET::STAGE_BEST_SCORE;
    
//    m_CurrentPlanetLevel = userDataMng->getUserData_ParamData(USERDATA_KEY::PLANET_LIST, m_PlanetIndex,
//                                                              PARAM_PLANET::STAGE_BEST_LEVEL, 0);
//    m_CurrentPlanetScore = userDataMng->getUserData_ParamData(USERDATA_KEY::PLANET_LIST, m_PlanetIndex,
//                                                              bestScoreKey, 0);
//    m_CurrentPlanetRank  = userDataMng->getUserData_ParamData(USERDATA_KEY::PLANET_LIST, m_PlanetIndex,
//                                                              PARAM_PLANET::STAGE_BEST_RANK, 0);
//    auto level = CStageDataManager::Instance()->getStageMaxLevel(m_PlanetIndex);
//    auto maxLevel = StringUtils::format("/ %d", level);
//    m_PlanetMaxLevel->setString(maxLevel);
//    m_PlanetName->setString(m_CurrentData->_name);
}

cocos2d::Node* CPlanetSelectPopup::createIconLayer(std::string iconName,
                                                   cocos2d::Label** labelTitle)
{
    auto layer = Sprite::create("resultPopup_2.png");
    layer->setCascadeOpacityEnabled(true);
    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(layer);
    
    auto layerSize = layer->getContentSize();
    
    auto icon = Sprite::create(iconName);
    icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    icon->setPosition(Vec2(layerSize.width * 0.1f, layerSize.height * 0.5f));
    icon->setScale(100 / icon->getContentSize().height);
    layer->addChild(icon);
    
    (*labelTitle) = Label::createWithSystemFont("", FONT::MALGUNBD, 50);
    (*labelTitle)->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    (*labelTitle)->setPosition(Vec2(layerSize.width * 0.15f,  layerSize.height * 0.5f));
    layer->addChild((*labelTitle));
    
    return layer;
}

void CPlanetSelectPopup::createScoreLayer(std::string iconName,
                                          cocos2d::Label** labelTitle,
                                          cocos2d::Label** labelValue)
{
    auto layer = this->createIconLayer(iconName, labelTitle);
    (*labelValue) = Label::createWithTTF("", FONT::MALGUNBD, 50);
    (*labelValue)->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    (*labelValue)->setPosition(Vec2(layer->getContentSize().width * 0.9f,
                            layer->getContentSize().height * 0.5f));
    layer->addChild((*labelValue));
    
    m_ScoreLayerList.emplace_back(layer);
}

void CPlanetSelectPopup::createTotalScoreLayer(std::string text,
                                               std::string value,
                                               Label** label)
{
    auto layer = Sprite::create("resultPopup_1.png");
    layer->setCascadeOpacityEnabled(true);
    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(layer);
    
    auto layerSize = layer->getContentSize();
    
    auto textLabel = Label::createWithSystemFont(text, FONT::MALGUNBD, 60);
    textLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    textLabel->setPosition(Vec2(layerSize.width * 0.1f,  layerSize.height * 0.5f));
    layer->addChild(textLabel);
    
    (*label) = Label::createWithTTF(value, FONT::MALGUNBD, 60);
    (*label)->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    (*label)->setPosition(Vec2(layerSize.width * 0.9f,  layerSize.height * 0.5f));
    layer->addChild((*label));
    
    m_ScoreLayerList.emplace_back(layer);
}

void CPlanetSelectPopup::labelUpdate()
{
    auto userDataMng = CUserDataManager::Instance();
    int bestScoreKey = PARAM_PLANET::STAGE_ALL_SCORE;
    if(!CPlayManager::Instance()->IsLoggedIn())
        bestScoreKey = PARAM_PLANET::STAGE_BEST_SCORE;
    
    auto getPlanetInfo = [=](int param){
        return userDataMng->getUserData_ParamData(USERDATA_KEY::PLANET_LIST, m_PlanetIndex, param, 0);
    };

    auto allScore       = getPlanetInfo(bestScoreKey);
    auto allRank        = getPlanetInfo(PARAM_PLANET::STAGE_ALL_RANK);
    auto weeklyScore    = getPlanetInfo(PARAM_PLANET::STAGE_WEEK_SCORE);
    auto weeklyRank     = getPlanetInfo(PARAM_PLANET::STAGE_WEEK_RANK);
    auto dayScore       = getPlanetInfo(PARAM_PLANET::STAGE_DAY_SCORE);
    auto dayRank        = getPlanetInfo(PARAM_PLANET::STAGE_DAY_RANK);
    auto percent        = getPlanetInfo(PARAM_PLANET::STAGE_PERCENT);
    
    m_PlanetName->setString(TRANSLATE(m_CurrentData->_name));
    m_PercentLabel->setString(StringUtils::format("%d%%", percent));
    m_PercentTitleLabel->setString(TRANSLATE("PLANET_TEXT_PROGRESS"));
//    m_BestScoreLabel->setString(StringUtility::toCommaString(allScore));

    this->createChangeLabelAction(m_AllRankLabel, StringUtility::toCommaString(allScore),
                                  StringUtility::toCommaString(allRank));
    this->createChangeLabelAction(m_AllTitleLabel, TRANSLATE("PLANET_TEXT_ALL_SCORE"),
                                  TRANSLATE("PLANET_TEXT_ALL_RANK"));
    
    this->createChangeLabelAction(m_WeekRankLabel, StringUtility::toCommaString(weeklyScore),
                                  StringUtility::toCommaString(weeklyRank));
    this->createChangeLabelAction(m_WeekTitleLabel, TRANSLATE("PLANET_TEXT_WEEKLY_SCORE"),
                                  TRANSLATE("PLANET_TEXT_WEEKLY_RANK"));
    
    this->createChangeLabelAction(m_TodayRankLabel, StringUtility::toCommaString(dayScore),
                                  StringUtility::toCommaString(dayRank));
    this->createChangeLabelAction(m_TodayTitleLabel, TRANSLATE("PLANET_TEXT_DAILY_SCORE"),
                                  TRANSLATE("PLANET_TEXT_DAILY_RANK"));
}

void CPlanetSelectPopup::createChangeLabelAction(cocos2d::Label* label, std::string text1, std::string text2)
{
    if(label == nullptr) return;
    
    if(label->getActionByTag(100))
        label->stopActionByTag(100);
        
    auto fadeOut  = FadeTo::create(0.5f, 0);
    auto callFunc = CallFunc::create([=](){
        auto origin  = label->getString();
        auto bestStr = text1;
        if(origin != bestStr)   label->setString(bestStr);
        else                    label->setString(text2);
    });
    auto fadeIn   = FadeIn::create(0.5f);
    auto delay    = DelayTime::create(1.5f);
    auto seq      = Sequence::create(fadeOut, callFunc, fadeIn, delay, NULL);
    auto repeat   = RepeatForever::create(seq);
    repeat->setTag(100);
    label->runAction(repeat);
    label->setString(text1);
}
