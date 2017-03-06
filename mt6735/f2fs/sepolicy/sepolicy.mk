#
# Lenovo sepolicy, mind that all the developers should
# just extend the sepolicy rule here for all the lenovo
# applications or components with creating new files for
# labeling the new contexts or new domain.
#
LOCAL_PATH:= $(call my-dir)

BOARD_SEPOLICY_DIRS += device/mediatek/mt6735/f2fs/sepolicy/
