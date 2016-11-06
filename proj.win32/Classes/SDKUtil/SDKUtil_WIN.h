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

	// ���� �α���
	void GoogleLogin();

	// ���� Ŭ���� ���� Key / Value
	void GoogleCloudSave(std::string key, std::string value); 

	// ���� Ŭ���� �ε� Key
	void GoogleCloudLoad(std::string key); 

	// ����Ƽ �ֵ� ������ ����
	void ShowRewardUnityAds(); 

	// ����Ƽ �ֵ� �Ϲ� ����
	void ShowNormalUnityAds(); 

	// �佺Ʈâ
	void Toast(std::string content);

	void IsNetworkConnect(); 
};

#endif