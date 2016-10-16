#pragma once
#include "../Common/HSHUtility.h"

class CSDKUtil_AOS;
class CSDKUtil_IOS;
class CSDKUtil_WIN;
class CSDKUtil{
public:
	static CSDKUtil* Instance();

	// ���� �α���
	void GoogleLogin();

	// ���� Ŭ���� ���� Key / Value
	void GoogleCloudSave(std::string key, std::string value);

	// ���� Ŭ���� �ε� Key
	void GoogleCloudLoad(std::string key);

	// ����Ƽ �ֵ� ������ ����
	void ShowRewardUnityAds(const std::function<void(void)> &func);

	// ����Ƽ �ֵ� �Ϲ� ����
	void ShowNormalUnityAds(const std::function<void(void)> &func);

	// �佺Ʈâ
	void Toast(std::string content);

	// ����� �Լ� ȣ��
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