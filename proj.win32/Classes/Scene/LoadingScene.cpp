#include "LoadingScene.h"
#include "MenuScene.h"
#include "EmptyScene.h"
#include "../GameObject/Player.h"
#include "../MyUI/MyButton.h"
USING_NS_CC;
USING_NS_CC_EXT;

const char* sceneManifest = "AMTestScene1/project.manifest";
const char* storagePath = "Download/";
const char* backgroundPath = "compressed/Daisy_Flower.jpg";

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
    removeAllChildrenWithCleanup(true);
    clearData();
}

void CLoadingScene::clearData()
{
    m_LoadingScene = nullptr;
    m_AssetsManager = nullptr;
    m_AssetsManagerListener = nullptr;
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
        
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
        auto loadingImg = Sprite::create("player.png");
        loadingImg->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.x + visibleSize.height * 0.5f));
        loadingImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        addChild(loadingImg);
        loadingImg->runAction(RepeatForever::create(RotateBy::create(1.f, 360)));
        
        std::string manifest = sceneManifest;
        std::string storage = FileUtils::getInstance()->getWritablePath() + storagePath;
        CCLOG("Storage path for this test : %s", storage.c_str());
        m_AssetsManager = AssetsManagerEx::create(manifest, storage);
        m_AssetsManager->retain();
        
        
//        sCHARACTER_PARAM tempInfo;
//        tempInfo._normalTextureName = "player0.png";
//        
//        m_LoadingImg = CPlayer::create(tempInfo, 6.f, 0.0f, 400.0f);
//        if(m_LoadingImg != nullptr){
//            m_LoadingImg->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.x + visibleSize.height * 0.5f));
//            this->addChild(m_LoadingImg);
//        }
        
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
    if (!m_AssetsManager->getLocalManifest()->isLoaded())
    {
        CCLOG("Fail to update assets, step skipped.");
        onLoadEnd();
    }
    else
    {
        m_AssetsManagerListener = cocos2d::extension::EventListenerAssetsManagerEx::create(m_AssetsManager, [this](EventAssetsManagerEx* event){
            static int failCount = 0;
            switch (event->getEventCode())
            {
                case EventAssetsManagerEx::EventCode::NEW_VERSION_FOUND: {
                    CCLOG("New version found.");
                    AssetsManagerEx::State state = m_AssetsManager->getState();
                    CCLOG("AM state:%d", (int)state);
                }
                    break;
                case EventAssetsManagerEx::EventCode::ERROR_NO_LOCAL_MANIFEST:
                {
                    CCLOG("No local manifest file found, skip assets update.");
                    this->onLoadEnd();
                }
                    break;
                case EventAssetsManagerEx::EventCode::UPDATE_PROGRESSION:
                {
                    std::string assetId = event->getAssetId();
                    float percent = event->getPercent();
                    std::string str;
                    if (assetId == AssetsManagerEx::VERSION_ID)
                    {
                        str = StringUtils::format("Version file: %.2f", percent) + "%";
                    }
                    else if (assetId == AssetsManagerEx::MANIFEST_ID)
                    {
                        str = StringUtils::format("Manifest file: %.2f", percent) + "%";
                    }
                    else
                    {
                        str = StringUtils::format("%.2f", percent) + "%";
                        CCLOG("%.2f Percent", percent);
                    }
                    
                    CCLOG("%s", str.c_str());
                }
                    break;
                case EventAssetsManagerEx::EventCode::ERROR_DOWNLOAD_MANIFEST:
                case EventAssetsManagerEx::EventCode::ERROR_PARSE_MANIFEST:
                {
                    CCLOG("Fail to download manifest file, update skipped.");
                    this->onLoadEnd();
                }
                    break;
                case EventAssetsManagerEx::EventCode::ALREADY_UP_TO_DATE:
                case EventAssetsManagerEx::EventCode::UPDATE_FINISHED:
                {
                    CCLOG("Update finished. %s", event->getMessage().c_str());
                    this->onLoadEnd();
                }
                    break;
                case EventAssetsManagerEx::EventCode::UPDATE_FAILED:
                {
                    CCLOG("Update failed. %s", event->getMessage().c_str());
                    
                    failCount++;
                    if (failCount < 5)
                    {
                        m_AssetsManager->downloadFailedAssets();
                    }
                    else
                    {
                        CCLOG("Reach maximum fail count, exit update process");
                        failCount = 0;
                        this->onLoadEnd();
                    }
                }
                    break;
                case EventAssetsManagerEx::EventCode::ERROR_UPDATING:
                {
                    CCLOG("Asset %s : %s", event->getAssetId().c_str(), event->getMessage().c_str());
                }
                    break;
                case EventAssetsManagerEx::EventCode::ERROR_DECOMPRESS:
                {
                    CCLOG("%s", event->getMessage().c_str());
                }
                    break;
                default:
                    break;
            }
        });
        Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(m_AssetsManagerListener, 1);
        
        m_AssetsManager->update();
    }

}

void CLoadingScene::onLoadEnd()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto loadEndBtn = CMyButton::create(backgroundPath, END, [](){CCLOG("Load End");});
    if(loadEndBtn != nullptr){
        loadEndBtn->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.x + visibleSize.height * 0.1f));
        loadEndBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        addChild(loadEndBtn);
    }
}

void CLoadingScene::createMenuScene()
{
    Director::getInstance()->getScheduler()->schedule([](float delta){
        
        auto tempScene = CEmptyScene::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(0.8f, tempScene));
        
        Director::getInstance()->getScheduler()->schedule([](float delta){
            auto Scene = CMenuScene::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(0.8f, Scene));
        }, Director::getInstance(), 1.f, 0, 0.f, false, "createMenuScene");
        
    }, Director::getInstance(), 0.f, 0, 0.f, false, "createEmptyScene");
}

void CLoadingScene::update(float delta)
{
//    m_LoadingImg->Rotation(1, delta);
}