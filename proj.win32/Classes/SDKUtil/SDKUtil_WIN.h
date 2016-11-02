#pragma once

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "SDKUtil.h"
#include "../Common/HSHUtility.h"
#include "../Scene/GameScene.h"
#include "../DataManager/UserDataManager.h"
#include "../Common/NoticeDefine.h"

class CSDKUtil_WIN{
public:
	CSDKUtil_WIN(){};
	~CSDKUtil_WIN(){};

	// ���� �α���
	void GoogleLogin(){
		CCLOG("SDKUtil_WIN %s Test", __FUNCTION__); 
		__NotificationCenter::getInstance()->postNotification(NOTICE::LOGIN_RESULT, NULL);
	};

	// ���� Ŭ���� ���� Key / Value
	void GoogleCloudSave(std::string key, std::string value){ CCLOG("SDKUtil_WIN %s Test", __FUNCTION__); }

	// ���� Ŭ���� �ε� Key
	void GoogleCloudLoad(std::string key){ CCLOG("SDKUtil_WIN %s Test", __FUNCTION__); };

	// ����Ƽ �ֵ� ������ ����
	void ShowRewardUnityAds(){
		CCLOG("SDKUtil_WIN %s Test", __FUNCTION__);

		Director::getInstance()->getScheduler()->schedule([](float delta){
			CSDKUtil::Instance()->CallUnityAdsSavedFunction();
		}, Director::getInstance(), 0.f, 0, 0.f, false, "CallUnityAdsSavedFunction");

		CSDKUtil::Instance()->setIsRewardUnityAdsReady(true);
	}

	// ����Ƽ �ֵ� �Ϲ� ����
	void ShowNormalUnityAds(){
		CCLOG("SDKUtil_WIN %s Test", __FUNCTION__);

		Director::getInstance()->getScheduler()->schedule([](float delta){
			CSDKUtil::Instance()->CallUnityAdsSavedFunction();
		}, Director::getInstance(), 0.f, 0, 0.f, false, "CallUnityAdsSavedFunction");

		CSDKUtil::Instance()->setIsNormalUnityAdsReady(true);
	}

	// �佺Ʈâ
	void Toast(std::string content){ CCLOG("SDKUtil_WIN %s Test", __FUNCTION__); }
    
	void IsNetworkConnect(){ 
		__NotificationCenter::getInstance()->postNotification(NOTICE::NETWORK_RESULT, NULL);
		CSDKUtil::Instance()->setIsNetworkConnect(true);
	};
};

#endif