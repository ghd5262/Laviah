#pragma once

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "SDKUtil.h"
#include "../Common/HSHUtility.h"
#include "../Scene/GameScene.h"
#include "../DataManager/UserDataManager.h"
#include "../Common/NoticeDefine.h"

class CSDKUtil_WIN{
public:
	CSDKUtil_WIN();
	~CSDKUtil_WIN();

	void GoogleLogin();

	void GoogleCloudSave(std::string key, std::string value); 

	void GoogleCloudLoad(std::string key); 

	void AddDataToAutoSaveList(std::string key, std::string value);

	void AutoSave();

	void ShowRewardUnityAds(); 

	void ShowNormalUnityAds(); 

	void Toast(std::string content);

	void IsNetworkConnect(); 
};

#endif