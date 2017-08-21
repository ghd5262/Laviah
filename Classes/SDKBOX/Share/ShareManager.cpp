#include "ShareManager.hpp"
#include "../../Download/DownloadManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;

CShareManager::CShareManager()
: m_CapturePath(""){}

CShareManager::~CShareManager(){}

bool CShareManager::init(){ return true; }

CShareManager* CShareManager::Instance()
{
    static CShareManager instance;
    return &instance;
}

void CShareManager::Initialize()
{
    sdkbox::PluginShare::setListener(this);
    sdkbox::PluginShare::init();
}

void CShareManager::Share(bool dialog, sdkbox::SocialPlatform platform)
{
    auto path = CShareManager::Instance()->getCapturePath();
//    if (!path.empty() && FileUtils::getInstance()->isFileExist(path))
//    {
        sdkbox::SocialShareInfo info;
        info.text  = "#Laviah";
        info.title = "Laviah";
        info.image = path;
        info.link  = CDownloadManager::Instance()->getAppUrl();
        info.platform = platform;
        info.showDialog = dialog;
        sdkbox::PluginShare::share(info);
//    }
//    else
//        MessageBox("capture first", "Failed");
}

void CShareManager::ShareNative()
{
    auto path = CShareManager::Instance()->getCapturePath();
//    if (!path.empty() && FileUtils::getInstance()->isFileExist(path))
//    {
        sdkbox::SocialShareInfo info;
        info.text  = "#Laviah";
        info.title = "Laviah";
        info.image = path;
        info.link  = CDownloadManager::Instance()->getAppUrl();;
        
        sdkbox::PluginShare::nativeShare(info);
//    }
//    else
//        MessageBox("capture first", "Failed");
}

bool CShareManager::SaveNodeToFile(cocos2d::Node* node)
{
    auto image   = utils::captureNode(node);
    auto path    = FileUtils::getInstance()->getWritablePath() + "remoteImage/" + "ShareCapture.png";
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    path = "/mnt/sdcard/ShareCapture.png";
#endif
    auto succeed = image->saveToFile(path);
    if(!succeed){
        CCLOG("Capture failed. Can not save to file.");
        return false;
    }
    
    CShareManager::Instance()->setCapturePath(path);
    return true;
}

void CShareManager::onShareState(const sdkbox::SocialShareResponse& response)
{
    switch (response.state) {
        case sdkbox::SocialShareState::SocialShareStateNone: {
            CCLOG("SharePlugin::onShareState none");
            break;
        }
        case sdkbox::SocialShareState::SocialShareStateUnkonw: {
            CCLOG("SharePlugin::onShareState unkonw");
            break;
        }
        case sdkbox::SocialShareState::SocialShareStateBegin: {
            CCLOG("SharePlugin::onShareState begin");
            break;
        }
        case sdkbox::SocialShareState::SocialShareStateSuccess: {
            CCLOG("SharePlugin::onShareState success");
            break;
        }
        case sdkbox::SocialShareState::SocialShareStateFail: {
            CCLOG("SharePlugin::onShareState fail, error:%s", response.error.c_str());
            break;
        }
        case sdkbox::SocialShareState::SocialShareStateCancelled: {
            CCLOG("SharePlugin::onShareState cancelled");
            break;
        }
        case sdkbox::SocialShareStateSelectShow: {
            CCLOG("SharePlugin::onShareState show pancel %d", response.platform);
            break;
        }
        case sdkbox::SocialShareStateSelectCancelled: {
            CCLOG("SharePlugin::onShareState show pancel cancelled %d", response.platform);
            break;
        }
        case sdkbox::SocialShareStateSelected: {
            CCLOG("SharePlugin::onShareState show pancel selected %d", response.platform);
            break;
        }
        default: {
            CCLOG("SharePlugin::onShareState");
            break;
        }
    }
}

