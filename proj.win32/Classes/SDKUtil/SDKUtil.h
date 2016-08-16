#pragma once
#include "../Common/HSHUtility.h"

class CSDKUtil_AOS;
class CSDKUtil_IOS;
class CSDKUtil_WIN;
class CSDKUtil{
public:
    static CSDKUtil* Instance();
    
    // 구글 로그인
    void GoogleLogin();
    
    // 구글 클라우드 저장 Key / Value
    void GoogleCloudSave(std::string key, std::string value);

	// 구글 클라우드 로드 Key
	void GoogleCloudLoad(std::string key);
    
    // 유니티 애드 보상형 광고
	void ShowRewardUnityAds(const std::function<void(void)> &func);

    // 유니티 애드 일반 광고
	void ShowNormalUnityAds(const std::function<void(void)> &func);
    
    // 토스트창
    void Toast(std::string content);

	// 저장된 함수 호출
	void CallSavedFuncion();
    
private:
	void saveFunctionForCallBack(const std::function<void(void)> &func);

    CSDKUtil();
    ~CSDKUtil();
    
	CC_SYNTHESIZE(bool, m_isRewardUnityAdsReady, IsRewardUnityAdsReady);
	CC_SYNTHESIZE(bool, m_isNormalUnityAdsReady, IsNormalUnityAdsReady);

private:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CSDKUtil_IOS* m_SDKUtil;
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CSDKUtil_AOS* m_SDKUtil;
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	CSDKUtil_WIN* m_SDKUtil;
#endif

	std::function<void(void)> m_SavedFunc;
};