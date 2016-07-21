#pragma once
#include "../Common/HSHUtility.h"

class CSDKUtil_AOS;
class CSDKUtil_IOS;
class CSDKUtil{
public:
    static CSDKUtil* Instance();
    
    // 구글 클라우드 저장 Key / Value(json)
    void GoogleCloudSend(std::string key, std::string value);
    
    // 유니티 애드 보상형 광고
    void ShowUnityAdIncentivized();

    // 유니티 애드 일반 광고
    void ShowUnityAdInterstitial();
    
    // 토스트창
    void Toast(std::string content);
    
private:
    CSDKUtil();
    ~CSDKUtil();
    
private:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CSDKUtil_IOS* m_SDKUtil;
#else
    CSDKUtil_AOS* m_SDKUtil;
#endif
};