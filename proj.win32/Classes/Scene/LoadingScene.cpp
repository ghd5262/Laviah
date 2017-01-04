#include "LoadingScene.h"
#include "MenuScene.h"
#include "EmptyScene.h"
#include "GameScene.h"
#include "../GameObject/Player.h"
#include "../MyUI/MyButton.h"
#include "../Download/DownloadManager.h"
#include "../DataManager/UserDataManager.h"
#include "../SDKUtil/SDKUtil.h"
#include "../MyUI/Popup.h"
#include "../Common/NoticeDefine.h"

USING_NS_CC;
//USING_NS_CC_EXT;

CLoadingScene* CLoadingScene::m_LoadingScene = nullptr;

Scene* CLoadingScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = CLoadingScene::create();
	// add layer as a child to scene
	scene->setAnchorPoint(Vec2(0, 0));
	layer->setAnchorPoint(Vec2(0, 0));
	scene->addChild(layer);

	return scene;
}

CLoadingScene::~CLoadingScene()
{
	__NotificationCenter::getInstance()->removeAllObservers(this);
	removeAllChildrenWithCleanup(true);
	clearData();
}

void CLoadingScene::clearData()
{
	m_LoadingScene = nullptr;
}

bool CLoadingScene::init()
{
	scheduleUpdate();
	if (!Layer::init())
	{
		return false;
	}

	if (!initVariable())
		return false;
	return true;
}

bool CLoadingScene::initVariable()
{
	clearData();
	m_LoadingScene = this;

	auto createNotice = [=](cocos2d::SEL_CallFuncO selector, std::string name){
		__NotificationCenter::getInstance()->addObserver(this, selector, name, NULL);
	};

	createNotice(callfuncO_selector(CLoadingScene::callbackNetworkResult), NOTICE::NETWORK_RESULT);
	createNotice(callfuncO_selector(CLoadingScene::callbackLoginResult), NOTICE::LOGIN_RESULT);
	createNotice(callfuncO_selector(CLoadingScene::callbackUserDataLoadFinish), NOTICE::USERDATA_LOAD_FINISH);
	createNotice(callfuncO_selector(CLoadingScene::callbackDownloadFail), NOTICE::DOWN_ERROR);
	createNotice(callfuncO_selector(CLoadingScene::callbackDownloadComplete), NOTICE::DOWN_COMPLETE);

	CUserDataManager::Instance();
	CSDKUtil::Instance()->IsNetworkConnect();

	this->setContentSize(Director::getInstance()->getVisibleSize());

	InitLoadingSceneUI();

	return true;
}

void CLoadingScene::InitLoadingSceneUI()
{

}

void CLoadingScene::callbackNetworkResult(Ref* object)
{
	// 첫 실행 이라면 인터넷 연결 하라는 팝업
	if (CUserDataManager::Instance()->getIsFirstPlay() &&
		!CSDKUtil::Instance()->getIsNetworkConnect())
	{
		createNetworkConnectPopup();
	}
	else{
		if (CSDKUtil::Instance()->getIsNetworkConnect()){
			// 인터넷 연결되어 있다면 패키지 버전 비교 후 정상 실행
			m_Downlaoder = CDownloadManager::create();
			m_Downlaoder->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_Downlaoder->setPosition(Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.45f));
			addChild(m_Downlaoder);
		}
		else{
			__NotificationCenter::getInstance()->postNotification(NOTICE::DOWN_COMPLETE, nullptr);
		}
	}
}

void CLoadingScene::callbackDownloadFail(Ref* object)
{
    CPopup::create()
    ->setPositiveButton([=](Node* sender){
		if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			this->callbackLoginResult(object);
        CCLOG("Download Faild");
    }, "OK")
    ->setDefaultAnimation(ePOPUP_ANIMATION::OPEN_CENTER, ePOPUP_ANIMATION::CLOSE_CENTER)
    ->setMessage("Download Failed")
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(this->getContentSize() / 2)
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->show(this, ZORDER::POPUP);
}

void CLoadingScene::callbackDownloadComplete(Ref* object)
{
	if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
		CSDKUtil::Instance()->GoogleLogin();
	}
	else{
		callbackLoginResult(object);
	}
}

void CLoadingScene::callbackLoginResult(Ref* object)
{
	CUserDataManager::Instance()->initUserDefaultValue();
	CUserDataManager::Instance()->UserDataLoad();
}

void CLoadingScene::callbackUserDataLoadFinish(Ref* object)
{
	// 데이터 로딩 완료 후 패키지 다운로드
	createMenuScene();
}

void CLoadingScene::createMenuScene()
{
	Director::getInstance()->getScheduler()->schedule([=](float delta){

		auto tempScene = CEmptyScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.8f, tempScene));

		Director::getInstance()->getScheduler()->schedule([=](float delta){
			auto Scene = CGameScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.8f, Scene));
		}, Director::getInstance(), 0.f, 0, 1.f, false, "createGameScene");

	}, Director::getInstance(), 0.f, 0, 0.f, false, "createEmptyScene");
}

void CLoadingScene::createNetworkConnectPopup()
{
    CPopup::create()
    ->setPositiveButton([=](Node* sender){
        CSDKUtil::Instance()->IsNetworkConnect();
    }, "OK")
	->setDefaultAnimation(ePOPUP_ANIMATION::OPEN_CENTER, ePOPUP_ANIMATION::CLOSE_CENTER)
    ->setMessage("Please connect to the internet \n to download resources.")
	->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
	->setPopupPosition(this->getContentSize() / 2)
	->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
	->show(this, ZORDER::POPUP);
}

void CLoadingScene::update(float delta)
{
	//    m_LoadingImg->Rotation(1, delta);
}