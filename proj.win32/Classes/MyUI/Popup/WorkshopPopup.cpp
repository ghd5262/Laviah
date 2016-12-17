#include "WorkshopPopup.h"
#include "WorkshopPopupDP.h"
#include "../MyButton.h"
#include "../UserCoinButton.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/WorkshopItemDataManager.h"
#include "../../DataManager/UserDataManager.h"
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

	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto scrollBack = LayerColor::create(COLOR::WHITEGRAY_ALPHA, 1080.f, 1500.f);
	if (scrollBack != nullptr){
		scrollBack->setIgnoreAnchorPointForPosition(false);
		scrollBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		scrollBack->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 1.5f));
		this->addChild(scrollBack);
	}

	/* workShop label*/
	auto workShopLabel = Label::createWithTTF("Workshop", FONT::MALGUNBD, 80);
	if (workShopLabel != nullptr)
	{
		workShopLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		workShopLabel->setPosition(Vec2(scrollBack->getContentSize().width * 0.5f, scrollBack->getContentSize().height * 0.9f));
		workShopLabel->setColor(COLOR::DARKGRAY);
		workShopLabel->setOpacity(0);
		scrollBack->addChild(workShopLabel);
	}

	auto itemList = CWorkshopItemDataManager::Instance()->getSellingWorkshopItemList();
	Size layerSize = scrollBack->getContentSize();
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
		listView->setContentSize(Size(scrollBack->getContentSize().width, (dpSize.height + dpDistance) * spawnCount));
		listView->setScrollBarPositionFromCorner(Vec2(7, 7));
		listView->setItemsMargin(dpDistance);
		listView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		listView->setPosition(layerSize / 2);
		listView->setMagneticType(ListView::MagneticType::CENTER);
		listView->ScrollView::addEventListener((ui::ListView::ccScrollViewCallback)std::bind(&CWorkshopPopup::ScrollCallback, this, std::placeholders::_1, std::placeholders::_2));
		scrollBack->addChild(listView);

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
		->setButtonNormalImage("endIcon.png")
		->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
		->setButtonPosition(Vec2(this->getContentSize().width * 0.92f, this->getContentSize().height * 0.05f))
		->show(this);
	btnEnd->setCascadeOpacityEnabled(true);
	btnEnd->setOpacity(0);


	auto btnUserCoin = CUserCoinButton::create();
	if (btnUserCoin != nullptr)
	{
		btnUserCoin->setPosition(Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.05f));
		btnUserCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		btnUserCoin->setCascadeOpacityEnabled(true);
		btnUserCoin->setOpacity(0);
		this->addChild(btnUserCoin);
	}

	this->setOpenAnimation([=](Node* sender){
		scrollBack->runAction(EaseExponentialOut::create(MoveTo::create(0.8f, Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.62f))));

		workShopLabel->runAction(FadeIn::create(0.5f));
		btnEnd->runAction(FadeIn::create(0.5f));
		btnUserCoin->runAction(FadeIn::create(0.5f));
	});

	this->setCloseAnimation([=](Node* sender){
		scrollBack->runAction(EaseSineIn::create(MoveTo::create(0.4f, Vec2(visibleSize.width * 0.5f, visibleSize.height * 1.5f))));

		workShopLabel->runAction(FadeTo::create(0.5f, 0));
		btnEnd->runAction(FadeTo::create(0.5f, 0));
		btnUserCoin->runAction(FadeTo::create(0.5f, 0));
	});

	return true;
}

void CWorkshopPopup::End(Node* sender){
	CCLOG("format popup End");
	this->popupClose();
}

void CWorkshopPopup::ScrollCallback(cocos2d::Ref* ref, cocos2d::ui::ScrollView::EventType type)
{
	ListView* listView = dynamic_cast<ListView*>(ref);
	if (listView == nullptr || type != ScrollView::EventType::CONTAINER_MOVED)
	{
		return;
	}
}