#include "MenuLayer.hpp"
#include "MyButton.h"
#include "Popup/CharacterSelectPopup.h"
#include "Popup/WorkshopPopup.h"
#include "../Scene/GameScene.h"

using namespace cocos2d;
using namespace cocos2d::ui;

CMenuLayer* CMenuLayer::create()
{
    CMenuLayer *pRet = new(std::nothrow) CMenuLayer();
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
bool CMenuLayer::init()
{
    if (!CPopup::init()) return false;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();

	Size popupSize = this->getContentSize();

	auto createOneButtonPopup = [=](std::string message){
		CPopup::create()
			->setPositiveButton([=](Node* sender){}, "OK")
			->setDefaultAnimation(ePOPUP_ANIMATION::OPEN_CENTER, ePOPUP_ANIMATION::CLOSE_CENTER)
			->setMessage(message)
			->setMessageFont(Color3B::WHITE, 40)
			->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
			->setPopupPosition(popupSize / 2)
			->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
			->show(this);
	};

	auto createWidgetPopup = [=](CPopup* widget){
		widget->setPopupPosition(popupSize / 2)
			->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
			->show(this);
	};

	auto createTestButton = [=](const std::function<void(Node*)> &callback, std::string name, Vec2 pos, Size size){
		return CMyButton::create()
			->addEventListener(callback)
			->setLayer(LayerColor::create(COLOR::BRIGHTRED_ALPHA, size.width, size.height))
			->setContents(name)
			->setFont(Color3B::WHITE, 40)
			->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
			->setButtonPosition(pos)
			->show(this);
	};

	Vec2 testButtonPos[] = {
		Vec2(popupSize.width * 0.2f, popupSize.height * 0.25f),
		Vec2(popupSize.width * 0.4f, popupSize.height * 0.25f),
		Vec2(popupSize.width * 0.6f, popupSize.height * 0.25f),
		Vec2(popupSize.width * 0.8f, popupSize.height * 0.25f),
	};

	createTestButton([=](Node* sender){ createOneButtonPopup("Share is comming soon"); }, "Share", testButtonPos[0], Size(200, 150));
	createTestButton([=](Node* sender){	createWidgetPopup(CCharacterSelectPopup::create());	}, "Select", testButtonPos[1], Size(200, 150));
	createTestButton([=](Node* sender){
		//CGameSceneUI::create()
		//	->setPopupPosition(popupSize / 2)
		//	->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
		//	->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
		//	->show(this, 102);

		CGameScene::getGameScene()->GameStart();

	}, "Start", testButtonPos[2], Size(200, 150));
	createTestButton([=](Node* sender){	createWidgetPopup(CWorkshopPopup::create()); }, "Work", testButtonPos[3], Size(200, 150));

    this->setOpenAnimation([=](Node* sender){
    });
    
    this->setCloseAnimation([=](Node* sender){
    });
    
    return true;
}