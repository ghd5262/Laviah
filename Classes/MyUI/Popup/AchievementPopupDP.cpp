#include "AchievementPopupDP.hpp"
#include "RewardPopup.h"
#include "../MyButton.h"
#include "../UrlSprite.hpp"
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
        auto title = CAchievementDataManager::Instance()->getAchievementTitleByIndex(index);
        auto name  = Label::createWithSystemFont(title, FONT::MALGUNBD, 40);
        name->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        name->setPosition(Vec2(contentBG->getContentSize().width * 0.25f,
                               contentBG->getContentSize().height * 0.75f));
        contentBG->addChild(name);
    }
    
    // create content
    {
        auto contents = CAchievementDataManager::Instance()->getAchievementContentsByIndex(index, true);
        auto content = Label::createWithSystemFont(contents, FONT::MALGUN, 30,
                                                   Size(contentBG->getContentSize().width * 0.7f,
                                                        contentBG->getContentSize().height * 0.45f),
                                                   TextHAlignment::LEFT,
                                                   TextVAlignment::CENTER);
        content->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        content->setPosition(Vec2(contentBG->getContentSize().width * 0.25f,
                                  contentBG->getContentSize().height * 0.5f));
        contentBG->addChild(content);
    }
    
    // create star
    {
        auto maxLevel = CAchievementDataManager::Instance()->getAchievementMaxLevelByIndex(index, true);
        auto curLevel = CAchievementDataManager::getAchievementLevelByIndex(index);
        for(int count = 0; count <= maxLevel; count++)
        {
            auto star = Sprite::create("starIcon_s.png");
            star->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            star->setPosition(Vec2((contentBG->getContentSize().width * 0.6f) + (count * 50),
                                    contentBG->getContentSize().height * 0.25f));
            contentBG->addChild(star);
            
            if(count < curLevel)    star->setColor(COLOR::GOLD);
            else                    star->setOpacity(255 * 0.4f);
        }
    }
    
    // create reward button
    {
        auto updateRewardBtn = [=](CMyButton* btn){
            auto data  = CAchievementDataManager::Instance()->getCurLevelDataByIndex(index, true);
            btn->changeContents(StringUtils::format("\n%d", data._rewardValue));
            btn->setTouchEnable(false);

            auto state = CAchievementDataManager::getAchievementStateByIndex(index, true);
            if(state == ACHIEVEMENT_STATE::COMPLETED) {
                btn->setColor(COLOR::GOLD);
                btn->setTouchEnable(true);
                btn->setOpacity(255 * 0.8f);
            }
        };
        
        m_RewardBtn = CMyButton::create()
        ->addEventListener([=](Node* sender){
            this->Reward();
            updateRewardBtn(m_RewardBtn);
        })
        ->setLayer(LayerColor::create(COLOR::DARKGRAY_ALPHA, 260, 200))
        ->setContents("0")
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->show(bg);
        
        m_RewardBtn->setPosition(Vec2(bg->getContentSize().width
                                    - (m_RewardBtn->getContentSize().width * 0.5f),
                                    bg->getContentSize().height * 0.5f));
        m_RewardBtn->setSwallowTouches(false);
        
        updateRewardBtn(m_RewardBtn);
    }
    
    // create progress bar
    {
        // bar bg
        {
            auto barBG = LayerColor::create(COLOR::DARKGRAY_ALPHA, 250, 50);
            barBG->setIgnoreAnchorPointForPosition(false);
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
            progressBar->setOpacity(255 * 0.8f);
            contentBG->addChild(progressBar);
            
            auto golaString = StringUtils::format("%d / %d", 0, 0);
            auto goalLabel  = Label::createWithTTF(golaString, FONT::MALGUNBD, 20);
            goalLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            goalLabel->setPosition(progressBar->getContentSize() / 2);
            //            goalLabel->setColor(COLOR::DARKGRAY);
            progressBar->addChild(goalLabel);
            
            progressBar->runAction(ProgressTo::create(1.5f, 80.f));
        }
    }
    
    //    auto btn = CMyButton::create()
    //    ->addEventListener([=](Node* sender){
    //        this->retain();
    //        if(m_SelectDPListener)
    //        {
    //            m_SelectDPListener(m_DPIndex);
    //        }
    //        this->release();
    //    })
    //    ->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, 150.f, 150.f))
    //    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    //    ->show(this);
    //    btn->setPosition(this->getContentSize() / 2);
    //    btn->setSwallowTouches(false);
    
    
    return true;
}

void CAchievementPopupDP::Reward()
{
    // create reward popup.
    auto popup = CGameScene::getGameScene()->Reward();
    auto rewardPopup = dynamic_cast<CRewardPopup*>(popup);
    auto levelData   = CAchievementDataManager::Instance()->getCurLevelDataByIndex(m_AchievementData->_index,
                                                                                   true);
    rewardPopup->AddRewardToList(levelData._rewardKey, levelData._rewardValue);
    
    // level up.
    CAchievementDataManager::Instance()->HiddenAchievementLevelUP(m_AchievementData->_index);
}