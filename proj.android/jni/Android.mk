LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

#compile src--------------------------------------------------------------------
cppfilestemp1 := $(shell find $(LOCAL_PATH)/../../Classes -type d)
cppfilestemp2 := $(shell find $(cppfilestemp1) -name *.cpp -o -name *.c)
cppfilestemp3 := $(sort $(cppfilestemp2))
cppfiles := $(subst $(LOCAL_PATH)/,,$(cppfilestemp3))
$(info now....compile srcs....)
#-------------------------------------------------------------------------------


LOCAL_SRC_FILES := hellocpp/main.cpp \
                   $(cppfiles) \


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
