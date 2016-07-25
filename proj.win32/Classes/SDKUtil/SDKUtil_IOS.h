#pragma once

#include "../Common/HSHUtility.h"
class CSDKUtil_IOS{
public:
    CSDKUtil_IOS();
    ~CSDKUtil_IOS();
    
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
};