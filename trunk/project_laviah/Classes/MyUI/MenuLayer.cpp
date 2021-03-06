#include "MenuLayer.hpp"
#include "MyButton.h"
#include "Popup/WorkshopPopup.h"
#include "Popup/Option/OptionPopup.hpp"
#include "Popup/TitleCompleteNoticePopup.hpp"
#include "../Scene/GameScene.h"
#include "../DataManager/UserDataManager.h"
#include "../Download/DownloadManager.h"
#include "../SDKBOX/SDKBoxHeaders.h"
#include "../GameObject/ObjectManager.h"
#include "../GameObject/Rocket.h"
#include "../GameObject/Planet.h"
#include "../GameObject/Player.h"
#include "../AI/States/RocketStates.h"
#include <time.h>
#include <array>

using namespace cocos2d;
using namespace cocos2d::ui;
CMenuLayer* CMenuLayer::m_Instance = nullptr;

CMenuLayer::~CMenuLayer()
{
    m_Instance = nullptr;
}

CMenuLayer* CMenuLayer::Instance()
{
    if(m_Instance != nullptr) return m_Instance;
    
    m_Instance = new(std::nothrow) CMenuLayer();
    if (m_Instance && m_Instance->init())
    {
        m_Instance->autorelease();
        return m_Instance;
    }
    else
    {
        delete m_Instance;
        m_Instance = NULL;
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
        CGameScene::getGameScene()->CreateAlertPopup()
        ->setPositiveButton(callback, TRANSLATE("BUTTON_OK"))
        ->setMessage(message)
        ->show(CGameScene::getPopupLayer(), ZORDER::POPUP);
	};
    
    auto createTwoButtonPopup = [=](const std::function<void(Node*)> &callback, std::string message){
		CGameScene::getGameScene()->CreateAlertPopup()
        ->setPositiveButton(callback, TRANSLATE("BUTTON_YES"))
        ->setNegativeButton([](Node* sender){}, TRANSLATE("BUTTON_NO"))
        ->setMessage(message)
		->show(CGameScene::getPopupLayer(), ZORDER::POPUP);
    };

	auto createWidgetPopup = [=](CPopup* widget){
		widget->setPopupPosition(popupSize / 2)
			->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
			->show(this);
	};

	auto createLayerButton = [=](const std::function<void(Node*)> &callback, std::string name, Vec2 pos, Size size){
		auto btn = CMyButton::create()
			->addEventListener(callback)
			->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, size.width, size.height))
			->setContents(name)
			->setFont(Color3B::WHITE, 40)
			->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
			->setButtonPosition(pos)
			->show(this);
        return btn;
	};
    
    auto createButton = [=](const std::function<void(Node*)> &callback, std::string iconName, Vec2 pos){
        auto btn = CMyButton::create()
        ->addEventListener(callback)
        ->setButtonNormalImage(iconName)
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
        Vec2(popupSize.width * 0.1f, popupSize.height * 0.95f),
        Vec2(popupSize.width * 0.2f, popupSize.height * 0.95f),
        Vec2(popupSize.width * 0.3f, popupSize.height * 0.95f)
	};

    /*std::array<CMyButton*, 7> btnArray = {
        createTestButton([=](Node* sender){
            createOneButtonPopup([](Node* sender){}, "Share is comming soon");
        }, TRANSLATE("BUTTON_MENU_SHARE"), testButtonPos[0], Size(200, 150)),
        
        createTestButton([=](Node* sender){
            createWidgetPopup(CCharacterSelectPopup::create());
        }, TRANSLATE("BUTTON_MENU_SELECT"), testButtonPos[1], Size(200, 150)),
        
        createTestButton([=](Node* sender){
			CObjectManager::Instance()->getRocket()->ChangeState(CFlyAway::Instance());
            CGameScene::getGameScene()->GameStart();
            this->popupClose();
        }, TRANSLATE("BUTTON_MENU_START"), testButtonPos[2], Size(200, 150)),
        
        createTestButton([=](Node* sender){
            createWidgetPopup(CWorkshopPopup::create());
		}, TRANSLATE("BUTTON_MENU_WORKSHOP"), testButtonPos[3], Size(200, 150)),

//        createTestButton([=](Node* sender){
//			auto savedTime = CUserDataManager::Instance()->getLastSavedTime();
//            auto msg = TRANSLATE("USERDATA_SAVE_CHECK") + std::string("\n") + StringUtils::format(TRANSLATE("USERDATA_LAST_SAVED_TIME").c_str(),
//				savedTime->tm_year + 1900,
//				savedTime->tm_mon + 1,
//				savedTime->tm_mday,
//				savedTime->tm_hour,
//				savedTime->tm_min,
//				savedTime->tm_sec);
//            createTwoButtonPopup([](Node* sender){
//                //CUserDataManager::Instance()->setUserData_ItemRemoveAll(USERDATA_KEY::ACHIEVEMENT_CUR_LIST);
//                //CUserDataManager::Instance()->setUserData_ItemGet(USERDATA_KEY::ACHIEVEMENT_CUR_LIST, 0);
//                //CUserDataManager::Instance()->setUserData_ItemGet(USERDATA_KEY::ACHIEVEMENT_CUR_LIST, 1);
//                //CUserDataManager::Instance()->setUserData_ItemGet(USERDATA_KEY::ACHIEVEMENT_CUR_LIST, 2);
//
//				//CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::BEST_SCORE, 0);
//				//CObjectManager::Instance()->getRocket()->ChangeState(CFlyAway::Instance());
//                //CGameScene::getGameScene()->RandomCoin();
//				CUserDataManager::Instance()->SaveUserData(true, true);
//			}, msg);
//        }, "S", testButtonPos[4], Size(100, 100)),
        
//        createTestButton([=](Node* sender){
//            createTwoButtonPopup([](Node* sender){
//                CUserDataManager::Instance()->setUserData_Reset();
//                CUserDataManager::Instance()->SaveUserData(true, true);
//            }, "Reset?");
//        }, "R", testButtonPos[5], Size(100, 100)),
    };*/
    
    // workshop button
    createButton([=](Node* sender){
        CGameScene::getGameScene()->OpenWorkshopPopup();
    }, "workshopIcon_6.png", Vec2(popupSize.width * 0.56f, popupSize.height * 0.95f));
    
    // option button
    createButton([=](Node* sender){
        CGameScene::getGameScene()->OpenOptionPopup(0);
    }, "optionIcon.png", Vec2(popupSize.width * 0.08f, popupSize.height * 0.95f));
    
    createButton([=](Node* sender){
        CGameScene::getGameScene()->OpenPlanetSelectPopup();
    }, "planetSelectIcon.png", Vec2(popupSize.width * 0.92f, popupSize.height * 0.95f));
    
    
