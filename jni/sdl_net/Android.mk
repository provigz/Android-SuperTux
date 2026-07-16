LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := sdl_net

LOCAL_CFLAGS := -I$(LOCAL_PATH) -I$(LOCAL_PATH)/../sdl/include

LOCAL_CPP_EXTENSION := .cpp

LOCAL_SRC_FILES := $(notdir $(wildcard $(LOCAL_PATH)/*.c))

LOCAL_STATIC_LIBRARIES := sdl
LOCAL_SHARED_LIBRARIES :=
LOCAL_LDLIBS :=

include $(BUILD_STATIC_LIBRARY)

