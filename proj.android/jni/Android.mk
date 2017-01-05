LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos/audio/include)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../proj.win32/Classes/AI/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../proj.win32/Classes/AI/States/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../proj.win32/Classes/Common/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../proj.win32/Classes/DataManager/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../proj.win32/Classes/DataManager/ChallengeRewarder/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../proj.win32/Classes/DataManager/ChallengeChecker/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../proj.win32/Classes/GameObject/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../proj.win32/Classes/GameObject/Bullet/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../proj.win32/Classes/GameObject/Shooter/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../proj.win32/Classes/json/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../proj.win32/Classes/MyUI/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../proj.win32/Classes/MyUI/Popup/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../proj.win32/Classes/Particle/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../proj.win32/Classes/Scene/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../proj.win32/Classes/Task/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../proj.win32/Classes/SDKUtil/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../proj.win32/Classes/GoogleCloud/*.cpp)
PROJECT_FILES += $(wildcard $(LOCAL_PATH)/../../proj.win32/Classes/Download/*.cpp)

PROJECT_FILES := $(PROJECT_FILES:$(LOCAL_PATH)/%=%)

LOCAL_SRC_FILES := hellocpp/main.cpp \
					../../proj.win32/Classes/AppDelegate.cpp\
                   $(PROJECT_FILES) \


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../proj.win32/Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
