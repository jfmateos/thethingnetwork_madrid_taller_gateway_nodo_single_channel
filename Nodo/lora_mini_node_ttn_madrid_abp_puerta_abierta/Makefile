ARDUINO_DIR := $(HOME)/opt/arduino-1.8.6
INO_FILE := lora_mini_node_ttn_madrid_abp_puerta_abierta.ino

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
		-libraries ../libraries \
		-fqbn=MiniCore:avr:328:bootloader=true,variant=modelP,BOD=1v8,LTO=Os,clock=8MHz_external \
		-ide-version=10806 \
		-warnings=all \
		-prefs=build.warn_data_percentage=75 \
		$(INO_FILE)
