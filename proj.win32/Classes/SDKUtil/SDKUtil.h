#pragma once
#include "../Common/HSHUtility.h"

class CSDKUtil_AOS;
class CSDKUtil_IOS;
class CSDKUtil_WIN;
class CSDKUtil{
public:
	static CSDKUtil* Instance();

	void GoogleLogin();

	void GoogleCloudSave(std::string key, std::string value);

	void GoogleCloudLoad(std::string key);

	void ShowRewardUnityAds(const std::function<void(void)> &func);

	void ShowNormalUnityAds(const std::function<void(void)> &func);

	void Toast(std::string content);

	void IsNetworkConnect();

	void CallUnityAdsSavedFunction();

private:
	void saveFuncUnityAdsCallBack(const std::function<void(void)> &func);

	CSDKUtil();
	~CSDKUtil();

	CC_SYNTHESIZE(bool, m_IsRewardUnityAdsReady, IsRewardUnityAdsReady);
	CC_SYNTHESIZE(bool, m_IsNormalUnityAdsReady, IsNormalUnityAdsReady);
	CC_SYNTHESIZE(bool, m_IsNetworkConnect, IsNetworkConnect);

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