#include "AchievementPopup.hpp"
#include "AchievementPopupDP.hpp"
#include "../MyButton.h"
#include "../MenuLayer.hpp"
#include "../UserCoinButton.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"
#include "../../GameObject/ObjectManager.h"
#include "../../DataManager/AchievementDataManager.hpp"
#include "../../SDKBOX/SDKBox.h"

using namespace cocos2d;
using namespace cocos2d::ui;

CAchievementPopup* CAchievementPopup::create()
{
    CAchievementPopup *pRet = new(std::nothrow) CAchievementPopup();
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

bool CAchievementPopup::init()
{
    if (!CPopup::init()) return false;
    
    Size layerSize  = this->getContentSize();
    
    auto bg = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080.f, 1920.f);
    if (bg != nullptr){
        bg->setIgnoreAnchorPointForPosition(false);
        bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        bg->setPosition(Vec2(layerSize.width * 0.5f, layerSize.height * 1.5f));
        this->addChild(bg);
    }
    
    /* ranking label*/
    auto rankingLabel = Label::createWithSystemFont("Achievement", FONT::MALGUNBD, 80);
    if (rankingLabel != nullptr)
    {
        rankingLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        rankingLabel->setPosition(Vec2(bg->getContentSize().width * 0.5f,
                                       bg->getContentSize().height * 0.8f));
        this->addChild(rankingLabel);
    }
    
    auto achievementList  = CAchievementDataManager::Instance()->getHiddenAchievementList();
    Size dpSize           = Size(1080, 200);
    size_t dpDistance     = 15;
    int spawnCount        = 4;
    auto userLevel        = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::LEVEL);
    
    
    typedef std::pair<int, const ACHIEVEMENT*> PAIR;
    auto sortedList       = std::vector<PAIR>(achievementList.begin(), achievementList.end());
    // sort
    {
        // sort by clear (clear achievements locate to the top.)
        std::stable_sort(sortedList.begin(), sortedList.end(), [=](PAIR dataA, PAIR dataB){
            auto stateA = CAchievementDataManager::getAchievementStateByIndex(dataA.second->_index, true);
            auto stateB = CAchievementDataManager::getAchievementStateByIndex(dataB.second->_index, true);
            if(stateA != ACHIEVEMENT_STATE::COMPLETED && stateB != ACHIEVEMENT_STATE::COMPLETED) return false;
            return (stateA > stateB);
        });
        // sort by hidden type (hidden achievements locate to the top.)
        std::stable_sort(sortedList.begin(), sortedList.end(), [=](PAIR dataA, PAIR dataB){
            auto storyA = (dataA.second->_type == ACHIEVEMENT_TYPE::STORY_TYPE);
            auto storyB = (dataB.second->_type == ACHIEVEMENT_TYPE::STORY_TYPE);

            return (storyA > storyB);
        });
        // sort by complete (completed achievements locate to the bottom.)
        std::stable_sort(sortedList.begin(), sortedList.end(), [=](PAIR dataA, PAIR dataB){
            auto stateA = CAchievementDataManager::Instance()->CompletedAllOfLevels(dataA.second->_index);
            auto stateB = CAchievementDataManager::Instance()->CompletedAllOfLevels(dataB.second->_index);
            
            return (stateA < stateB);
        });
    }
    
    // Create the list view
    auto listView = ListView::create();
    if (listView != nullptr){
        listView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
        listView->setBounceEnabled(true);
        listView->setBackGroundImageScale9Enabled(true);
        listView->setContentSize(Size(dpSize.width, (dpSize.height + dpDistance) * spawnCount));
        listView->setScrollBarPositionFromCorner(Vec2(7, 7));
        listView->setItemsMargin(dpDistance);
        listView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        listView->setPosition(layerSize / 2);
        listView->setMagneticType(ListView::MagneticType::BOTH_END);
        listView->setCascadeOpacityEnabled(true);
        bg->addChild(listView);
        
//        int lastCompletedIndex = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::LAST_COM_ACHIEVEMENT);
        int dpIndex = 0;
        int scrollIndex = 0;
        for(auto achievement : sortedList)
        {
            auto data  = achievement.second;

            if(!data->_visibleType) continue;
            if(userLevel < data->_openLevel) continue;
            if(CAchievementDataManager::Instance()->IsHiddenAchievement(data->_index)) continue;
//            if(lastCompletedIndex == data->_index) scrollIndex = dpIndex;
            
            auto achievementDP = CAchievementPopupDP::create(data);
            achievementDP->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            achievementDP->setCascadeOpacityEnabled(true);
            achievementDP->setRewardExit([=](){
                this->End(nullptr);
            });
            listView->pushBackCustomItem(achievementDP);
            dpIndex++;
        }
        
        if(sortedList.size() > spawnCount){
            // Scrolling to last completed achievement
            if(scrollIndex > spawnCount){
                this->scheduleOnce([=](float delta){
                    listView->scrollToItem(scrollIndex, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE, 0.f);
                }, 0.f, "ScrollToItem");
            }
        }
    }
    
    auto createButton = [=](std::function<void(Node*)> callback, std::string icon, Vec2 pos, bool use){
        return CMyButton::create()
        ->addEventListener(callback)
        ->setButtonSingleUse(use)
        ->setButtonNormalImage(icon)
        ->setButtonPosition(pos)
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->show(bg);
    };
    
    auto btnEnd  = createButton([=](Node* sender){ this->End(sender); },
                               "endIcon.png",
                               Vec2(bg->getContentSize().width * 0.92f,
                                    bg->getContentSize().height * 0.05f),
                               true);
    
