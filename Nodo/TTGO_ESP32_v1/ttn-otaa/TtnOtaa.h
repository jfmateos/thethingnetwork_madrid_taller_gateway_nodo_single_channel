// TtnOtaa.h

#ifndef _TTNOTAA_h
#define _TTNOTAA_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>

#define BUILTIN_LED 25

void os_getArtEui (u1_t* buf);
void os_getDevEui (u1_t* buf);
void os_getDevKey (u1_t* buf);

#if defined ARDUINO_ARCH_ESP8266 || defined ARDUINO_ARCH_ESP32
#include <functional>
typedef std::function<void (ev_t)> onLMICEvent_t;
typedef std::function<void (uint8_t*, uint8_t)> onDownlinkData_t;
#else
typedef void (*onLMICEvent_t)(ev_t);
typedef void (*onDownlinkData_t)(uint8_t*, uint8_t);
#endif

class TTNotaa
{
 protected:
	 u4_t netid = 0;
	 devaddr_t devaddr = 0;
	 u1_t nwkKey[16];
	 u1_t artKey[16];

	 onLMICEvent_t throwEvent = NULL;
	 onDownlinkData_t onDownlinkData = NULL;
	 

 public:
	u1_t appeui[8];
	u1_t deveui[8];
	u1_t appkey[16];

	void begin (const u1_t appeui[8], const u1_t deveui[8], const u1_t appkey[16]);
	void onEvent (ev_t ev);
	void setEventHandler (onLMICEvent_t handler);
	void setDownlinkHandler (onDownlinkData_t handler);

	// request data delivery to TTN
	// returns	 0 if OK, 
	//			-1 if transaction pending
	//			-2 if too much data
	int requestSendData (uint8_t *data, uint8_t len);

};

extern TTNotaa TtnOtaa;

#endif

