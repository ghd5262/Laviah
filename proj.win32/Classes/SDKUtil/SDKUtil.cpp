#include "SDKUtil.h"
#include "SDKUtil_AOS.h"
#include "SDKUtil_IOS.h"
#include "cocos2d.h"

USING_NS_CC;

CSDKUtil* CSDKUtil::Instance()
{
    static CSDKUtil instance;
    return &instance;
}

CSDKUtil::CSDKUtil()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CCLOG("SDKUtil_IOS");
    m_SDKUtil = new CSDKUtil_IOS();
#else
    CCLOG("SDKUtil_AOS");
    m_SDKUtil = new CSDKUtil_AOS();
#endif
}

CSDKUtil::~CSDKUtil()
{
    if(m_SDKUtil != nullptr)
        delete m_SDKUtil;
}

// 구글 클라우드 저장 Key / Value(json)
void CSDKUtil::GoogleCloudSend(std::string key, std::string value)
{
    m_SDKUtil->GoogleCloudSend(key, value);
}

// 유니티 애드 보상형 광고
void CSDKUtil::ShowUnityAdIncentivized()
{
    m_SDKUtil->ShowUnityAdIncentivized();
}

// 유니티 애드 일반 광고
void CSDKUtil::ShowUnityAdInterstitial()
{
    m_SDKUtil->ShowUnityAdInterstitial();
}

// 토스트창
void CSDKUtil::Toast(std::string content)
{
    m_SDKUtil->Toast(content);
}



