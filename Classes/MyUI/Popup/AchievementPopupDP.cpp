#include "AchievementPopupDP.hpp"
#include "RewardPopup.h"
#include "../MyButton.h"
#include "../UrlSprite.hpp"
#include "../ScoreUI.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/AchievementDataManager.hpp"
#include "../../DataManager/AchievementRewarder/AchievementRewarder.hpp"
#include "../../Common/StringUtility.h"
#include "../../SDKBOX/SDKBoxHeaders.h"

CAchievementPopupDP* CAchievementPopupDP::create(const ACHIEVEMENT* data)
{
    CAchievementPopupDP *pRet = new(std::nothrow) CAchievementPopupDP(data);
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

bool CAchievementPopupDP::init()
{
    if (!Widget::init()) return false;
    
    auto index     = m_AchievementData->_index;
    auto levelData = CAchievementDataManager::Instance()->getCurLevelDataByIndex(index, true);
    
    auto bg = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080.f, 200.f);
    if (bg != nullptr){
        this->setContentSize(bg->getContentSize());
        bg->setIgnoreAnchorPointForPosition(false);
        bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        bg->setPosition(this->getContentSize() / 2);
        this->addChild(bg);
    }
    
    auto contentBG = LayerColor::create(COLOR::BRIGHTGRAY_ALPHA, 805.f, 200.f);
    if (contentBG != nullptr){
        contentBG->setIgnoreAnchorPointForPosition(false);
        contentBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        contentBG->setPosition(Vec2(contentBG->getContentSize().width * 0.5f,
                                    contentBG->getContentSize().height * 0.5f));
        bg->addChild(contentBG);
    }

    // create picture
    {
//        auto pic = CUrlSprite::create()
//        ->setUrl(m_User->_url)
//        ->setSize(Size(200.f, 200.f))
//        ->build(this);
//        pic->setPosition(Vec2(this->getContentSize().width * 0.25f,
//                              this->getContentSize().height * 0.5f));
    }
    
    // create title
    {
        auto title = Label::createWithSystemFont("", FONT::MALGUNBD, 40);
        title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        title->setPosition(Vec2(contentBG->getContentSize().width * 0.05f,
                                contentBG->getContentSize().height * 0.8f));
        title->setColor(COLOR::GOLD);
        contentBG->addChild(title);
        
        // update
        m_ListenerList.emplace_back([=](){
            title->setString(CAchievementDataManager::Instance()->getAchievementTitleByIndex(index));
        });
    }
    
    // create content
    {
        auto content = Label::createWithSystemFont("", FONT::MALGUN, 30,
                                                   Size(contentBG->getContentSize().width * 0.9f,
                                                        contentBG->getContentSize().height * 0.45f),
                                                   TextHAlignment::LEFT,
                                                   TextVAlignment::CENTER);
        content->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        content->setPosition(Vec2(contentBG->getContentSize().width * 0.05f,
                                  contentBG->getContentSize().height * 0.5f));
        contentBG->addChild(content);
        
        // update
        m_ListenerList.emplace_back([=](){
            content->setString(CAchievementDataManager::Instance()->getAchievementContentsByIndex(index, true));
        });
    }
    
    // create type icon
    {
        auto iconName = StringUtils::format("achievementTypeIcon_%d.png", m_AchievementData->_type);
        auto typeIcon = Sprite::create(iconName);
        typeIcon->setPosition(Vec2(contentBG->getContentSize()));
        typeIcon->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        contentBG->addChild(typeIcon);
    }
    
    // create star
    {
        auto maxLevel = CAchievementDataManager::Instance()->getAchievementMaxLevelByIndex(index, true);
        std::vector<Sprite*> starList;
        for(int count = 0; count <= maxLevel && 0 < maxLevel; count++)
        {
            auto star = Sprite::create("starIcon_s.png");
            star->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            star->setPosition(Vec2((contentBG->getContentSize().width * 0.45f) + (count * 50),
                                    contentBG->getContentSize().height * 0.2f));
            contentBG->addChild(star);
            starList.push_back(star);
        }
        
        // update
        m_ListenerList.emplace_back([=](){
            auto curLevel = CAchievementDataManager::getAchievementLevelByIndex(index, true);
            auto count    = 0;
            for(auto star : starList){
                star->setOpacity(255);
                if(count++ < curLevel) star->setColor(COLOR::GOLD);
                else {
                    star->setColor(Color3B::BLACK);
                    star->setOpacity(255 * 0.8f);
                }
            }
        });
    }
    
    // create reward button
    {
        auto rewardBtn = CMyButton::create()
        ->addEventListener([=](Node* sender){
            this->Reward();
        })
        ->setLayer(LayerColor::create(COLOR::DARKGRAY_ALPHA, 260, 200))
        ->setContents(" ")
        ->setFont(Color3B::WHITE, 35)
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->show(bg);
        
        rewardBtn->setPosition(Vec2(bg->getContentSize().width
                                    - (rewardBtn->getContentSize().width * 0.5f),
                                    bg->getContentSize().height * 0.5f));
        rewardBtn->setSwallowTouches(false);
        
        
        auto rewardLabel = CScoreUI::create(m_RewardValue)
        ->setFont(FONT::MALGUNBD, 40, Color3B::WHITE, 0)
        ->setIcon("coinIcon_s.png")
        ->setScoreAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->show(rewardBtn);
        rewardLabel->setPosition(rewardBtn->getContentSize() / 2);
        
        // update
        m_ListenerList.emplace_back([=](){
            auto data  = CAchievementDataManager::Instance()->getCurLevelDataByIndex(index, true);
            m_RewardValue = data._rewardValue;
            
            if(data._rewardKey != ACHIEVEMENT_REWARD_KEY::REWARD_COIN &&
               data._rewardKey != ACHIEVEMENT_REWARD_KEY::REWARD_COIN_RANDOM)
            {
                rewardLabel->setVisible(false);
                rewardBtn->changeContents(TRANSLATE("ACHIEVEMENT_GET_BUTTON"));
                rewardBtn->setFont(COLOR::GOLD, 35);
            }
            
            if(CAchievementDataManager::Instance()->CompletedAllOfLevels(index))
            {
                rewardLabel->setVisible(false);
                rewardBtn->changeContents(TRANSLATE("ACHIEVEMENT_COMPLETE_BUTTON"));
                rewardBtn->setFont(Color3B::WHITE, 35);
            }
            
            rewardLabel->setColor(Color3B::WHITE);
            rewardBtn->setTouchEnable(false, Color3B::GRAY);
            
            auto state = CAchievementDataManager::getAchievementStateByIndex(index, true);
            if(state == ACHIEVEMENT_STATE::COMPLETED) {
                rewardBtn->setTouchEnable(true);
                rewardLabel->setColor(COLOR::GOLD);
            }
        });
    }
    
    // create progress bar
    {
        // bar bg
        {
            auto barBG = Sprite::create("achievementProgress_s.png");
            barBG->setColor(Color3B::BLACK);
            barBG->setOpacity(255 * 0.8f);
            barBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            barBG->setPosition(contentBG->getContentSize().width * 0.05f,
                               contentBG->getContentSize().height * 0.2f);
            contentBG->addChild(barBG);
        }
        
        // bar
        {
            auto bar = Sprite::create("achievementProgress_s.png");
            bar->setColor(COLOR::GOLD);
            auto progressBar = ProgressTimer::create(bar);
            progressBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            progressBar->setPosition(contentBG->getContentSize().width * 0.05f,
                                     contentBG->getContentSize().height * 0.2f);
            progressBar->setMidpoint(Vec2(0, 0));
            progressBar->setType(ProgressTimer::Type::BAR);
            progressBar->setBarChangeRate(Vec2(1, 0));
            contentBG->addChild(progressBar);
            
            auto goalLabel  = Label::createWithTTF("", FONT::MALGUNBD, 28);
            goalLabel->enableOutline(COLOR::DARKGRAY_ALPHA, 1);
            goalLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            goalLabel->setPosition(progressBar->getContentSize() / 2);
            progressBar->addChild(goalLabel);
            
            // update
            m_ListenerList.emplace_back([=](){
                auto data  = CAchievementDataManager::Instance()->getCurLevelDataByIndex(index, true);
                auto value = CAchievementDataManager::Instance()->getHiddenAchievementCurrentValue(index);
                auto max   = data._contentsValue;
                
                auto state = CAchievementDataManager::getAchievementStateByIndex(index, true);
                if(state == ACHIEVEMENT_STATE::COMPLETED && value < max)
                    value  = max;
                
                if(CAchievementDataManager::Instance()->CompletedAllOfLevels(index))
                    value  = max;
            
                goalLabel->setString(StringUtils::format("%d / %d", value, max));
                progressBar->runAction(ProgressTo::create(1.5f, GVALUE->getPercent(value, max)));
            });
        }
    }

    this->contentUpdate();

    return true;
}

