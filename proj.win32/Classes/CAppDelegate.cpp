#include "CAppDelegate.h"
#include "Scene/GameScene.h"
#include "Common/AudioManager.h"
#include "MyUI/Popup.h"
#include "MyUI/Popup/PausePopup.h"
#include "GameObject/Player.h"
#include "GameObject/ObjectManager.h"

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
    int i = 0;
    FileUtils *fileUtils = FileUtils::getInstance();
    auto searchPaths = fileUtils->getSearchPaths();
    searchPaths.insert(searchPaths.begin() + i++, "imageRes");
    fileUtils->setSearchPaths(searchPaths);
    
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
        glview = GLViewImpl::create("My Game");
        glview->setFrameSize(720, 1280);
        director->setOpenGLView(glview);
    }
    director->getOpenGLView()->setDesignResolutionSize(1080, 1920, ResolutionPolicy::SHOW_ALL);
    
    // turn on display FPS
    director->setDisplayStats(true);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    // create a scene. it's an autorelease object
    auto scene = CGameScene::createScene();
    
    // run
    director->runWithScene(scene);
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    CAudioManager::Instance()->AllPause();
    
    //GamePause
    if(CGameScene::getGameScene() != nullptr){
        if(CObjectManager::Instance()->getPlayer() != nullptr
           && !CObjectManager::Instance()->getPlayer()->getIsDead()
           && !CObjectManager::Instance()->getIsGamePause()){
            
            Size visibleSize = Director::getInstance()->getVisibleSize();
            Vec2 origin = Director::getInstance()->getVisibleOrigin();
            
            auto popup = CPopup::createWithSpecificFormat(CPausePopup::create(), POPUPEFFECT_none);
            popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
                                    origin.x + visibleSize.height * 0.5f));
            popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            CGameScene::getGridWorld()->addChild(popup, 102);
            CGameScene::getGameScene()->GamePause();
        }
    }
    
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    CAudioManager::Instance()->AllResume();
    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
