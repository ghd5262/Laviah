#include "TitleCompleteNoticePopup.hpp"
#include "../MyButton.h"
#include "../Popup.h"
#include "../MenuLayer.hpp"
#include "../../Scene/GameScene.h"
#include "../../GameObject/ObjectManager.h"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/AchievementDataManager.hpp"

USING_NS_CC;

namespace TITLE_COMPLETE_NOTICE{
    static const std::string TAG_UPDATE = "checkTitleCompleteOnRealTime";
    static const std::string TAG_SHOW   = "achievementNoticeShow";
    static const float STAY_LIMIT_TIME  = 5.f;
};

CTitleCompleteNoticePopup* CTitleCompleteNoticePopup::create()
{
    CTitleCompleteNoticePopup *pRet = new(std::nothrow) CTitleCompleteNoticePopup();
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

bool CTitleCompleteNoticePopup::init()
{
    if (!CPopup::init()) return false;
    this->schedule([=](float delta){
        this->checkAchievementCompleteOnRealTime();
        this->show();
    }, 0.5f, TITLE_COMPLETE_NOTICE::TAG_UPDATE);
    
    m_LayerBG = Sprite::create("resultPopup_2.png");
    this->setContentSize(m_LayerBG->getContentSize());
    m_LayerBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_LayerBG->setPosition(this->getContentSize() / 2);
    m_LayerBG->setOpacity(0);
    m_LayerBG->setCascadeOpacityEnabled(true);
    this->addChild(m_LayerBG);
    
    auto popupSize = this->getContentSize();
    
    m_AchievementLabelBack = Label::createWithSystemFont("", FONT::MALGUNBD, 45,
                                                          Size(popupSize.width * 0.8f, popupSize.height * 3.f),
                                                          TextHAlignment::CENTER,
                                                          TextVAlignment::CENTER);
    m_AchievementLabelBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_AchievementLabelBack->setPosition(popupSize / 2);
    m_LayerBG->addChild(m_AchievementLabelBack);
    
    m_AchievementLabelFront = Label::createWithSystemFont("", FONT::MALGUNBD, 45,
                                                          Size(popupSize.width * 0.8f, popupSize.height * 3.f),
                                                          TextHAlignment::CENTER,
                                                          TextVAlignment::CENTER);
    m_AchievementLabelFront->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_AchievementLabelFront->setPosition(popupSize / 2);
    m_AchievementLabelFront->setColor(COLOR::GOLD);
    m_LayerBG->addChild(m_AchievementLabelFront);
    
    auto icon = Sprite::create("achievementIcon.png");
    icon->setColor(COLOR::GOLD);
    icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    icon->setPosition(Vec2(popupSize.width * 0.08f, popupSize.height * 0.5f));
    m_LayerBG->addChild(icon);
    
    m_NewIcon = Sprite::create("achievementIconNew_2.png");
    m_NewIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_NewIcon->setPosition(Vec2(popupSize.width * 0.08f, popupSize.height * 0.5f));
    m_NewIcon->setVisible(false);
    m_LayerBG->addChild(m_NewIcon);

    
//    CAchievementDataManager::Instance()->UpdateCurHiddenAchievementList();
    
    return true;
}

void CTitleCompleteNoticePopup::checkAchievementCompleteOnRealTime()
{
    if (!CObjectManager::Instance()->getIsGamePause()) return;

    CAchievementDataManager::Instance()->CompleteCheckRealTime(true);
}

void CTitleCompleteNoticePopup::show()
{
    if (!CObjectManager::Instance()->getIsGamePause()) return;
    if (!CGameScene::getGameScene()->getIsMenuLayerFront()) return;
    if (!m_Checkable) return;
    
    auto& completedList = CAchievementDataManager::Instance()->getCompletedList();
    if (completedList.empty()) return;

    m_Checkable = false;
    
    auto data   = completedList.front();
    auto title  = CAchievementDataManager::Instance()->getAchievementTitle(data._index, data._level);
    std::string ment = std::string("\n") + TRANSLATE("GET_ACHIEVEMENT");
    if(data._isHidden) ment = std::string("\n") + TRANSLATE("GET_HIDDEN_ACHIEVEMENT");
    
    m_AchievementLabelBack->setString(title + ment);
    m_AchievementLabelFront->setString(title + std::string("\n"));
    completedList.pop();
    
    // Update achievement button
    bool enable = CAchievementDataManager::Instance()->ExistCompletedHiddenAchievement();
    CMenuLayer::Instance()->AchievementButtonState(enable, data._isHidden);
    
    // new icon
    m_NewIcon->setVisible(data._isHidden);

    // Save the index of the last completed achievement
//    CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::LAST_COM_ACHIEVEMENT, data._index);
    
    this->scheduleOnce([=](float delta){
        
        Vec2 startPos    = Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 2.f);
        Vec2 targetPos   = this->getContentSize() / 2;
        auto easeAction  = EaseExponentialOut::create(MoveTo::create(0.5f, targetPos));
        auto downAction  = Spawn::createWithTwoActions(easeAction, FadeTo::create(0.3f, 255));
        auto delayAction = DelayTime::create(TITLE_COMPLETE_NOTICE::STAY_LIMIT_TIME);
        auto sineAction  = EaseSineIn::create(MoveTo::create(0.3f, startPos));
        auto upAction    = Spawn::createWithTwoActions(sineAction, FadeTo::create(0.1f, 0));
        auto callFunc    = CallFunc::create([=](){
            m_Checkable = true;
        });
        auto sequenceAction = Sequence::create(downAction, delayAction, upAction, callFunc, nullptr);
        
        m_LayerBG->setPosition(startPos);
        m_LayerBG->runAction(sequenceAction);
        
    }, 0.f, TITLE_COMPLETE_NOTICE::TAG_SHOW);
}

