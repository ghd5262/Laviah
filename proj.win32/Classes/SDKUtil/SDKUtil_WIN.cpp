#include "SDKUtil_WIN.h"

CSDKUtil_WIN::CSDKUtil_WIN(){}

CSDKUtil_WIN::~CSDKUtil_WIN(){}

// 구글 로그인
void CSDKUtil_WIN::GoogleLogin()
{
	CCLOG("SDKUtil_WIN %s Test", __FUNCTION__);
	__NotificationCenter::getInstance()->postNotification(NOTICE::LOGIN_RESULT, NULL);
};

// 구글 클라우드 저장 Key / Value
void CSDKUtil_WIN::GoogleCloudSave(std::string key, std::string value)
{ 
	CCLOG("SDKUtil_WIN %s Test", __FUNCTION__); 
}

// 구글 클라우드 로드 Key
void CSDKUtil_WIN::GoogleCloudLoad(std::string key)
{ 
	CCLOG("SDKUtil_WIN %s Test", __FUNCTION__); 
};


// 유니티 애드 보상형 광고
void CSDKUtil_WIN::ShowRewardUnityAds()
{
	CCLOG("SDKUtil_WIN %s Test", __FUNCTION__);

	Director::getInstance()->getScheduler()->schedule([](float delta){
		CSDKUtil::Instance()->CallUnityAdsSavedFunction();
	}, Director::getInstance(), 0.f, 0, 0.f, false, "CallUnityAdsSavedFunction");

	CSDKUtil::Instance()->setIsRewardUnityAdsReady(true);
}

// 유니티 애드 일반 광고
void CSDKUtil_WIN::ShowNormalUnityAds()
{
	CCLOG("SDKUtil_WIN %s Test", __FUNCTION__);

	Director::getInstance()->getScheduler()->schedule([](float delta){
		CSDKUtil::Instance()->CallUnityAdsSavedFunction();
	}, Director::getInstance(), 0.f, 0, 0.f, false, "CallUnityAdsSavedFunction");

	CSDKUtil::Instance()->setIsNormalUnityAdsReady(true);
}

// 토스트창
void CSDKUtil_WIN::Toast(std::string content)
{ 
	CCLOG("SDKUtil_WIN %s Test", __FUNCTION__);
}

void CSDKUtil_WIN::IsNetworkConnect()
{
	__NotificationCenter::getInstance()->postNotification(NOTICE::NETWORK_RESULT, NULL);
	CSDKUtil::Instance()->setIsNetworkConnect(true);
}