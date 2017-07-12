#pragma once

class CUnityAdsManager{
public:
    static CUnityAdsManager* Instance();
    
    void Initialize(bool testMode = false);
    
    void ShowUnityAds(const std::function<void(void)> &func, bool isRewarded = true);
    
    void Toast(std::string content);
    
    void IsNetworkConnect();
    
    void CallUnityAdsSavedFunction();
    
    void CallNetworkConnectSavedFunction();
private:
    CUnityAdsManager();
    ~CUnityAdsManager();
    
    CC_SYNTHESIZE(bool, m_IsNetworkConnect, IsNetworkConnect);
    CC_SYNTHESIZE(std::function<void(void)>, m_UnityAdsSavedFunc, UnityAdsSavedFunc);
    CC_SYNTHESIZE(std::function<void(void)>, m_NetworkConnectSavedFunc, NetworkConnectSavedFunc);
};


/* implement these functions with sdk api of different platforms */
#ifdef __cplusplus
extern "C" {
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
    
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_UnityAdsJNI_reward (JNIEnv *, jobject, jstring s);
    static char* jstringTostring(JNIEnv* env, jstring jstr);
    static std::string jstringToStdstring(JNIEnv* env, jstring jstr);
    
    // Unity Ads required methods
    extern void UnityAdsInit(const char* parameter, bool testMode);
    extern bool UnityAdsIsReady (const char* parameter);
    extern void UnityAdsShow (const char* parameter);
    
    // Unity Ads assist methods
    extern bool UnityAdsGetDebugMode();
    extern std::string UnityAdsGetPlacementState(const char* parameter);
    extern std::string UnityAdsGetVersion();
    extern bool UnityAdsIsInitialized();
    extern bool UnityAdsIsSupported();
    extern void UnityAdsSetDebugMode(bool debugMode);
  
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    // Unity Ads required methods
    void UnityAdsInit (const char *parameter, bool testMode);
    bool UnityAdsIsReady (const char *parameter);
    void UnityAdsShow (const char *parameter);
    
    // Unity Ads assist methods
    bool UnityAdsGetDebugMode();
    std::string UnityAdsGetPlacementState(const char* parameter);
    std::string UnityAdsGetVersion();
    bool UnityAdsIsInitialized();
    bool UnityAdsIsSupported();
    void UnityAdsSetDebugMode(bool debugMode);
#endif
    
#ifdef __cplusplus
}
#endif
