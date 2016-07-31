#include "SDKUtil_AOS.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include "cocos2d.h"
#include "SDKUtil.h"
#include "../GoogleCloud/GoogleCloudManager.h"

USING_NS_CC;

CSDKUtil_AOS::CSDKUtil_AOS()
	: m_JavaUrlString("")
{
    m_JavaUrlString = "org/cocos2dx/cpp/AppActivity";
}

CSDKUtil_AOS::~CSDKUtil_AOS()
{
    
}

void CSDKUtil_AOS::JavaCallMethod(const char* methodName)
{
    CCLOG("CPP_JavaCallMethod %s", methodName);
    cocos2d::JniMethodInfo info;
    bool isHave = cocos2d::JniHelper::getStaticMethodInfo(info, m_JavaUrlString.c_str(), methodName, "()V");
    if (isHave) {
        info.env->CallStaticVoidMethod(info.classID, info.methodID);
    } else {
        CCLOG("There is no %s java method", methodName);
    }
}

// 구글 클라우드 저장 Key / Value
void CSDKUtil_AOS::GoogleCloudSave(std::string key, std::string value)
{
    std::string methodName = "CPP_GoogleCloudSave";
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

// 구글 클라우드 로드 Key
void CSDKUtil_AOS::GoogleCloudLoad(std::string key)
{
	std::string methodName = "CPP_GoogleCloudLoad";
	CCLOG("JavaCallMethod %s", methodName.c_str());
	cocos2d::JniMethodInfo info;
	bool isHave = cocos2d::JniHelper::getStaticMethodInfo(info, m_JavaUrlString.c_str(), methodName.c_str(), "(Ljava/lang/String;)V");
	if (isHave) {
		jstring stringArg1 = info.env->NewStringUTF(key.c_str());

		info.env->CallStaticVoidMethod(info.classID, info.methodID, stringArg1);
	}
	else {
		CCLOG("There is no %s java method", methodName.c_str());
	}
}

// 유니티 애드 보상형 광고
void CSDKUtil_AOS::ShowRewardUnityAds()
{
	JavaCallMethod("CPP_ShowRewardUnityAds");
}

// 유니티 애드 일반 광고
void CSDKUtil_AOS::ShowNormalUnityAds()
{
	JavaCallMethod("CPP_ShowNormalUnityAds");
}

// 토스트창
void CSDKUtil_AOS::Toast(std::string content)
{
    std::string methodName = "CPP_Toast";
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

extern "C" {
	// 구글 클라우드 로그인 성공
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_JAVA_1GoogleConnectionResult(JNIEnv*  env, jobject thiz, bool isSucceed)
	{
		std::string methodName = "JAVA_1GoogleConnectionResult";
		CCLOG("JavaCallBackMethod %s", methodName.c_str());
		CCLOG("Google Cloud Connection %s", (isSucceed == true) ? "SUCCEED", "FAILED");
		CGoogleCloudManager::Instance()->IsConnected(isSucceed);
	}

	// 구글 클라우드 커넥션 끊김
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_JAVA_1GoogleConnectionSuspended(JNIEnv*  env, jobject thiz)
	{
		std::string methodName = "JAVA_1GoogleConnectionSuspended";
		CCLOG("JavaCallBackMethod %s", methodName.c_str());
	}

	// 구글 클라우드 데이터 로드 
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_JAVA_1GoogleCloudLoad(JNIEnv*  env, jobject thiz, jstring key, jstring value)
	{
		std::string methodName = "JAVA_1GoogleCloudLoad";
		CCLOG("JavaCallBackMethod %s", methodName.c_str());

		std::string keyStr = JniHelper::jstring2string(key);
		std::string valueStr = JniHelper::jstring2string(value);

		Director::getInstance()->getScheduler()->schedule([keyStr, valueStr](float delta){
			CGoogleCloudManager::Instance()->GoogleCloudDataLoad(keyStr, valueStr);
		}, Director::getInstance(), 0.f, 0, 0.f, false, "GoogleCloudDataLoad");
	}

	// 유니티 일반 광고 준비완료 
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_JAVA_1NormalUnityAdsReady(JNIEnv*  env, jobject thiz)
	{
		std::string methodName = "JAVA_1NormalUnityAdsReady";
		CCLOG("JavaCallBackMethod %s", methodName.c_str());

		CSDKUtil::Instance()->setIsNormalUnityAdsReady(true);
	}
    
	// 유니티 보상형 광고 준비완료
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_JAVA_1RewardUnityAdsReady(JNIEnv*  env, jobject thiz)
	{
		std::string methodName = "JAVA_1RewardUnityAdsReady";
		CCLOG("JavaCallBackMethod %s", methodName.c_str());

		CSDKUtil::Instance()->setIsRewardUnityAdsReady(true);
	}

	// 유니티 광고 시작
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_JAVA_1UnityAdsStart(JNIEnv*  env, jobject thiz)
    {
        std::string methodName = "JAVA_1UnityAdsStart";
        CCLOG("JavaCallBackMethod %s", methodName.c_str());
    }
    
	// 유니티 광고 종료
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_JAVA_1UnityAdsFinish(JNIEnv*  env, jobject thiz)
    {
        std::string methodName = "JAVA_1UnityAdsFinish";
        CCLOG("JavaCallBackMethod %s", methodName.c_str());

		Director::getInstance()->getScheduler()->schedule([](float delta){
			CSDKUtil::Instance()->CallSavedFuncion();
		}, Director::getInstance(), 0.f, 0, 0.f, false, "CallSavedFuncion");
    }
    
	// 유니티 광고 에러
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_JAVA_1UnityAdsError(JNIEnv*  env, jobject thiz)
    {
        std::string methodName = "JAVA_1UnityAdsError";
        CCLOG("JavaCallBackMethod %s", methodName.c_str());
    }
}

#endif