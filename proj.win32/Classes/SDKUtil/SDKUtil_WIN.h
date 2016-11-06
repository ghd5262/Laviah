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

	// 구글 로그인
	void GoogleLogin();

	// 구글 클라우드 저장 Key / Value
	void GoogleCloudSave(std::string key, std::string value); 

	// 구글 클라우드 로드 Key
	void GoogleCloudLoad(std::string key); 

	// 유니티 애드 보상형 광고
	void ShowRewardUnityAds(); 

	// 유니티 애드 일반 광고
	void ShowNormalUnityAds(); 

	// 토스트창
	void Toast(std::string content);

	void IsNetworkConnect(); 
};

#endif