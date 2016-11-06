#include "SDKUtil_WIN.h"

CSDKUtil_WIN::CSDKUtil_WIN(){}

CSDKUtil_WIN::~CSDKUtil_WIN(){}

// ���� �α���
void CSDKUtil_WIN::GoogleLogin()
{
	CCLOG("SDKUtil_WIN %s Test", __FUNCTION__);
	__NotificationCenter::getInstance()->postNotification(NOTICE::LOGIN_RESULT, NULL);
};

// ���� Ŭ���� ���� Key / Value
void CSDKUtil_WIN::GoogleCloudSave(std::string key, std::string value)
{ 
	CCLOG("SDKUtil_WIN %s Test", __FUNCTION__); 
}

// ���� Ŭ���� �ε� Key
void CSDKUtil_WIN::GoogleCloudLoad(std::string key)
{ 
	CCLOG("SDKUtil_WIN %s Test", __FUNCTION__); 
};


// ����Ƽ �ֵ� ������ ����
void CSDKUtil_WIN::ShowRewardUnityAds()
{
	CCLOG("SDKUtil_WIN %s Test", __FUNCTION__);

	Director::getInstance()->getScheduler()->schedule([](float delta){
		CSDKUtil::Instance()->CallUnityAdsSavedFunction();
	}, Director::getInstance(), 0.f, 0, 0.f, false, "CallUnityAdsSavedFunction");

	CSDKUtil::Instance()->setIsRewardUnityAdsReady(true);
}

// ����Ƽ �ֵ� �Ϲ� ����
void CSDKUtil_WIN::ShowNormalUnityAds()
{
	CCLOG("SDKUtil_WIN %s Test", __FUNCTION__);

	Director::getInstance()->getScheduler()->schedule([](float delta){
		CSDKUtil::Instance()->CallUnityAdsSavedFunction();
	}, Director::getInstance(), 0.f, 0, 0.f, false, "CallUnityAdsSavedFunction");

	CSDKUtil::Instance()->setIsNormalUnityAdsReady(true);
}

// �佺Ʈâ
void CSDKUtil_WIN::Toast(std::string content)
{ 
	CCLOG("SDKUtil_WIN %s Test", __FUNCTION__);
}

void CSDKUtil_WIN::IsNetworkConnect()
{
	__NotificationCenter::getInstance()->postNotification(NOTICE::NETWORK_RESULT, NULL);
	CSDKUtil::Instance()->setIsNetworkConnect(true);
}