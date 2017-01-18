ifeq ($(strip $(TARGET_USERIMAGES_USE_F2FS)), true)
LOCAL_PATH := $(call my-dir)

#copy userdata.img to out
.PHONY: f2fs_userdata_image
f2fs_userdata_image: RIVATE_PATH := $(LOCAL_PATH)
f2fs_userdata_image: userdataimage

	$(hide) echo -e "\n## Copy f2fs image to userdata.img ##\n"
	$(hide) cp -rf $(RIVATE_PATH)/userdata.img $(PRODUCT_OUT)/

# depend on default droid taget
droid: f2fs_userdata_image
otapackage: f2fs_userdata_image
endif

