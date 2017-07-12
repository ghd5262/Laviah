#include "UnityAdsManager.hpp"

CUnityAdsManager* CUnityAdsManager::Instance()
{
    static CUnityAdsManager instance;
    return &instance;
}

CUnityAdsManager::CUnityAdsManager()
: m_UnityAdsSavedFunc(nullptr)
, m_NetworkConnectSavedFunc(nullptr){}

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
        m_UnityAdsSavedFunc = func;
        UnityAdsShow(placementID.c_str());
    } else {
        CCLOG("%s : yet cannot show", placementID.c_str());
    }
}

void CUnityAdsManager::Toast(std::string content)
{
//    m_SDKUtil->Toast(content);
}

void CUnityAdsManager::IsNetworkConnect()
{
//    m_SDKUtil->IsNetworkConnect();
}

void CUnityAdsManager::CallUnityAdsSavedFunction()
{
    if (m_UnityAdsSavedFunc != nullptr)
    {
        m_UnityAdsSavedFunc();
        m_UnityAdsSavedFunc = nullptr;
    }
    else
        CCASSERT(false, "CallUnityAdsSavedFunction : No function was saved.");
}

void CUnityAdsManager::CallNetworkConnectSavedFunction()
{
    if (m_NetworkConnectSavedFunc != nullptr)
    {
        m_NetworkConnectSavedFunc();
        m_NetworkConnectSavedFunc = nullptr;
    }
    else
        CCASSERT(false, "CallNetworkConnectSavedFunction : No function was saved.");
}
