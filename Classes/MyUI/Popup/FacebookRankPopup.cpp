#include "FacebookRankPopup.hpp"
#include "FacebookRankPopupDP.hpp"
#include "../MyButton.h"
#include "../MenuLayer.hpp"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"
#include "../../GameObject/ObjectManager.h"
#include "../../SDKBOX/SDKBox.h"
#include "../../Common/StringUtility.h"

using namespace cocos2d;
using namespace cocos2d::ui;

CFacebookRankPopup* CFacebookRankPopup::create()
{
    CFacebookRankPopup *pRet = new(std::nothrow) CFacebookRankPopup();
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

bool CFacebookRankPopup::init()
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
    auto rankingLabel = Label::createWithSystemFont("Record", FONT::MALGUNBD, 80);
    if (rankingLabel != nullptr)
    {
        rankingLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        rankingLabel->setPosition(Vec2(bg->getContentSize().width * 0.5f,
                                       bg->getContentSize().height * 0.8f));
        this->addChild(rankingLabel);
    }
    
    auto userList   = CFacebookManager::Instance()->getFBUserList();
    auto sequence   = CFacebookManager::Instance()->getMyRank();
    Size dpSize     = Size(1080, 200);
    size_t dpDistance = 15;
    int spawnCount  = 4;
    
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
        
        int index = 1;
        // create dp
        for (auto user : userList)
        {
            bool isPlayer = ((sequence+1) == index);
            auto userDP   = CFacebookRankPopupDP::create(user.second, index, isPlayer);
            userDP->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            userDP->setCascadeOpacityEnabled(true);
            listView->pushBackCustomItem(userDP);
            index++;
        }
        
        if(userList.size() > spawnCount){
            // Scrolling to my score
            if(sequence > spawnCount){
                this->scheduleOnce([=](float delta){
                    listView->scrollToItem(sequence, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE, 0.f);
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
    
    auto btnEnd = createButton([=](Node* sender){ this->End(sender); },
                               "endIcon.png",
                               Vec2(bg->getContentSize().width * 0.92f,
                                    bg->getContentSize().height * 0.05f), true);
    
    auto btnInvite = createButton([=](Node* sender){ CFacebookManager::Instance()->OpenInviteDialog(); },
                               "facebookInviteIcon.png",
                               Vec2(bg->getContentSize().width * 0.08f,
                                    bg->getContentSize().height * 0.05f), false);
    
    auto fbData   = CFacebookManager::Instance()->getMyFacebookData();
    auto fbScore  = fbData->_score;
    auto btnShare = createButton([=](Node* sender){
        auto contents = StringUtils::format("제 점수는요!\n%s", StringUtility::toCommaString(fbScore).c_str());
        CFacebookManager::Instance()->OpenLinkShareDialog(contents, "내가 이렇게 점수가 높다!!");
    }, "shareIcon_1.png", Vec2(bg->getContentSize().width * 0.2f,
                               bg->getContentSize().height * 0.05f), false);
    
    
    this->setOpenAnimation([=](Node* sender){
        auto action = [=](Node* owner){
            auto delay = DelayTime::create(1.f);
            auto fade  = FadeIn::create(0.5f);
            auto sequence = Sequence::createWithTwoActions(delay, fade);
            owner->setOpacity(0);
            owner->runAction(sequence);
        };
        
        action(btnEnd);
        action(btnInvite);
        action(btnShare);
        action(rankingLabel);

        auto moveAction = MoveTo::create(1.2f, Vec2(layerSize.width * 0.5f, layerSize.height * 0.5f));
        auto easeAction = EaseExponentialInOut::create(moveAction);

        bg->runAction(easeAction);
    }, 1.2f);
    
    this->setCloseAnimation([=](Node* sender){
        bg->runAction(EaseExponentialInOut::create(MoveTo::create(1.2f, Vec2(layerSize.width * 0.5f,
                                                                             layerSize.height * 1.5f))));
        btnEnd->runAction(FadeTo::create(0.3f, 0));
        btnInvite->runAction(FadeTo::create(0.3f, 0));
        btnShare->runAction(FadeTo::create(0.3f, 0));
        rankingLabel->runAction(FadeTo::create(0.3f, 0));
    });
    
    this->setDefaultCallback([=](Node* sender){
        this->End(sender);
    });
    
    return true;
}

void CFacebookRankPopup::End(Node* sender){
    CObjectManager::Instance()->ZoomOutRank();
    CGameScene::getGameScene()->MenuFadeIn();
    
    this->popupClose(1.3f);
}
