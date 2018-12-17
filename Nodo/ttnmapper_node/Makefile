USER_LIB_PATH:=$(realpath ../libraries)

ARDUINO_LIBS=\
  SPI\
  IBM_LMIC_framework\
  Low-Power

ARDUINO_DIR:=$(HOME)/opt/arduino-$(ARDUINO_IDE_VERSION)
ARDMK_DIR:=$(HOME)/opt/arduino-mk

CORE = MiniCore
ARCHITECTURE = avr
BOARD_TAG = 328
MCU=atmega328p
F_CPU=8000000L
HEX_MAXIMUM_SIZE=32256
BOARD_VERSION=2.0.1

ARDUINO_PLATFORM_LIB_PATH:=$(HOME)/.arduino15/packages/$(CORE)/hardware/$(ARCHITECTURE)/$(BOARD_VERSION)/libraries
ARDUINO_CORE_PATH:=$(HOME)/.arduino15/packages/$(CORE)/hardware/$(ARCHITECTURE)/$(BOARD_VERSION)/cores/MCUdude_corefiles
ARDUINO_VAR_PATH:=$(HOME)/.arduino15/packages/$(CORE)/hardware/$(ARCHITECTURE)/$(BOARD_VERSION)/variants
VARIANT=standard


include $(ARDMK_DIR)/Arduino.mk
