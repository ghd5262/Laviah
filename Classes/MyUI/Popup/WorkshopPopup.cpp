#include "WorkshopPopup.h"
#include "WorkshopPopupDP.h"
#include "../MyButton.h"
#include "../UserCoinButton.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/WorkshopItemDataManager.h"
#include "../../DataManager/UserDataManager.h"
#include "../../GameObject/ObjectManager.h"
#include "ui/UIListView.h"

using namespace cocos2d;
using namespace cocos2d::ui;

CWorkshopPopup* CWorkshopPopup::create()
{
	CWorkshopPopup *pRet = new(std::nothrow) CWorkshopPopup();
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

bool CWorkshopPopup::init()
{
	if (!CPopup::init()) return false;

	Size layerSize = this->getContentSize();

	auto bg = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080.f, 1920.f);
	if (bg != nullptr){
		bg->setIgnoreAnchorPointForPosition(false);
		bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bg->setPosition(Vec2(layerSize.width * 0.5f, layerSize.height * 1.5f));
		this->addChild(bg);
	}

	/* workShop label*/
	auto workShopLabel = Label::createWithSystemFont(TRANSLATE("WORKSHOP_POPUP_TITLE"), FONT::MALGUNBD, 80);
	if (workShopLabel != nullptr)
	{
		workShopLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		workShopLabel->setPosition(Vec2(bg->getContentSize().width * 0.5f,
                                        bg->getContentSize().height * 0.8f));
		this->addChild(workShopLabel);
	}

	auto itemList = CWorkshopItemDataManager::Instance()->getSellingWorkshopItemList();
	Size dpSize = Size(1080, 200);
	size_t dpDistance = 15;
	float spawnCount = 4;

	unsigned currentItemIdx = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::SELECT_ITEM);

	// Create the list view
	auto listView = ListView::create();
	if (listView != nullptr){
		listView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
		listView->setBounceEnabled(true);
		listView->setBackGroundImageScale9Enabled(true);
		listView->setContentSize(Size(bg->getContentSize().width, (dpSize.height + dpDistance) * spawnCount));
		listView->setScrollBarPositionFromCorner(Vec2(7, 7));
		listView->setItemsMargin(dpDistance);
		listView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		listView->setPosition(layerSize / 2);
		listView->setMagneticType(ListView::MagneticType::BOTH_END);
        bg->addChild(listView);

		unsigned dpIdx = 0;
		unsigned currentItemDPIdx = 0;

		for (auto item : itemList)
		{
			auto itemDP = CWorkshopPopupDP::create(item);
			itemDP->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			listView->pushBackCustomItem(itemDP);

			if (item._idx == currentItemIdx){
				currentItemDPIdx = dpIdx;
			}
			dpIdx++;
		}

		// Scrolling to current character
		this->scheduleOnce([=](float delta){
			listView->scrollToItem(currentItemDPIdx, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE, 0.f);
		}, 0.f, "ScrollToItem");
	}

    auto btnEnd = CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->End(sender);
    })
    ->setButtonSingleUse(true)
    ->setButtonNormalImage("endIcon.png")
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(Vec2(this->getContentSize().width * 0.92f,
                             this->getContentSize().height * 0.05f))
    ->show(this);
    
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
        action(btnUserCoin);
        action(workShopLabel);
        
        auto moveAction = MoveTo::create(1.2f, Vec2(layerSize.width * 0.5f, layerSize.height * 0.5f));
        auto easeAction = EaseExponentialInOut::create(moveAction);
        bg->runAction(easeAction);
        
	}, 1.2f);

	this->setCloseAnimation([=](Node* sender){
		bg->runAction(EaseExponentialInOut::create(MoveTo::create(1.2f, Vec2(layerSize.width * 0.5f,
                                                                             layerSize.height * 1.5f))));

        btnEnd->runAction(FadeTo::create(0.3f, 0));
        btnUserCoin->runAction(FadeTo::create(0.3f, 0));
        workShopLabel->runAction(FadeTo::create(0.3f, 0));
	});
    
    this->setDefaultCallback([=](Node* sender){
        this->End(sender);
    });

	return true;
}

void CWorkshopPopup::End(Node* sender){
    CObjectManager::Instance()->ZoomOutRank();
    CGameScene::getGameScene()->MenuFadeIn();
    
	this->popupClose(1.3f);
}