//    auto btnTwit = createButton([=](Node* sender){ this->End(sender); },
//                               "twitterShareIcon_s.png",
//                               Vec2(bg->getContentSize().width * 0.08f,
//                                    bg->getContentSize().height * 0.05f),
//                               false);
//    
//    auto btnFace = createButton([=](Node* sender){ this->End(sender); },
//                               "facebookShareIcon_s.png",
//                               Vec2(bg->getContentSize().width * 0.2f,
//                                    bg->getContentSize().height * 0.05f),
//                               false);
    
    auto btnLeaderboards = createButton([=](Node* sender){ CPlayManager::Instance()->OpenLeaderboard(); },
                                        "googleLeaderboardsIcon.png",
                                        Vec2(bg->getContentSize().width * 0.08f,
                                             bg->getContentSize().height * 0.05f),
                                        false);
    
    auto btnAchievements = createButton([=](Node* sender){ CPlayManager::Instance()->OpenAchievement(); },
                                        "googleAchievementsIcon.png",
                                        Vec2(bg->getContentSize().width * 0.2f,
                                             bg->getContentSize().height * 0.05f),
                                        false);
    
    auto btnUserCoin = CUserCoinButton::create();
    if (btnUserCoin != nullptr)
    {
        btnUserCoin->setPosition(Vec2(this->getContentSize().width * 0.5f,
                                      this->getContentSize().height * 0.05f));
        btnUserCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        this->addChild(btnUserCoin);
    }
    
    this->setOpenAnimation([=](Node* sender){
        auto action = [=](Node* owner){
            auto delay = DelayTime::create(1.f);
            auto fade  = FadeIn::create(0.5f);
            auto sequence = Sequence::createWithTwoActions(delay, fade);
            owner->setOpacity(0);
            owner->runAction(sequence);
        };
        
        action(btnEnd);
//        action(btnTwit);
//        action(btnFace);
        action(btnLeaderboards);
        action(btnAchievements);

        action(rankingLabel);
        action(btnUserCoin);
        
        auto moveAction = MoveTo::create(1.2f, Vec2(layerSize.width * 0.5f, layerSize.height * 0.5f));
        auto easeAction = EaseExponentialInOut::create(moveAction);
        
        bg->runAction(easeAction);
    }, 1.2f);
    
    this->setCloseAnimation([=](Node* sender){
        bg->runAction(EaseExponentialInOut::create(MoveTo::create(1.2f, Vec2(layerSize.width * 0.5f,
                                                                             layerSize.height * 1.5f))));
        btnEnd->runAction(FadeTo::create(0.3f, 0));
//        btnTwit->runAction(FadeTo::create(0.3f, 0));
//        btnFace->runAction(FadeTo::create(0.3f, 0));
        btnLeaderboards->runAction(FadeTo::create(0.3f, 0));
        btnAchievements->runAction(FadeTo::create(0.3f, 0));

        rankingLabel->runAction(FadeTo::create(0.3f, 0));
        btnUserCoin->runAction(FadeTo::create(0.3f, 0));
    });
    
    this->setDefaultCallback([=](Node* sender){
        this->End(sender);
    });
    
    return true;
}

void CAchievementPopup::End(Node* sender){
    CObjectManager::Instance()->ZoomMoveMiddle();
    CGameScene::getGameScene()->MenuFadeIn();
    
    this->popupClose(1.3f);
}
