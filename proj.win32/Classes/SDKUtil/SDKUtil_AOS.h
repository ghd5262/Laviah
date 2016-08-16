#pragma once

#include "../Common/HSHUtility.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

class CSDKUtil_AOS{
public:
    CSDKUtil_AOS();
    ~CSDKUtil_AOS();
    
    // 구글 로그인
    void GoogleLogin();
    
	// 구글 클라우드 저장 Key / Value
	void GoogleCloudSave(std::string key, std::string value);

	// 구글 클라우드 로드 Key
	void GoogleCloudLoad(std::string key);
    
    // 유니티 애드 보상형 광고
	void ShowRewardUnityAds();
    
    // 유니티 애드 일반 광고
	void ShowNormalUnityAds();
    
    // 토스트창
    void Toast(std::string content);

private:
    void JavaCallMethod(const char* methodName);
    
private:
    std::string m_JavaUrlString;
};

#endif