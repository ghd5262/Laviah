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
	, m_isNormalUnityAdsReady(false)
	, m_isRewardUnityAdsReady(false)
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

// ���� �α���
void CSDKUtil::GoogleLogin(){
	m_SDKUtil->GoogleLogin();
}

// ���� Ŭ���� ���� Key / Value(json)
void CSDKUtil::GoogleCloudSave(std::string key, std::string value)
{
	m_SDKUtil->GoogleCloudSave(key, value);
}

// ���� Ŭ���� �ε� Key
void CSDKUtil::GoogleCloudLoad(std::string key)
{
	m_SDKUtil->GoogleCloudLoad(key);
}

void CSDKUtil::saveFunctionForCallBack(const std::function<void(void)> &func)
{
	if (func == nullptr)
		CCASSERT(false, "Saving Function is null");
	m_SavedFunc = func;
}

// ����Ƽ �ֵ� ������ ����
void CSDKUtil::ShowRewardUnityAds(const std::function<void(void)> &func)
{
	if (m_isRewardUnityAdsReady){
		m_isRewardUnityAdsReady = false;
		saveFunctionForCallBack(func);
		m_SDKUtil->ShowRewardUnityAds();
	}
}

// ����Ƽ �ֵ� �Ϲ� ����
void CSDKUtil::ShowNormalUnityAds(const std::function<void(void)> &func)
{
	if (m_isNormalUnityAdsReady){
		m_isNormalUnityAdsReady = false;
		saveFunctionForCallBack(func);
		m_SDKUtil->ShowNormalUnityAds();
	}
}
// �佺Ʈâ
void CSDKUtil::Toast(std::string content)
{
	m_SDKUtil->Toast(content);
}

// ����� �Լ� ȣ��
void CSDKUtil::CallSavedFuncion()
{
	if (m_SavedFunc != nullptr)
	{
		m_SavedFunc();
		m_SavedFunc = nullptr;	// �ѹ� ȣ���ϸ� ���� 
	}
	else
		CCASSERT(false, "No Function was saved.");
}

