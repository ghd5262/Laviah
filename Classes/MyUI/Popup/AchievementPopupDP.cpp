#include "AchievementPopupDP.hpp"
#include "RewardPopup.h"
#include "../MyButton.h"
#include "../UrlSprite.hpp"
#include "../ScoreUI.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/AchievementDataManager.hpp"
#include "../../Common/StringUtility.h"

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
        title->setPosition(Vec2(contentBG->getContentSize().width * 0.25f,
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
                                                   Size(contentBG->getContentSize().width * 0.7f,
                                                        contentBG->getContentSize().height * 0.45f),
                                                   TextHAlignment::LEFT,
                                                   TextVAlignment::CENTER);
        content->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        content->setPosition(Vec2(contentBG->getContentSize().width * 0.25f,
                                  contentBG->getContentSize().height * 0.5f));
        contentBG->addChild(content);
        
        // update
        m_ListenerList.emplace_back([=](){
            content->setString(CAchievementDataManager::Instance()->getAchievementContentsByIndex(index, true));
        });
    }
    
    // create star
    {
        auto maxLevel = CAchievementDataManager::Instance()->getAchievementMaxLevelByIndex(index, true);
        std::vector<Sprite*> starList;
        for(int count = 0; count <= maxLevel; count++)
        {
            auto star = Sprite::create("starIcon_s.png");
            star->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            star->setPosition(Vec2((contentBG->getContentSize().width * 0.66f) + (count * 50),
                                    contentBG->getContentSize().height * 0.25f));
            contentBG->addChild(star);
            starList.push_back(star);
        }
        
        // update
        m_ListenerList.emplace_back([=](){
            auto curLevel = CAchievementDataManager::getAchievementLevelByIndex(index);
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
            
            if(CAchievementDataManager::Instance()->CompletedAllOfLevels(index))
            {
                rewardLabel->setVisible(false);
                rewardBtn->changeContents("COMPLETE");
            }
            
            rewardLabel->setColor(Color3B::WHITE);
            rewardBtn->setTouchEnable(false);
            
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
            barBG->setPosition(contentBG->getContentSize().width * 0.25f,
                               contentBG->getContentSize().height * 0.25f);
            contentBG->addChild(barBG);
        }
        
        // bar
        {
            auto bar = Sprite::create("achievementProgress_s.png");
            bar->setColor(COLOR::GOLD);
            auto progressBar = ProgressTimer::create(bar);
            progressBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            progressBar->setPosition(contentBG->getContentSize().width * 0.25f,
                                     contentBG->getContentSize().height * 0.25f);
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
                
                
//                auto state = CAchievementDataManager::getAchievementStateByIndex(index, true);
//                if(state == ACHIEVEMENT_STATE::COMPLETED)
//                    value  = max;
            
                goalLabel->setString(StringUtils::format("%d / %d", value, max));
                progressBar->runAction(ProgressTo::create(1.5f, this->getPercent(value, max)));
            });
        }
    }

    this->contentUpdate();

    return true;
}

void CAchievementPopupDP::Reward()
{
    auto state = CAchievementDataManager::getAchievementStateByIndex(m_AchievementData->_index, true);
    if(state != ACHIEVEMENT_STATE::COMPLETED) return;
    
    // create reward popup.
    auto popup = CGameScene::getGameScene()->Reward();
    auto rewardPopup = dynamic_cast<CRewardPopup*>(popup);
    auto levelData   = CAchievementDataManager::Instance()->getCurLevelDataByIndex(m_AchievementData->_index,
                                                                                   true);
    rewardPopup->AddRewardToList(levelData._rewardKey, levelData._rewardValue);
    rewardPopup->setExitCallback([=](){
        this->contentUpdate();
    });
    
    // level up.
    CAchievementDataManager::Instance()->HiddenAchievementLevelUP(m_AchievementData->_index);
}

void CAchievementPopupDP::contentUpdate()
{
    for(auto listener : m_ListenerList)
        listener();
}

float CAchievementPopupDP::getPercent(float value, float max)
{
    if(value != 0 && max != 0)
        return std::min(100.f, (value / max) * 100.f);
    return 0.f;
}