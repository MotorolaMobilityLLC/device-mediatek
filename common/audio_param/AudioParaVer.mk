#
# audio params release notes
#
# history:
#  2017-03-09 本次主要优化通话主客观和产线/ALT所报问题
#  2017-03-13 主要调试 宽频客观通话指标
#  2017-03-18 主要调试 改善杂音
#  2017-03-30 主要优化免提上下行通话和降噪效
#  2017-04-05 解决联想所报音阶调整为7阶后音量变小问题
#  2017-04-14 本次参数主要调试联想所报免提上行不清晰等问题
#  2017-05-15 本次参数主要调试联想所报主观问题:
#				1、录音录像底噪偏大，人声失真，调试Audio Record,增益和NR参数；
#				2、免提通话效果稍差，调试handfree -NB 模式NR参数；
#				3、耳机通话底噪大稍大，调试headset 模式 NR参数。
# 2017-05-25 mtk优化免提参数，修改SPE参数以及Analog Gain
AUDIO_PARA_VER := MANNING-DVT2-2017-5-25
PRODUCT_PROPERTY_OVERRIDES += persist.audio.lenovo.version=$(AUDIO_PARA_VER)
