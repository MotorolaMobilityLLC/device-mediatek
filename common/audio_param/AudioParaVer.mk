#
# audio params release notes
#
# history:
#  2017-03-09 本次主要优化通话主客观和产线/ALT所报问题
#  2017-03-13 主要调试 宽频客观通话指标
#  2017-03-18 主要调试 改善杂音
AUDIO_PARA_VER := MANNING-DVT2-2017-3-18
PRODUCT_PROPERTY_OVERRIDES += persist.audio.lenovo.version=$(AUDIO_PARA_VER)
