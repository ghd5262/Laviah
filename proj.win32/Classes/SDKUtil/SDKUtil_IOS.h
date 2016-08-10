#pragma once

#include "../Common/HSHUtility.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

class CSDKUtil_IOS{
public:
	CSDKUtil_IOS();
	~CSDKUtil_IOS();

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
};

#endif