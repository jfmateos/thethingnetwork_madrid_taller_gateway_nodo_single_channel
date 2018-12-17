#include "TCA9544.h"

TCA9544::TCA9544(uint8_t addressBits){//feed in the A0-A2 bits to automatically adjust the TCA's address
	deviceAddress = addressBits + 0x70;
	controlRegisterState = 0; //The control register is zero on intialization
}

void TCA9544::changeChannel(uint8_t channelSelect){
	controlRegisterState = ((controlRegisterState & 0b11110000) | (0b100 | channelSelect));
	Wire.beginTransmission(deviceAddress);
	Wire.write(controlRegisterState);
	Wire.endTransmission();
}

uint8_t TCA9544::readInterrupts(){
	uint8_t inters;
	Wire.requestFrom(deviceAddress, uint8_t(1));
	if (Wire.available()){
		controlRegisterState = Wire.read();
	}
	inters = controlRegisterState >> 4;
	return inters;
}

uint8_t TCA9544::currentChannel(){
	uint8_t channel;
	Wire.requestFrom(deviceAddress, uint8_t(1));
	if (Wire.available()){
    channel = Wire.read() & 0b11;
	}
	
	return channel;
}
