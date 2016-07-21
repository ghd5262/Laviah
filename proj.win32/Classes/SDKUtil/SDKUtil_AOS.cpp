#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)



#include "SDKUtil_AOS.h"
#include "platform/android/jni/JniHelper.h"
#include "cocos2d.h"

USING_NS_CC;

CSDKUtil_AOS::CSDKUtil_AOS()
{
    m_JavaUrlString = "org/cocos2dx/cpp/AppActivity";
}

CSDKUtil_AOS::~CSDKUtil_AOS()
{
    
}

void CSDKUtil_AOS::JavaCallMethod(const char* methodName)
{
    CCLOG("JavaCallMethod %s", methodName);
    cocos2d::JniMethodInfo info;
    bool isHave = cocos2d::JniHelper::getStaticMethodInfo(info, m_JavaUrlString.c_str(), methodName, "()V");
    if (isHave) {
        info.env->CallStaticVoidMethod(info.classID, info.methodID);
    } else {
        CCLOG("There is no %s java method", methodName);
    }
}

// 구글 클라우드 저장 Key / Value(json)
void CSDKUtil_AOS::GoogleCloudSend(std::string key, std::string value)
{
    std::string methodName = "GoogleCloudSend";
    CCLOG("JavaCallMethod %s", methodName.c_str());
    cocos2d::JniMethodInfo info;
    bool isHave = cocos2d::JniHelper::getStaticMethodInfo(info, m_JavaUrlString.c_str(), methodName.c_str(), "(Ljava/lang/String;Ljava/lang/String;)V");
    if (isHave) {
        jstring stringArg1 = info.env->NewStringUTF(key.c_str());
        jstring stringArg2 = info.env->NewStringUTF(value.c_str());
        
        info.env->CallStaticVoidMethod(info.classID, info.methodID, stringArg1, stringArg2);
    } else {
        CCLOG("There is no %s java method", methodName.c_str());
    }
}

// 유니티 애드 보상형 광고
void CSDKUtil_AOS::ShowUnityAdIncentivized()
{
    JavaCallMethod("ShowUnityAdIncentivized");
}

// 유니티 애드 일반 광고
void CSDKUtil_AOS::ShowUnityAdInterstitial()
{
    JavaCallMethod("ShowUnityAdInterstitial");
}

// 토스트창
void CSDKUtil_AOS::Toast(std::string content)
{
    std::string methodName = "Toast";
    CCLOG("JavaCallMethod %s", methodName.c_str());
    cocos2d::JniMethodInfo info;
    bool isHave = cocos2d::JniHelper::getStaticMethodInfo(info, m_JavaUrlString.c_str(), methodName.c_str(), "(Ljava/lang/String;)V");
    if (isHave) {
        jstring stringArg1 = info.env->NewStringUTF(content.c_str());
        
        info.env->CallStaticVoidMethod(info.classID, info.methodID, stringArg1);
    } else {
        CCLOG("There is no %s java method", methodName.c_str());
    }
}


#endif