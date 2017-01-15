#include "SDKUtil_WIN.h"
#include "../GoogleCloud/GoogleCloudManager.h"
#include <time.h>

using namespace cocos2d;

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
	auto unixTime = time(NULL);
	CGoogleCloudManager::Instance()->setLastSavedTime(unixTime);
}

// 구글 클라우드 로드 Key
void CSDKUtil_WIN::GoogleCloudLoad(std::string key)
{ 
	CCLOG("SDKUtil_WIN %s Test", __FUNCTION__); 
};

// Set user data to auto save list
void CSDKUtil_WIN::AddDataToAutoSaveList(std::string key, std::string value)
{
	CCLOG("SDKUtil_WIN %s Test", __FUNCTION__);
}

// auto save to google cloud.
void CSDKUtil_WIN::AutoSave()
{
	CCLOG("SDKUtil_WIN %s Test", __FUNCTION__);
	auto unixTime = time(NULL);
	CGoogleCloudManager::Instance()->setLastSavedTime(unixTime);
}

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
	CSDKUtil::Instance()->setIsNetworkConnect(true);
	CSDKUtil::Instance()->CallNetworkConnectSavedFunction();
}