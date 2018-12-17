ARDUINO_DIR := $(HOME)/opt/arduino-1.8.6
INO_FILE := ESP-sc-gway.ino

all:
	$(ARDUINO_DIR)/arduino-builder \
		-compile \
		-logger=human \
		-hardware $(ARDUINO_DIR)/hardware \
		-hardware $(HOME)/.arduino15/packages \
		-tools $(ARDUINO_DIR)/tools-builder \
		-tools $(ARDUINO_DIR)/hardware/tools/avr \
		-tools $(HOME)/.arduino15/packages \
		-built-in-libraries $(ARDUINO_DIR)/libraries \
		-libraries ../../libraries \
                -prefs=build.warn_data_percentage=75 \
		-fqbn=esp8266:esp8266:d1_mini:CpuFrequency=80,VTable=flash,FlashSize=4M1M,LwIPVariant=v2mss536,Debug=Disabled,DebugLevel=None____,FlashErase=sdk,UploadSpeed=921600 \
		-ide-version=10806 \
		$(INO_FILE)
