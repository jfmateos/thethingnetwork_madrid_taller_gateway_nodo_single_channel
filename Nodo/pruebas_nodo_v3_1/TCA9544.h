/*
TCA9544 Library
.h file

This library is used to simplify the controll scheme of the TCA9544 chip for use with a bioreactor controller.


Lukas Jaworski

University of Miami
2016
*/

//note, the function sd.begin() will need to be called somewhere in the mian body of your program

#ifndef TCA9544_H
#define TCA9544_H
#include <Wire.h>
#include "Arduino.h"


class TCA9544 {
	public:
	TCA9544(uint8_t addressBits = 0);
	void changeChannel(uint8_t channelSelect);//The selection is ZERO indexed (0,1,2,3)
	uint8_t currentChannel();// returns the current active channel-actually reads the device control register
	uint8_t readInterrupts();// returns which channels currently have active interrupts
	
	private:
	uint8_t controlRegisterState;
	uint8_t deviceAddress;
	
	
};
#endif