//    // option button
//    createButton([=](Node* sender){
//        CPlayManager::Instance()->ScoreSave([=](){}, "Ella", 2);
//        CPlayManager::Instance()->ScoreSave([=](){}, "Riva", 3);
//        CPlayManager::Instance()->ScoreSave([=](){}, "Arid", 4);
//        CPlayManager::Instance()->ScoreSave([=](){}, "Oculus", 5);
//        CPlayManager::Instance()->ScoreSave([=](){}, "Reverse", 6);
//
////        CGameScene::getGameScene()->OpenFBTestPopup();
//    }, "optionIcon.png", Vec2(popupSize.width * 0.08f, popupSize.height * 0.05f));
    
    // map button
//    createButton([=](Node* sender){
//        createOneButtonPopup([=](Node* sender){}, "Coming soon");
//    }, "mapIcon.png", Vec2(popupSize.width * 0.08f, popupSize.height * 0.05f));
    
    // facebook rank button
    createButton([=](Node* sender){
        auto button = dynamic_cast<CMyButton*>(sender);
        button->setTouchEnable(false, Color3B::WHITE);
        CDownloadManager::IsNetworkConnected([=](bool isConnected){
            if(isConnected){
                CGameScene::getGameScene()->OpenPermRequestPopup([=](){
                    CGameScene::getGameScene()->OpenRankPopup();
                });
            }
            else{
                createOneButtonPopup([=](Node* sender){}, TRANSLATE("NETWORK_CONNECT_CHECK"));
            }
            button->setTouchEnable(true);
        });
        
    }, "rankingIcon.png", Vec2(popupSize.width * 0.2f, popupSize.height * 0.95f));
    
    // achievement button
    m_AchievementButton = createButton([=](Node* sender){
        CGameScene::getGameScene()->OpenAchievementPopup();
    }, "achievementIcon.png", Vec2(popupSize.width * 0.32f, popupSize.height * 0.95f));
    
    m_NewIcon = Sprite::create("achievementIconNew_2.png");
    m_NewIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_NewIcon->setPosition(m_AchievementButton->getContentSize() / 2);
    m_NewIcon->setVisible(false);
    m_AchievementButton->addChild(m_NewIcon);
    
    // character select button
    createButton([=](Node* sender){
        CGameScene::getGameScene()->OpenCharacterPopup();
    }, "characterIcon.png", Vec2(popupSize.width * 0.44f, popupSize.height * 0.95f));
    
    // start button
    auto startBtn = CMyButton::create()
    ->addEventListener([=](Node* sender){
        dynamic_cast<CMyButton*>(sender)->setTouchEnable(false);
        CGameScene::getGameScene()->ScreenFade([=](){
            dynamic_cast<CMyButton*>(sender)->setTouchEnable(true);
            CGameScene::getGameScene()->GameStart();
        });
    })
    ->setEnableSound(false)
    ->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, popupSize.width, popupSize.height * 0.2f))
    ->setContents(TRANSLATE("BUTTON_MENU_START"), Vec2(0.5f, 0.5f))
    ->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.2f))
    ->show(this);
    startBtn->setSwallowTouches(false);
    
    CTranslateManager::Instance()->addTranslateEventListener([=](){
        startBtn->changeContents(TRANSLATE("BUTTON_MENU_START"));
    });
    
    CTitleCompleteNoticePopup::create()
    ->setDefaultCallbackEnable(false)
    ->setBackgroundVisible(false)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.8f))
    ->show(this);
    
    this->setDefaultCallback([=](Node* sender){
        CGameScene::getGameScene()->GameExit(false);
    }, false);

    return true;
}

