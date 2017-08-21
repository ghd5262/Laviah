LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos/audio/include)
$(call import-add-path, $(LOCAL_PATH))

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/AI/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/AI/States/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/Common/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/DataManager/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/DataManager/AchievementRewarder/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/DataManager/AchievementChecker/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/GameObject/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/GameObject/Bullet/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/GameObject/Shooter/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/json/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/MyUI/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/MyUI/Popup/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/MyUI/Tutorial/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/MyUI/Popup/Option/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/Particle/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/Scene/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/Task/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/Download/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/SDKBOX/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/SDKBOX/Play/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/SDKBOX/UnityAds/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/SDKBOX/Facebook/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/SDKBOX/Share/*.cpp)

PROJECT_FILES := $(PROJECT_FILES:$(LOCAL_PATH)/%=%)

LOCAL_SRC_FILES := hellocpp/main.cpp \
			../../Classes/AppDelegate.cpp\
			$(PROJECT_FILES) \

LOCAL_CPPFLAGS := -DSDKBOX_ENABLED
LOCAL_LDLIBS := -landroid -llog
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_WHOLE_STATIC_LIBRARIES := PluginFacebook \
sdkbox \
PluginSdkboxPlay \
PluginShare

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module, ./sdkbox)
$(call import-module, ./pluginfacebook)
$(call import-module, ./pluginsdkboxplay)
$(call import-module, ./pluginshare)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
