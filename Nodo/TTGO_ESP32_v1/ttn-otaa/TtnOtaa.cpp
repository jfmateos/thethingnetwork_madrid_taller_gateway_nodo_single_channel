// 
// 
// 

#include "TtnOtaa.h"

void os_getArtEui (u1_t* buf) { memcpy_P (buf, TtnOtaa.APPEUI, 8); }
void os_getDevEui (u1_t* buf) { memcpy_P (buf, TtnOtaa.DEVEUI, 8); }
void os_getDevKey (u1_t* buf) { memcpy_P (buf, TtnOtaa.APPKEY, 16); }
void onEvent (ev_t ev) { TtnOtaa.onEvent (ev); }

// Pin mapping for TTGO ESP32 v1
const lmic_pinmap lmic_pins = {
	.nss = 18,
	.rxtx = LMIC_UNUSED_PIN,
	.rst = 14,
	.dio = {/*dio0*/ 26, /*dio1*/ 33, /*dio2*/ 32 }
};

void TTNotaa::begin(const u1_t appeui[8], const u1_t deveui[8], const u1_t appkey[16])
{
	memcpy_P (TtnOtaa.APPEUI, appeui, 8);
	memcpy_P (TtnOtaa.DEVEUI, deveui, 8);
	memcpy_P (TtnOtaa.APPKEY, appkey, 16);
	// LMIC init
	os_init ();
	// Reset the MAC state. Session and pending data transfers will be discarded.
	LMIC_reset ();

	LMIC_setupChannel (0, 868100000, DR_RANGE_MAP (DR_SF12, DR_SF7), BAND_CENTI);      // g-band
	LMIC_setupChannel (1, 868100000, DR_RANGE_MAP (DR_SF12, DR_SF7), BAND_CENTI);      // g-band
	LMIC_setupChannel (2, 868100000, DR_RANGE_MAP (DR_SF12, DR_SF7), BAND_CENTI);      // g-band
	LMIC_setupChannel (3, 868100000, DR_RANGE_MAP (DR_SF12, DR_SF7), BAND_CENTI);      // g-band
	LMIC_setupChannel (4, 868100000, DR_RANGE_MAP (DR_SF12, DR_SF7), BAND_CENTI);      // g-band
	LMIC_setupChannel (5, 868100000, DR_RANGE_MAP (DR_SF12, DR_SF7), BAND_CENTI);      // g-band
	LMIC_setupChannel (6, 868100000, DR_RANGE_MAP (DR_SF12, DR_SF7), BAND_CENTI);      // g-band
	LMIC_setupChannel (7, 868100000, DR_RANGE_MAP (DR_SF12, DR_SF7), BAND_CENTI);      // g-band
	LMIC_setupChannel (8, 868100000, DR_RANGE_MAP (DR_SF12, DR_SF7), BAND_CENTI);      // g2-band

																					   // TTN uses SF9 for its RX2 window.
	LMIC.dn2Dr = DR_SF9;

	// Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library)
	LMIC_setDrTxpow (DR_SF7, 14);
}

