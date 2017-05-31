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
    
    auto icon = Sprite::create("achievementIcon_2.png");
    icon->setColor(COLOR::GOLD);
    icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    icon->setPosition(Vec2(popupSize.width * 0.08f, popupSize.height * 0.5f));
    m_LayerBG->addChild(icon);
    
//    CAchievementDataManager::Instance()->UpdateCurHiddenAchievementList();
    
    return true;
}

void CTitleCompleteNoticePopup::checkAchievementCompleteOnRealTime()
{
    if (!CObjectManager::Instance()->getIsGamePause()) return;
    
    auto data = CAchievementDataManager::Instance()->CompleteCheckRealTime(true);
    if(data != nullptr){
        auto level  = CAchievementDataManager::getAchievementLevelByIndex(data->_index, true);
        auto hidden = (data->_hiddenType && level <= 0);
        m_ShowList.push(COMPLETED_ACHIEVEMENT(data->_index, level, hidden));
    }
    
//    if (!CGameScene::getGameScene()->getIsMenuLayerFront()) return;
//    if (!m_Checkable) return;
    
//    auto data = CAchievementDataManager::Instance()->CompleteCheckRealTime(true);
//    m_ShowList.push(data);
//    if (data != nullptr) {
//        bool enable = CAchievementDataManager::Instance()->ExistCompletedHiddenAchievement();
//        CMenuLayer::Instance()->AchievementButtonState(enable);
    
//        m_Checkable   = false;
//        auto level = CAchievementDataManager::Instance()->getAchievementLevelByIndex(data->_index);
//        level      = (level > 0) ? level-1 : level; // 이미 클리어한 업적을 보여 줘야 하기때문에 level-1
//        auto title = CAchievementDataManager::Instance()->getAchievementTitle(data->_index, level);
//        m_AchievementLabel->setString(title + std::string("\n업적을 획득 하셨습니다!"));
//        this->show();
//    }
}

void CTitleCompleteNoticePopup::show()
{
    if (!CObjectManager::Instance()->getIsGamePause()) return;
    if (!CGameScene::getGameScene()->getIsMenuLayerFront()) return;
    if (!m_Checkable) return;
    if (m_ShowList.empty()) return;
    
    m_Checkable = false;
    
    // Update achievement button
    bool enable = CAchievementDataManager::Instance()->ExistCompletedHiddenAchievement();
    CMenuLayer::Instance()->AchievementButtonState(enable);
    
    auto data   = m_ShowList.front();
    auto title  = CAchievementDataManager::Instance()->getAchievementTitle(data._index, data._level);
    std::string ment = "\n업적을 획득하셨습니다!";
    if(data._isHidden) ment = "\n숨겨진 업적을 발견했습니다!";
    
    m_AchievementLabelBack->setString(title + ment);
    m_AchievementLabelFront->setString(title + std::string("\n"));
    m_ShowList.pop();
    
    // Save the index of the last completed achievement
    CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::LAST_COM_ACHIEVEMENT, data._index);
    
    this->scheduleOnce([=](float delta){
        
        Vec2 startPos    = Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 2.f);
        Vec2 targetPos   = this->getContentSize() / 2;
        auto easeAction  = EaseExponentialOut::create(MoveTo::create(0.5f, targetPos));
        auto downAction  = Spawn::createWithTwoActions(easeAction, FadeTo::create(0.3f, 255 * 0.8f));
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

