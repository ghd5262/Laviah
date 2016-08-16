#include "SDKUtil_IOS.h"
#include "cocos2d.h"
#include "SDKUtil.h"
#include "../DataManager/UserDataManager.h"

#if ( CC_TARGET_PLATFORM == CC_PLATFORM_IOS )

USING_NS_CC;

CSDKUtil_IOS::CSDKUtil_IOS()
{
    CUserDataManager::Instance()->GoogleLoginResult();
}

CSDKUtil_IOS::~CSDKUtil_IOS()
{
    
}

// 구글 로그인
void CSDKUtil_IOS::GoogleLogin()
{
    std::string methodName = "GoogleLogin";
    CCLOG("IOS Method %s", methodName.c_str());
}

// 구글 클라우드 저장 Key / Value
void CSDKUtil_IOS::GoogleCloudSave(std::string key, std::string value)
{
    std::string methodName = "GoogleCloudSave";
    CCLOG("IOS Method %s", methodName.c_str());
}

// 구글 클라우드 로드 Key
void CSDKUtil_IOS::GoogleCloudLoad(std::string key)
{
    std::string methodName = "GoogleCloudLoad";
    CCLOG("IOS Method %s", methodName.c_str());
}

// 유니티 애드 보상형 광고
void CSDKUtil_IOS::ShowRewardUnityAds()
{
    CSDKUtil::Instance()->setIsRewardUnityAdsReady(true);
    
    std::string methodName = "ShowUnityAdIncentivized";
    CCLOG("IOS Method %s", methodName.c_str());
}

// 유니티 애드 일반 광고
void CSDKUtil_IOS::ShowNormalUnityAds()
{
    CSDKUtil::Instance()->setIsNormalUnityAdsReady(true);
    
    std::string methodName = "ShowUnityAdInterstitial";
    CCLOG("IOS Method %s", methodName.c_str());
}

// 토스트창
void CSDKUtil_IOS::Toast(std::string content)
{
    std::string methodName = "Toast";
    CCLOG("IOS Method %s", methodName.c_str());
}

#endif