void CMenuLayer::AchievementButtonState(bool enable, bool newIcon/* = false*/)
{
    if(!m_AchievementButton || !m_NewIcon) return;
    
    if(enable) m_AchievementButton->setColor(COLOR::GOLD);
    else       m_AchievementButton->setColor(Color3B::WHITE);
    
    m_NewIcon->setVisible(enable & newIcon);
    m_NewIcon->setColor(Color3B::WHITE);
}

bool CMenuLayer::TouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent)
{
//	Point touchLocation = pTouch->getLocationInView();
//	touchLocation = Director::getInstance()->convertToGL(touchLocation);
//	CGameScene::getGameScene()->setTouchPos(convertToNodeSpace(touchLocation));
//	CObjectManager::Instance()->getRocket()->ChangeState(CFlyToTouchArea::Instance());

	return true;
}

void CMenuLayer::TouchScreen(cocos2d::Touch* pTouch, cocos2d::Event* pEvent)
{
	Point touchLocation = pTouch->getLocationInView();
	touchLocation = Director::getInstance()->convertToGL(touchLocation);
	CGameScene::getGameScene()->setTouchPos(convertToNodeSpace(touchLocation));
//	CObjectManager::Instance()->getRocket()->ChangeState(CFlyToTouchArea::Instance());

	//return true;
}
