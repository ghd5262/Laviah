#include "LoadingScene.h"
#include "GameScene.h"
#include "../Download/DownloadManager.h"
#include "../DataManager/UserDataManager.h"
#include "../MyUI/Popup.h"

USING_NS_CC;

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

CLoadingScene::CLoadingScene()
: m_DownloadRetryCount(3){};

CLoadingScene::~CLoadingScene(){}

bool CLoadingScene::init()
{
    CCLOG("Loading Scene %s", __FUNCTION__);
	if (!Layer::init())
        return false;
    
    this->setContentSize(Director::getInstance()->getVisibleSize());
    this->startDownload();

	return true;
}

void CLoadingScene::startDownload()
{
    CCLOG("Loading Scene %s", __FUNCTION__);
    CDownloadManager::IsNetworkConnected([=](bool isConnected){
        if(isConnected){
            m_DownloadRetryCount--;
            
            // 인터넷 연결되어 있다면 패키지 버전 비교 후 정상 실행
            auto downloadManager = CDownloadManager::create();
            downloadManager->setDownloadFailedListener([=](){
                CCLOG("Download retry count %d", m_DownloadRetryCount);
                if(m_DownloadRetryCount)    this->startDownload();
                else                        this->callbackDownloadFail();
            });
            downloadManager->setDownloadSucceedListener([=](){
                this->callbackDownloadComplete();
            });
            downloadManager->setRequireNextVersion([=](){
                auto appUrl = downloadManager->getAppUrl();
                this->callbackRequireLatestVersion(appUrl);
            });
            downloadManager->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            downloadManager->setPosition(Vec2(this->getContentSize().width * 0.5f,
                                              this->getContentSize().height * 0.45f));
            this->addChild(downloadManager);
        }
        else{
            auto firstPlay   = CUserDataManager::Instance()->getIsFirstPlay();
            auto lastVersion = CUserDataManager::Instance()->getLastResourcesVersion();
            auto curVersion  = Application::getInstance()->getVersion();
            
            auto downloadEnable = (firstPlay || (lastVersion != curVersion));
            
            if(downloadEnable) this->createNetworkConnectPopup();
            else               this->callbackDownloadComplete();
            
        }
    });
}

void CLoadingScene::callbackDownloadFail()
{
	CCLOG("Loading Scene %s", __FUNCTION__);
    CPopup::create()
    ->setPositiveButton([=](Node* sender){
        m_DownloadRetryCount = 3;
        this->startDownload();
    }, "OK")
    ->setDefaultAnimation(ePOPUP_ANIMATION::OPEN_CENTER, ePOPUP_ANIMATION::CLOSE_CENTER)
    ->setMessage("Download failed. \nCheck your internet connection. \nIf you see this message again after connecting, please email to the developer.")
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(this->getContentSize() / 2)
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->show(this, ZORDER::POPUP);
}

void CLoadingScene::callbackDownloadComplete()
{
	CCLOG("Loading Scene %s", __FUNCTION__);
    CUserDataManager::Instance()->Initialize();
    this->createMenuScene();
}

void CLoadingScene::callbackRequireLatestVersion(std::string appUrl)
{
    CPopup::create()
    ->setPositiveButton([=](Node* sender){
        Application::getInstance()->openURL(appUrl);
    }, "OK")
    ->setDefaultAnimation(ePOPUP_ANIMATION::OPEN_CENTER, ePOPUP_ANIMATION::CLOSE_CENTER)
    ->setMessage("Please download the latest version of the game.")
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(this->getContentSize() / 2)
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->show(this, ZORDER::POPUP);
}

void CLoadingScene::createMenuScene()
{
	CCLOG("Loading Scene %s", __FUNCTION__);
    auto Scene = CGameScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.8f, Scene));
}

void CLoadingScene::createNetworkConnectPopup()
{
    CPopup::create()
    ->setPositiveButton([=](Node* sender){
        this->startDownload();
    }, "OK")
	->setDefaultAnimation(ePOPUP_ANIMATION::OPEN_CENTER, ePOPUP_ANIMATION::CLOSE_CENTER)
    ->setMessage("Please connect the network to download resources of the new version of game.")
	->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
	->setPopupPosition(this->getContentSize() / 2)
	->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
	->show(this, ZORDER::POPUP);
}
