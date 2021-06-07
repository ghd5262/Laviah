#include "LoadingScene.h"
#include "GameScene.h"
#include "MenuScene.h"
#include "../Download/DownloadManager.h"
#include "../DataManager/UserDataManager.h"
#include "../MyUI/Popup.h"
#include "../SDKBOX/SDKBoxHeaders.h"

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
: m_DownloadRetryCount(3)
, m_ProgressBar(nullptr)
, m_PercentageLabel(nullptr){};

CLoadingScene::~CLoadingScene(){}

bool CLoadingScene::init()
{
    CCLOG("Loading Scene %s", __FUNCTION__);
	if (!Layer::init())
        return false;
    
    sdkbox::PluginGoogleAnalytics::logScreen(GA_SCREEN::LOAD);

    this->setContentSize(Director::getInstance()->getVisibleSize());
    this->startDownload();
    
    // bg
    {
        auto sprite = Sprite::create("loadingSceneBG.png");
        sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprite->setPosition(this->getContentSize() / 2);
        this->addChild(sprite);
    }
    
    // bar
    {
        auto bar = Sprite::create("achievementProgress.png");
        m_ProgressBar = ProgressTimer::create(bar);
        m_ProgressBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_ProgressBar->setPosition(Vec2(this->getContentSize().width * 0.5f,
                                        this->getContentSize().height * 0.1f));
        m_ProgressBar->setMidpoint(Vec2(0, 0));
        m_ProgressBar->setType(ProgressTimer::Type::BAR);
        m_ProgressBar->setBarChangeRate(Vec2(1, 0));
        m_ProgressBar->setOpacity(255 * 0.4f);
        m_ProgressBar->setScale(0.3f);
        this->addChild(m_ProgressBar);
    }
    
    // label
    {
        m_PercentageLabel = Label::createWithTTF("", FONT::MALGUNBD, 40);
        m_PercentageLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_PercentageLabel->setPosition(Vec2(this->getContentSize().width * 0.5f,
                                            this->getContentSize().height * 0.13f));
        m_PercentageLabel->setColor(Color3B::WHITE);
        this->addChild(m_PercentageLabel);
    }
	return true;
}

void CLoadingScene::startDownload()
{
    CCLOG("Loading Scene %s", __FUNCTION__);
//    CDownloadManager::IsNetworkConnected([=](bool isConnected){
//        if(isConnected){
//            m_DownloadRetryCount--;
//
//            // 인터넷 연결되어 있다면 패키지 버전 비교 후 정상 실행
//            auto downloadManager = CDownloadManager::Instance();
//            downloadManager->setDownloadFailedListener([=](){
//                CCLOG("Download retry count %d", m_DownloadRetryCount);
//                if(m_DownloadRetryCount)    this->startDownload();
//                else                        this->callbackDownloadFail();
//            });
//            downloadManager->setDownloadSucceedListener([=](){
//                this->callbackDownloadComplete();
//            });
//            downloadManager->setRequireNextVersion([=](){
//                auto appUrl = downloadManager->getAppUrl();
//                this->callbackRequireLatestVersion(appUrl);
//            });
//            downloadManager->setFileDownloadProgress([=](int current, int max){
//                this->callbackFileDownloadProgress(current, max);
//            });
//            downloadManager->setFileDecompressProgress([=](int current, int max){
//                this->callbackFileDecompressProgress(current, max);
//            });
//            downloadManager->DownloadStart();
//        }
//        else{
//            auto firstPlay   = CUserDataManager::Instance()->getIsFirstPlay();
//            auto lastVersion = CUserDataManager::Instance()->getLastResourcesVersion();
//            auto curVersion  = Application::getInstance()->getVersion();
//
//            auto downloadEnable = (firstPlay || (lastVersion != curVersion));
//
////            if(downloadEnable) this->createNetworkConnectPopup();
//            this->callbackDownloadComplete();
//
//        }
//    });
    
    Director::getInstance()->getScheduler()->schedule([=](float deltaTime){
        this->callbackDownloadComplete();
    }, this, 0.0f, 0.0f, 3.0f, false, "downloadComplete");
}

float getPercent(float value, float max)
{
    if(value != 0 && max != 0)
        if(value >= max) return 100.f;
    return (value / max) * 100.f;
    return 0.f;
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

void CLoadingScene::callbackFileDownloadProgress(int current, int max)
{
    CCLOG("Download : %d / %d", current, max);
    auto percent = StringUtils::format("Download %d / %d", current, max);
    m_PercentageLabel->setString(percent);
    m_ProgressBar->runAction(ProgressTo::create(0.2f, getPercent(current, max)));
}

void CLoadingScene::callbackFileDecompressProgress(int current, int max)
{
    CCLOG("Decompress : %d / %d", current, max);
    m_ProgressBar->runAction(ProgressTo::create(0.2f, getPercent(current, max)));
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
