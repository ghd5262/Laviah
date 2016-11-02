#pragma once

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "SDKUtil.h"
#include "../Common/HSHUtility.h"
#include "../Scene/GameScene.h"
#include "../DataManager/UserDataManager.h"

class CSDKUtil_WIN{
public:
	CSDKUtil_WIN(){};
	~CSDKUtil_WIN(){};

	// 구글 로그인
	void GoogleLogin(){ CCLOG("SDKUtil_WIN %s Test", __FUNCTION__); CUserDataManager::Instance()->GoogleLoginResult(); };

	// 구글 클라우드 저장 Key / Value
	void GoogleCloudSave(std::string key, std::string value){ CCLOG("SDKUtil_WIN %s Test", __FUNCTION__); }

	// 구글 클라우드 로드 Key
	void GoogleCloudLoad(std::string key){ CCLOG("SDKUtil_WIN %s Test", __FUNCTION__); };

	// 유니티 애드 보상형 광고
	void ShowRewardUnityAds(){
		CCLOG("SDKUtil_WIN %s Test", __FUNCTION__);

		Director::getInstance()->getScheduler()->schedule([](float delta){
			CSDKUtil::Instance()->CallUnityAdsSavedFunction();
		}, Director::getInstance(), 0.f, 0, 0.f, false, "CallUnityAdsSavedFunction");

		CSDKUtil::Instance()->setIsRewardUnityAdsReady(true);
	}

	// 유니티 애드 일반 광고
	void ShowNormalUnityAds(){
		CCLOG("SDKUtil_WIN %s Test", __FUNCTION__);

		Director::getInstance()->getScheduler()->schedule([](float delta){
			CSDKUtil::Instance()->CallUnityAdsSavedFunction();
		}, Director::getInstance(), 0.f, 0, 0.f, false, "CallUnityAdsSavedFunction");

		CSDKUtil::Instance()->setIsNormalUnityAdsReady(true);
	}

	// 토스트창
	void Toast(std::string content){ CCLOG("SDKUtil_WIN %s Test", __FUNCTION__); }
    
    void IsNetworkConnect();
};

#endif