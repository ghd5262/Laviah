#include "SDKUtil.h"
#include "SDKUtil_AOS.h"
#include "SDKUtil_IOS.h"
#include "SDKUtil_WIN.h"
#include "cocos2d.h"
USING_NS_CC;

CSDKUtil* CSDKUtil::Instance()
{
	static CSDKUtil instance;
	return &instance;
}

CSDKUtil::CSDKUtil()
	: m_SDKUtil(nullptr)
	, m_SavedFunc(nullptr)
	, m_IsNormalUnityAdsReady(false)
	, m_IsRewardUnityAdsReady(false)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	CCLOG("SDKUtil_IOS");
	m_SDKUtil = new CSDKUtil_IOS();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	CCLOG("SDKUtil_AOS");
	m_SDKUtil = new CSDKUtil_AOS();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	CCLOG("SDKUtil_WIN32");
	m_SDKUtil = new CSDKUtil_WIN();
#endif
}

CSDKUtil::~CSDKUtil()
{
	if (m_SDKUtil != nullptr)
		delete m_SDKUtil;
}

void CSDKUtil::GoogleLogin(){
	m_SDKUtil->GoogleLogin();
}

void CSDKUtil::GoogleCloudSave(std::string key, std::string value)
{
	m_SDKUtil->GoogleCloudSave(key, value);
}

void CSDKUtil::GoogleCloudLoad(std::string key)
{
	m_SDKUtil->GoogleCloudLoad(key);
}

void CSDKUtil::saveFuncUnityAdsCallBack(const std::function<void(void)> &func)
{
	if (func == nullptr)
		CCASSERT(false, "Saving Function is null");
	m_SavedFunc = func;
}

void CSDKUtil::ShowRewardUnityAds(const std::function<void(void)> &func)
{
	if (m_IsRewardUnityAdsReady){
		m_IsRewardUnityAdsReady = false;
		saveFuncUnityAdsCallBack(func);
		m_SDKUtil->ShowRewardUnityAds();
	}
}

void CSDKUtil::ShowNormalUnityAds(const std::function<void(void)> &func)
{
	if (m_IsNormalUnityAdsReady){
		m_IsNormalUnityAdsReady = false;
		saveFuncUnityAdsCallBack(func);
		m_SDKUtil->ShowNormalUnityAds();
	}
}

void CSDKUtil::Toast(std::string content)
{
	m_SDKUtil->Toast(content);
}

void CSDKUtil::IsNetworkConnect()
{
	m_SDKUtil->IsNetworkConnect();
}

void CSDKUtil::CallUnityAdsSavedFunction()
{
	if (m_SavedFunc != nullptr)
	{
		m_SavedFunc();
		m_SavedFunc = nullptr;
	}
	else
		CCASSERT(false, "No Function was saved.");
}

