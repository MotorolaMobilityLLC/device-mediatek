LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE        := override_nvram

LOCAL_MODULE_TAGS   := optional

LOCAL_SHARED_LIBRARIES := libcutils liblog libnvram

LOCAL_C_INCLUDES := \
	$(MTK_PATH_SOURCE)/external/nvram/libnvram \
	$(MTK_PATH_SOURCE)/custom/common/cgen/inc \
	$(MTK_PATH_CUSTOM)/cgen/cfgfileinc \
	$(MTK_PATH_CUSTOM)/cgen/inc

LOCAL_SRC_FILES     := override_nvram.c

include $(BUILD_EXECUTABLE)