void TTNotaa::onEvent (ev_t ev) {
	Serial.print (os_getTime ());
	Serial.print (": ");
	switch (ev) {
	case EV_SCAN_TIMEOUT:
		Serial.println (F ("EV_SCAN_TIMEOUT"));
		break;
	case EV_BEACON_FOUND:
		Serial.println (F ("EV_BEACON_FOUND"));
		break;
	case EV_BEACON_MISSED:
		Serial.println (F ("EV_BEACON_MISSED"));
		break;
	case EV_BEACON_TRACKED:
		Serial.println (F ("EV_BEACON_TRACKED"));
		break;
	case EV_JOINING:
		Serial.println (F ("EV_JOINING"));
		break;
	case EV_JOINED:
		Serial.println (F ("EV_JOINED"));
		{
			LMIC_getSessionKeys (&netid, &devaddr, nwkKey, artKey);
			Serial.print ("netid: ");
			Serial.println (netid, DEC);
			Serial.print ("devaddr: ");
			Serial.println (devaddr, HEX);
			Serial.print ("artKey: ");
			for (int i = 0; i<sizeof (artKey); ++i) {
				Serial.print (artKey[i], HEX);
			}
			Serial.println ("");
			Serial.print ("nwkKey: ");
			for (int i = 0; i<sizeof (nwkKey); ++i) {
				Serial.print (nwkKey[i], HEX);
			}
			Serial.println ("");

			LMIC_setSeqnoUp (1);
		}
		// Disable link check validation (automatically enabled
		// during join, but not supported by TTN at this time).
		LMIC_setLinkCheckMode (0);
		LMIC_setupChannel (0, 868100000, DR_RANGE_MAP (DR_SF12, DR_SF7), BAND_CENTI);      // g-band
		LMIC_setupChannel (1, 868100000, DR_RANGE_MAP (DR_SF12, DR_SF7), BAND_CENTI);      // g-band
		LMIC_setupChannel (2, 868100000, DR_RANGE_MAP (DR_SF12, DR_SF7), BAND_CENTI);      // g-band
		LMIC_setupChannel (3, 868100000, DR_RANGE_MAP (DR_SF12, DR_SF7), BAND_CENTI);      // g-band
		LMIC_setupChannel (4, 868100000, DR_RANGE_MAP (DR_SF12, DR_SF7), BAND_CENTI);      // g-band
		LMIC_setupChannel (5, 868100000, DR_RANGE_MAP (DR_SF12, DR_SF7), BAND_CENTI);      // g-band
		LMIC_setupChannel (6, 868100000, DR_RANGE_MAP (DR_SF12, DR_SF7), BAND_CENTI);      // g-band
		LMIC_setupChannel (7, 868100000, DR_RANGE_MAP (DR_SF12, DR_SF7), BAND_CENTI);      // g-band
		LMIC_setupChannel (8, 868100000, DR_RANGE_MAP (DR_SF12, DR_SF7), BAND_CENTI);      // g2-band

		break;
	case EV_RFU1:
		Serial.println (F ("EV_RFU1"));
		break;
	case EV_JOIN_FAILED:
		Serial.println (F ("EV_JOIN_FAILED"));
		break;
	case EV_REJOIN_FAILED:
		Serial.println (F ("EV_REJOIN_FAILED"));
		break;
	case EV_TXCOMPLETE:
		Serial.println (F ("EV_TXCOMPLETE (includes waiting for RX windows)"));
		if (LMIC.txrxFlags & TXRX_ACK) {
			Serial.println (F ("Received ack"));
		}
		if (LMIC.dataLen) {
			Serial.print (F ("Received "));
			Serial.print (LMIC.dataLen);
			Serial.println (F (" bytes of payload"));
		}
		// Schedule next transmission
		// os_setTimedCallback (&sendjob, os_getTime () + sec2osticks (TX_INTERVAL), do_send);
		break;
	case EV_LOST_TSYNC:
		Serial.println (F ("EV_LOST_TSYNC"));
		break;
	case EV_RESET:
		Serial.println (F ("EV_RESET"));
		break;
	case EV_RXCOMPLETE:
		// data received in ping slot
		Serial.println (F ("EV_RXCOMPLETE"));
		break;
	case EV_LINK_DEAD:
		Serial.println (F ("EV_LINK_DEAD"));
		break;
	case EV_LINK_ALIVE:
		Serial.println (F ("EV_LINK_ALIVE "));
		break;
	case EV_TXSTART:
		Serial.println (F ("EV_TXSTART "));
		break;
	default:
		Serial.print (F ("Unknown event: "));
		Serial.println (ev);
		break;
	}
	if (throwEvent) {
		throwEvent (ev);
	}
}

void TTNotaa::setEventHandler (onLMICEvent_t handler) {
	throwEvent = handler;
}

TTNotaa TtnOtaa;
