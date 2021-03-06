#include "UnityAdsManager.hpp"
#include "cocos2d.h"
#include "../../Scene/GameScene.h"
#include "../../MyUI/Popup.h"

using namespace cocos2d;

CUnityAdsManager* CUnityAdsManager::Instance()
{
    static CUnityAdsManager instance;
    return &instance;
}

CUnityAdsManager::CUnityAdsManager()
: m_UnityAdsSucceedCallback(nullptr)
, m_UnityAdsFailedCallback(nullptr)
, m_UnityAdsSkippedCallback(nullptr){}

CUnityAdsManager::~CUnityAdsManager(){}

void CUnityAdsManager::Initialize(bool testMode/* = false*/)
{
    std::string gameID = "1474731";
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    gameID = "1474732";
#endif
    UnityAdsInit(gameID.c_str(), testMode);
}

void CUnityAdsManager::ShowUnityAds(const std::function<void(void)> &func, bool isRewarded/* = true*/)
{
    std::string placementID     = "rewardedVideo";
    if(!isRewarded) placementID = "video";
    
    if(UnityAdsIsReady(placementID.c_str())) {
        m_UnityAdsSucceedCallback = func;
        UnityAdsShow(placementID.c_str());
    } else {
        CCLOG("%s : yet cannot show", placementID.c_str());
        CGameScene::getGameScene()->CreateAlertPopup()
        ->setPositiveButton([=](Node* sender){
            this->callListener(m_UnityAdsFailedCallback);
        }, TRANSLATE("BUTTON_OK"))
        ->setMessage(TRANSLATE("NETWORK_CONNECT_CHECK"))
        ->show(CGameScene::getGameScene(), ZORDER::POPUP);
    }
}

void CUnityAdsManager::CallUnityAdsSavedFunction(int state)
{
    switch (state) {
        case 0: // error
            this->callListener(m_UnityAdsFailedCallback);

            break;
        case 1: // skip
            this->callListener(m_UnityAdsSkippedCallback);

            break;
        case 2: // complete
            this->callListener(m_UnityAdsSucceedCallback);

            break;
    }
}

void CUnityAdsManager::callListener(std::function<void(void)>& listener)
{
    if (listener != nullptr)
    {
        listener();
        listener = nullptr;
    }
}
