#include "AppDelegate.h"
#include "Scene/GameScene.h"
#include "Scene/MenuScene.h"
#include "Scene/LoadingScene.h"
#include "Common/AudioManager.h"
#include "DataManager/UserDataManager.h"
#include "SDKBOX/SDKBox.h"

USING_NS_CC;

AppDelegate::AppDelegate() {
    
}

AppDelegate::~AppDelegate()
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };
    
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    
    CCLOG("AppDelegate %s", __FUNCTION__);
    auto createWritablePath = [=](std::string path){
        auto writable = FileUtils::getInstance()->getWritablePath();
        auto fullPath = writable + path + "/";
        FileUtils::getInstance()->createDirectory(fullPath);
        return fullPath;
    };
    
    int i = 0;
    FileUtils *fileUtils = FileUtils::getInstance();
    auto searchPaths = fileUtils->getSearchPaths();
    searchPaths.insert(searchPaths.begin() + i++, "imageRes");
    searchPaths.insert(searchPaths.begin() + i++, createWritablePath("update"));
    searchPaths.insert(searchPaths.begin() + i++, createWritablePath("remoteImage"));
    fileUtils->setSearchPaths(searchPaths);
    
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
        glview = GLViewImpl::create("My Game");
        glview->setFrameSize(360, 640);
        director->setOpenGLView(glview);
    }
    director->getOpenGLView()->setDesignResolutionSize(1080, 1920, ResolutionPolicy::SHOW_ALL);
    
    // turn on display FPS
    director->setDisplayStats(true);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    // create a scene. it's an autorelease object
    auto scene = CLoadingScene::createScene();
    
    // run
    director->runWithScene(scene);
    
    // facebook init
    sdkbox::PluginFacebook::init();    
    
    // sdkbox play init
//    sdkbox::PluginSdkboxPlay::init();
    CPlayManager::Instance()->Initialize();
    
    // unity ads init
    CUnityAdsManager::Instance()->Initialize(true);
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    CAudioManager::Instance()->AllPause();
    
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    CAudioManager::Instance()->AllResume();
    
    //GamePause
    if(CGameScene::getGameScene() != nullptr){
        CGameScene::getGameScene()->OpenGamePausePopup();
    }
    
    CUserDataManager::Instance()->SaveUserDataAutomatically();
    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
