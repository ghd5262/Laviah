#pragma once

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "../Common/HSHUtility.h"
#include "../Scene/GameScene.h"

class CSDKUtil_WIN{
public:
	CSDKUtil_WIN(){ 
		CSDKUtil::Instance()->setIsRewardUnityAdsReady(true); 
		CSDKUtil::Instance()->setIsNormalUnityAdsReady(true); 
	};
	~CSDKUtil_WIN(){};

	// ���� Ŭ���� ���� Key / Value
	void GoogleCloudSave(std::string key, std::string value){ CCLOG("SDKUtil_WIN %s Test", __FUNCTION__); }

	// ���� Ŭ���� �ε� Key
	void GoogleCloudLoad(std::string key){ CCLOG("SDKUtil_WIN %s Test", __FUNCTION__); };

	// ����Ƽ �ֵ� ������ ����
	void ShowRewardUnityAds(){
		CCLOG("SDKUtil_WIN %s Test", __FUNCTION__);

		Director::getInstance()->getScheduler()->schedule([](float delta){
			CSDKUtil::Instance()->CallSavedFuncion();
		}, Director::getInstance(), 0.f, 0, 0.f, false, "CallSavedFuncion");

		CSDKUtil::Instance()->setIsRewardUnityAdsReady(true);
	}

	// ����Ƽ �ֵ� �Ϲ� ����
	void ShowNormalUnityAds(){ 
		CCLOG("SDKUtil_WIN %s Test", __FUNCTION__); 

		Director::getInstance()->getScheduler()->schedule([](float delta){
			CSDKUtil::Instance()->CallSavedFuncion();
		}, Director::getInstance(), 0.f, 0, 0.f, false, "CallSavedFuncion");

		CSDKUtil::Instance()->setIsNormalUnityAdsReady(true); 
	}

	// �佺Ʈâ
	void Toast(std::string content){ CCLOG("SDKUtil_WIN %s Test", __FUNCTION__); }
};

#endif