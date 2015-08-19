# Hey Emacs, this is a -*- makefile -*-
#
# crazyflie_2.0.makefile
#
# https://www.bitcraze.io/crazyflie-2/
#

BOARD=crazyflie
BOARD_VERSION=2.0
BOARD_CFG=\"boards/$(BOARD)_$(BOARD_VERSION).h\"

ARCH=stm32
ARCH_L=f4
HARD_FLOAT=yes
$(TARGET).ARCHDIR = $(ARCH)
$(TARGET).LDSCRIPT=$(SRC_ARCH)/lisa-mx.ld

# -----------------------------------------------------------------------

# default flash mode is via usb dfu bootloader (luftboot)
# other possibilities: DFU-UTIL, SWD, JTAG_BMP, STLINK, SERIAL
FLASH_MODE ?= DFU-UTIL

# while the crazyflie doesn't have luftboot, it has a bootloader until same offset
# see also https://wiki.bitcraze.io/projects:crazyflie2:development:dfu
HAS_BOOTLOADER ?= 1
ifeq (,$(findstring $(HAS_BOOTLOADER),0 FALSE))
$(TARGET).CFLAGS+=-DLUFTBOOT
$(TARGET).LDFLAGS+=-Wl,-Ttext=0x8004000
DFU_ADDR = 0x8004000
#DFU_PRODUCT = Lisa/Lia
endif


#
# default LED configuration
#
RADIO_CONTROL_LED  ?= 4
BARO_LED           ?= none
AHRS_ALIGNER_LED   ?= 2
GPS_LED            ?= none
SYS_TIME_LED       ?= 1


#
# default uart configuration
#

MODEM_PORT ?= UART1
MODEM_BAUD ?= B57600


#
# default actuator configuration
#
# you can use different actuators by adding a configure option to your firmware section
# e.g. <configure name="ACTUATORS" value="actuators_ppm/>
# and by setting the correct "driver" attribute in servo section
# e.g. <servo driver="Ppm">
#
ACTUATORS ?= actuators_pwm
