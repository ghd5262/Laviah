#include "SDKUtil_IOS.h"
#include "cocos2d.h"

USING_NS_CC;

CSDKUtil_IOS::CSDKUtil_IOS()
{
    
}

CSDKUtil_IOS::~CSDKUtil_IOS()
{
    
}

// 구글 클라우드 저장 Key / Value(json)
void CSDKUtil_IOS::GoogleCloudSend(std::string key, std::string value)
{
    std::string methodName = "GoogleCloudSend";
    CCLOG("IOS Method %s", methodName.c_str());
}

// 유니티 애드 보상형 광고
void CSDKUtil_IOS::ShowUnityAdIncentivized()
{
    std::string methodName = "ShowUnityAdIncentivized";
    CCLOG("IOS Method %s", methodName.c_str());
}

// 유니티 애드 일반 광고
void CSDKUtil_IOS::ShowUnityAdInterstitial()
{
    std::string methodName = "ShowUnityAdInterstitial";
    CCLOG("IOS Method %s", methodName.c_str());
}

// 토스트창
void CSDKUtil_IOS::Toast(std::string content)
{
    std::string methodName = "Toast";
    CCLOG("IOS Method %s", methodName.c_str());
}
