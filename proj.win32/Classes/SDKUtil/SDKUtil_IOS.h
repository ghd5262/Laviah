#pragma once

#include "../Common/HSHUtility.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

class CSDKUtil_IOS{
public:
	CSDKUtil_IOS();
	~CSDKUtil_IOS();

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
    
    // 네트워크 연결 여부
    void IsNetworkConnect();

};

#endif