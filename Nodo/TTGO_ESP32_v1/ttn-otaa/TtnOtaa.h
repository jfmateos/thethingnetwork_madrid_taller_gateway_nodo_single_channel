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
#else
typedef void (*onLMICEvent_t)(ev_t);
#endif

class TTNotaa
{
 protected:
	 u4_t netid = 0;
	 devaddr_t devaddr = 0;
	 u1_t nwkKey[16];
	 u1_t artKey[16];

	 onLMICEvent_t throwEvent = NULL;
	 

 public:
	void begin();
	void onEvent (ev_t ev);
	void setEventHandler (onLMICEvent_t handler);
	u1_t PROGMEM APPEUI[8];
	u1_t PROGMEM DEVEUI[8];
	u1_t PROGMEM APPKEY[16];
};

extern TTNotaa TtnOtaa;

#endif

