#include "UnityAdsAPIs.h"

CUnityAdsAPIs* CUnityAdsAPIs::Instance()
{
    static CUnityAdsAPIs instance;
    return &instance;
}

CUnityAdsAPIs::CUnityAdsAPIs()
: m_UnityAdsSavedFunc(nullptr)
, m_NetworkConnectSavedFunc(nullptr){}

CUnityAdsAPIs::~CUnityAdsAPIs(){}

void CUnityAdsAPIs::UnityAdsInitialize(bool testMode/* = false*/)
{
    std::string gameID = "1474731";
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    gameID = "1474732";
#endif
    UnityAdsInit(gameID.c_str(), testMode);
}

void CUnityAdsAPIs::ShowUnityAds(const std::function<void(void)> &func, bool isRewarded/* = true*/)
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

void CUnityAdsAPIs::Toast(std::string content)
{
//    m_SDKUtil->Toast(content);
}

void CUnityAdsAPIs::IsNetworkConnect()
{
//    m_SDKUtil->IsNetworkConnect();
}

void CUnityAdsAPIs::CallUnityAdsSavedFunction()
{
    if (m_UnityAdsSavedFunc != nullptr)
    {
        m_UnityAdsSavedFunc();
        m_UnityAdsSavedFunc = nullptr;
    }
    else
        CCASSERT(false, "CallUnityAdsSavedFunction : No function was saved.");
}

void CUnityAdsAPIs::CallNetworkConnectSavedFunction()
{
    if (m_NetworkConnectSavedFunc != nullptr)
    {
        m_NetworkConnectSavedFunc();
        m_NetworkConnectSavedFunc = nullptr;
    }
    else
        CCASSERT(false, "CallNetworkConnectSavedFunction : No function was saved.");
}
