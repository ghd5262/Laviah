#include "MenuLayer.hpp"
#include "MyButton.h"
#include "Popup/CharacterSelectPopup.h"
#include "Popup/WorkshopPopup.h"
#include "../Scene/GameScene.h"
#include "../DataManager/UserDataManager.h"
#include "../GameObject/ObjectManager.h"
#include "../GameObject/Rocket.h"
#include "../AI/States/RocketStates.h"
#include <array>

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
    
	Size popupSize = this->getContentSize();
    this->setCascadeColorEnabled(true);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(false);
	touchListener->onTouchBegan = CC_CALLBACK_2(CMenuLayer::TouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(CMenuLayer::TouchScreen, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	auto createOneButtonPopup = [=](const std::function<void(Node*)> &callback, std::string message){
        CPopup::create()
        ->setDefaultCallback([=](Node* sender){
            auto popup = dynamic_cast<CPopup*>(sender);
            popup->popupClose();
        })
        ->setPositiveButton(callback, "OK")
        ->setDefaultAnimation(ePOPUP_ANIMATION::OPEN_CENTER, ePOPUP_ANIMATION::CLOSE_CENTER)
        ->setMessage(message)
        ->setMessageFont(Color3B::WHITE, 40)
        ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
        ->setPopupPosition(popupSize / 2)
        ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->show(this);
	};
    
    auto createTwoButtonPopup = [=](const std::function<void(Node*)> &callback, std::string message){
        CPopup::create()
        ->setPositiveButton(callback, "YES")
        ->setNegativeButton([](Node* sender){}, "NO")
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
		auto btn = CMyButton::create()
			->addEventListener(callback)
			->setLayer(LayerColor::create(COLOR::BRIGHTRED_ALPHA, size.width, size.height))
			->setContents(name)
			->setFont(Color3B::WHITE, 40)
			->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
			->setButtonPosition(pos)
			->show(this);
        btn->setCascadeOpacityEnabled(true);
        return btn;
	};

	Vec2 testButtonPos[] = {
		Vec2(popupSize.width * 0.2f, popupSize.height * 0.25f),
		Vec2(popupSize.width * 0.4f, popupSize.height * 0.25f),
		Vec2(popupSize.width * 0.6f, popupSize.height * 0.25f),
		Vec2(popupSize.width * 0.8f, popupSize.height * 0.25f),
        Vec2(popupSize.width * 0.1f, popupSize.height * 0.95f)
	};
    
    std::array<CMyButton*, 5> btnArray = {
        createTestButton([=](Node* sender){
            createOneButtonPopup([](Node* sender){}, "Share is comming soon");
        }, "Share", testButtonPos[0], Size(200, 150)),
        
        createTestButton([=](Node* sender){
            createWidgetPopup(CCharacterSelectPopup::create());
        }, "Select", testButtonPos[1], Size(200, 150)),
        
        createTestButton([=](Node* sender){
			CObjectManager::Instance()->getRocket()->ChangeState(CFlyAway::Instance());
            CGameScene::getGameScene()->GameStart();
            this->popupClose();
        }, "Start", testButtonPos[2], Size(200, 150)),
        
        createTestButton([=](Node* sender){
            createWidgetPopup(CWorkshopPopup::create());
        }, "Work", testButtonPos[3], Size(200, 150)),
        
        createTestButton([=](Node* sender){
            createTwoButtonPopup([](Node* sender){
                CUserDataManager::Instance()->setUserData_ItemRemoveAll(USERDATA_KEY::CHALLENGE_CUR_LIST);
                CUserDataManager::Instance()->setUserData_ItemGet(USERDATA_KEY::CHALLENGE_CUR_LIST, 0);
                CUserDataManager::Instance()->setUserData_ItemGet(USERDATA_KEY::CHALLENGE_CUR_LIST, 1);
                CUserDataManager::Instance()->setUserData_ItemGet(USERDATA_KEY::CHALLENGE_CUR_LIST, 2);

				CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::BEST_SCORE, 0);
				CObjectManager::Instance()->getRocket()->ChangeState(CFlyAway::Instance());
                CGameScene::getGameScene()->RandomCoin();
            }, "Are you sure want reset user data?");
        }, "R", testButtonPos[4], Size(100, 100)),
    };
    
    this->setDefaultCallback([=](Node* sender){
        CGameScene::getGameScene()->GameExit(false);
    }, false);

    return true;
}

bool CMenuLayer::TouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent)
{
	Point touchLocation = pTouch->getLocationInView();
	touchLocation = Director::getInstance()->convertToGL(touchLocation);
	CGameScene::getGameScene()->setTouchPos(convertToNodeSpace(touchLocation));
	CObjectManager::Instance()->getRocket()->ChangeState(CFlyToTouchArea::Instance());

	return true;
}

void CMenuLayer::TouchScreen(cocos2d::Touch* pTouch, cocos2d::Event* pEvent)
{
	Point touchLocation = pTouch->getLocationInView();
	touchLocation = Director::getInstance()->convertToGL(touchLocation);
	CGameScene::getGameScene()->setTouchPos(convertToNodeSpace(touchLocation));
	CObjectManager::Instance()->getRocket()->ChangeState(CFlyToTouchArea::Instance());

	//return true;
}