void CAchievementPopupDP::Reward()
{
    
    auto achievementMNG = CAchievementDataManager::Instance();
    auto index = m_AchievementData->_index;
    auto state = achievementMNG->getAchievementStateByIndex(index, true);
    if(state != ACHIEVEMENT_STATE::COMPLETED) return;
    
    // create reward popup.
    auto levelData = achievementMNG->getCurLevelDataByIndex(index, true);
    CGameScene::getGameScene()->Reward([=](bool isPlay){
        this->contentUpdate();
        if(isPlay) {
            this->retain();
            if(m_RewardExit)
                m_RewardExit();
            this->release();
        }
    }, {
        sREWARD_DATA(levelData._rewardKey, levelData._rewardValue)
    }, TRANSLATE("REWARD_TITLE_ACHIEVEMENT"));
    
    // sdkbox play achievement clear
    auto curLevel = CAchievementDataManager::getAchievementLevelByIndex(index, true);
    auto key      = StringUtils::format(ACHIEVEMENT_DEFINE::ACHIEVEMENT_ID.c_str(), index, curLevel);
    CPlayManager::Instance()->AchievementComplete(key);
    
    // level up.
    achievementMNG->HiddenAchievementLevelUP(index);
    
    CGoogleAnalyticsManager::LogScreen(GA_SCREEN::REWARD_ACHIEVEMENT);
}

void CAchievementPopupDP::contentUpdate()
{
    for(auto listener : m_ListenerList)
        listener();
}
