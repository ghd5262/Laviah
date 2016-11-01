#include "LoadingScene.h"
#include "MenuScene.h"
#include "EmptyScene.h"
#include "../GameObject/Player.h"
#include "../MyUI/MyButton.h"
#include "../Download/DownloadManager.h"

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
        
        __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CLoadingScene::onAssetUpdateError), "CLoadingScene::onAssetUpdateError", NULL);
        
        __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CLoadingScene::onAssetUpdateComplete), "CLoadingScene::onAssetUpdateComplete", NULL);
        
        m_Downlaoder = CDownloadManager::create();
        addChild(m_Downlaoder);
        
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


//assets downloader callback
void CLoadingScene::onAssetUpdateError(Ref* object)
{
    
}

void CLoadingScene::onAssetUpdateComplete(Ref* object)
{
    createMenuScene();
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