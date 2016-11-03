#include "LoadingScene.h"
#include "MenuScene.h"
#include "EmptyScene.h"
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
	try
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

		InitLoadingSceneUI();
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void CLoadingScene::InitLoadingSceneUI()
{

}

void CLoadingScene::callbackNetworkResult(Ref* object)
{
	// ù ���� �̶�� ���ͳ� ���� �϶�� �˾�
	if (CUserDataManager::Instance()->getIsFirstPlay() &&
		!CSDKUtil::Instance()->getIsNetworkConnect())
	{
		createNetworkConnectPopup();
	}
	else{
		if (CSDKUtil::Instance()->getIsNetworkConnect()){
			// ���ͳ� ����Ǿ� �ִٸ� ��Ű�� ���� �� �� ���� ����
			m_Downlaoder = CDownloadManager::create();
			addChild(m_Downlaoder);
		}
		else{
			__NotificationCenter::getInstance()->postNotification(NOTICE::DOWN_COMPLETE, nullptr);
		}
	}
}

void CLoadingScene::callbackDownloadFail(Ref* object)
{

}

void CLoadingScene::callbackDownloadComplete(Ref* object)
{
	// �α���
	CSDKUtil::Instance()->GoogleLogin();
}

void CLoadingScene::callbackLoginResult(Ref* object)
{
	CUserDataManager::Instance()->initUserDefaultValue();
	CUserDataManager::Instance()->UserDataLoad();
}

void CLoadingScene::callbackUserDataLoadFinish(Ref* object)
{
	// ������ �ε� �Ϸ� �� ��Ű�� �ٿ�ε�
	createMenuScene();
}

void CLoadingScene::createMenuScene()
{
	Director::getInstance()->getScheduler()->schedule([=](float delta){

		auto tempScene = CEmptyScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.8f, tempScene));

		Director::getInstance()->getScheduler()->schedule([=](float delta){
			auto Scene = CMenuScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.8f, Scene));
		}, Director::getInstance(), 0.f, 0, 1.f, false, "createMenuScene");

	}, Director::getInstance(), 0.f, 0, 0.f, false, "createEmptyScene");
}

void CLoadingScene::createNetworkConnectPopup()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	CPopup::create()
		->setPositiveButton([=](Node* sender){
		CSDKUtil::Instance()->IsNetworkConnect();
		}, "OK")
		->setMessage("Please connect to the internet \n to download resources.")
		->show(this);
}

void CLoadingScene::update(float delta)
{
	//    m_LoadingImg->Rotation(1, delta);
}