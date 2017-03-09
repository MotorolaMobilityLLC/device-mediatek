ifeq ($(strip $(TARGET_USERIMAGES_USE_F2FS)), true)
LOCAL_PATH := $(call my-dir)

signtool_dir := $(LOCAL_PATH)/../../../../vendor/mediatek/proprietary/scripts/sign-image
define custom-sign-f2fs-image
	$(hide) mkdir -p $(ANDROID_PRODUCT_OUT)/unsigned-image/
	$(hide) cp -af $1 $(ANDROID_PRODUCT_OUT)/unsigned-image/
	$(hide) tempdir=`date +%s.%N` && mkdir -p /tmp/$$tempdir && \
		cp -af $1 /tmp/$$tempdir/`basename $1` && \
		echo "Lenovo sign: $1" && python $(signtool_dir)/lenovo-sign-image.py -i /tmp/$$tempdir/`basename $1` -o $2 && \
		rm -rf /tmp/$$tempdir/
	$(hide) cp -af $2 $1
endef


#copy userdata.img to out
.PHONY: f2fs_userdata_image
f2fs_userdata_image: RIVATE_PATH := $(LOCAL_PATH)
f2fs_userdata_image: userdataimage
	$(hide) echo -e "\n## Copy f2fs image to userdata.img ##\n"
	$(hide) cp -rf $(RIVATE_PATH)/userdata.img $(PRODUCT_OUT)/
ifeq ($(SECURE_BOOT),yes)
	$(hide) $(call custom-sign-f2fs-image, $(PRODUCT_OUT)/userdata.img, $(PRODUCT_OUT)/signed-image/userdata-sign.img)
endif


# depend on default droid taget
droid: f2fs_userdata_image
otapackage: f2fs_userdata_image
endif

