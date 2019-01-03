
obj-y += base/nx_bit_accessor.o

obj-y += module/nx_rstcon.o
obj-y += module/nx_clkgen.o
obj-y += module/nx_clkpwr.o
obj-y += module/nx_mcus.o
obj-y += module/nx_alive.o
obj-y += module/nx_gpio.o
obj-y += module/nx_pwm.o

obj-y += module/nx_usb20host.o
obj-y += module/nx_usb20otg.o
obj-y += module/nx_tieoff.o

obj-$(CONFIG_CMD_MMC)				+= module/nx_sdmmc.o
obj-$(CONFIG_SPI)					+= module/nx_ssp.o

obj-$(CONFIG_DISPLAY_OUT)			+= 	module/nx_displaytop.o		\
								   		module/nx_disptop_clkgen.o	\
								   		module/nx_dualdisplay.o		\
								   		module/nx_mlc.o				\
								   		module/nx_dpc.o

obj-$(CONFIG_DISPLAY_OUT_LVDS)	+= module/nx_lvds.o
obj-$(CONFIG_DISPLAY_OUT_MIPI)	+= module/nx_mipi.o
obj-$(CONFIG_DISPLAY_OUT_RESCONV)	+= module/nx_resconv.o

obj-$(CONFIG_DISPLAY_OUT_HDMI)	+= module/nx_hdmi.o
obj-$(CONFIG_DISPLAY_OUT_HDMI)	+= module/nx_ecid.o
obj-$(CONFIG_DISPLAY_OUT_HDMI)	+= module/nx_i2s.o
# obj-$(CONFIG_DISPLAY_OUT_HDMI)	+= 	module/nx_displaytop.o		\
# 								   		module/nx_disptop_clkgen.o	\
# 								   		module/nx_dualdisplay.o		\
# 								   		module/nx_mlc.o				\
# 								   		module/nx_dpc.o
obj-y	+= module/nx_rtc.o
obj-$(CONFIG_VIP) += module/nx_vip.o
