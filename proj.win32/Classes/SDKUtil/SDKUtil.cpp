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

// ±∏±€ ∑Œ±◊¿Œ
void CSDKUtil::GoogleLogin(){
	m_SDKUtil->GoogleLogin();
}

// ±∏±€ ≈¨∂ÛøÏµÂ ¿˙¿Â Key / Value(json)
void CSDKUtil::GoogleCloudSave(std::string key, std::string value)
{
	m_SDKUtil->GoogleCloudSave(key, value);
}

// ±∏±€ ≈¨∂ÛøÏµÂ ∑ŒµÂ Key
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

// ¿Ø¥œ∆º æ÷µÂ ∫∏ªÛ«¸ ±§∞Ì
void CSDKUtil::ShowRewardUnityAds(const std::function<void(void)> &func)
{
	if (m_IsRewardUnityAdsReady){
		m_IsRewardUnityAdsReady = false;
		saveFuncUnityAdsCallBack(func);
		m_SDKUtil->ShowRewardUnityAds();
	}
}

// ¿Ø¥œ∆º æ÷µÂ ¿œπ› ±§∞Ì
void CSDKUtil::ShowNormalUnityAds(const std::function<void(void)> &func)
{
	if (m_IsNormalUnityAdsReady){
		m_IsNormalUnityAdsReady = false;
		saveFuncUnityAdsCallBack(func);
		m_SDKUtil->ShowNormalUnityAds();
	}
}
// ≈‰Ω∫∆Æ√¢
void CSDKUtil::Toast(std::string content)
{
	m_SDKUtil->Toast(content);
}

// 네트워크 연결 여부
void CSDKUtil::IsNetworkConnect()
{
    m_SDKUtil->IsNetworkConnect();
}

// ¿˙¿Âµ» «‘ºˆ »£√‚
void CSDKUtil::CallUnityAdsSavedFunction()
{
	if (m_SavedFunc != nullptr)
	{
		m_SavedFunc();
		m_SavedFunc = nullptr;	// «—π¯ »£√‚«œ∏È ªË¡¶ 
	}
	else
		CCASSERT(false, "No Function was saved.");
}

