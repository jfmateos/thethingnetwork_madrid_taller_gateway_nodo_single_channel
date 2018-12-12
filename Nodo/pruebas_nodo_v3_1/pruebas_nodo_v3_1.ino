#include <Arduino.h>
#include  "adcvcc.h"
#include <Wire.h>
#include <U8x8lib.h>
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
#include <TinyGPS++.h>
TinyGPSPlus gps;
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
// Assign a Uniquej ID to the HMC5883 Compass Sensor
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(23456);
#include "TCA9544.h"
TCA9544 i2c(0);
#include <Adafruit_BME280.h>
Adafruit_BME280 bme;
#include "EmonLib.h"
EnergyMonitor emon1;

//this is the 8bit address, 7bit address = 0x1E
int KX126_DeviceAddress = 0x1E;
int KX126_Accel_X_LB = 0;
int KX126_Accel_X_HB = 0;
int KX126_Accel_Y_LB = 0;
int KX126_Accel_Y_HB = 0;
int KX126_Accel_Z_LB = 0;
int KX126_Accel_Z_HB = 0;
int KX126_Accel_X_RawOUT = 0;
int KX126_Accel_Y_RawOUT = 0;
int KX126_Accel_Z_RawOUT = 0;
float KX126_Accel_X_OUT = 0;
float KX126_Accel_Y_OUT = 0;
float KX126_Accel_Z_OUT = 0;

volatile bool avanzar = false;
volatile bool debounce = false;

/* ======================================================================
  Function: ADC_vect
  Purpose : IRQ Handler for ADC
  Input   : -
  Output  : -
  Comments: used for measuring 8 samples low power mode, ADC is then in
          free running mode for 8 samples
  ====================================================================== */
ISR(ADC_vect)
{
  // Increment ADC counter
  _adc_irq_cnt++;
}

void setup(void) {
  pinMode(A0, OUTPUT);
  digitalWrite(A0, HIGH);
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  delay(100);
  pinMode(3, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(6, OUTPUT);

  Wire.begin();
  Serial.begin(9600);
  delay(100);
  i2c.changeChannel(0);
  delay(100);
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  attachInterrupt(digitalPinToInterrupt(3), pulsar, FALLING);
}
void pulsar() {
  avanzar = true;
  attachInterrupt(digitalPinToInterrupt(3), despulsar, RISING);
}
void despulsar() {
  debounce = true;
}
void loop(void) {
  //Probar OLED

  u8x8.draw1x2String(0, 0, "OLED TEST");
  u8x8.drawString(0, 2, "If you can read");
  u8x8.drawString(0, 3, "this, then the");
  u8x8.drawString(0, 4, "screen works ;-)");
  delay(5000);
  u8x8.clear();
  u8x8.draw1x2String(0, 0, "D3 BUTTON TEST");
  u8x8.drawString(0, 2, "Press D3 button");
  u8x8.drawString(0, 3, "to continue.");
  while (!avanzar) {}
  avanzar = false;
  u8x8.clear();
  u8x8.draw1x2String(0, 0, "A1 BUTTON TEST");
  u8x8.drawString(0, 2, "Press A1 button");
  u8x8.drawString(0, 3, "to continue.");
  while (!debounce) {}
  delay(25);
  debounce = false;
  attachInterrupt(digitalPinToInterrupt(3), pulsar, FALLING);
  while (digitalRead(A1) == HIGH) {}
  while (digitalRead(A1) == LOW) {
    delay(100);
  }

  u8x8.clear();
  u8x8.draw1x2String(0, 0, "LED+BUZZER TEST");
  u8x8.drawString(0, 2, "The LED should");
  u8x8.drawString(0, 3, "blink and the");
  u8x8.drawString(0, 4, "buzzer buzz.");
  u8x8.drawString(0, 5, "Press D3 button");
  u8x8.drawString(0, 6, "to continue.");
  while (!avanzar) {
    tone(6, 1000, 250);
    delay(750);
  }
  avanzar = false;

  u8x8.clear();
  u8x8.draw1x2String(0, 0, "LDR TEST");
  u8x8.drawString(0, 2, "Luminosity: ");
  u8x8.drawString(0, 5, "Press D3 button");
  u8x8.drawString(0, 6, "to continue.");
  while (!debounce) {}
  delay(25);
  debounce = false;
  attachInterrupt(digitalPinToInterrupt(3), pulsar, FALLING);
  while (!avanzar) {
    u8x8.setCursor(12, 2);
    u8x8.print("    ");
    u8x8.setCursor(12, 2);
    u8x8.print(analogRead(A6));
    delay(200);
  }
  avanzar = false;

  u8x8.clear();
  u8x8.draw1x2String(0, 0, "NTC TEST");
  u8x8.drawString(0, 2, "NTC value: ");
  u8x8.drawString(0, 5, "Press D3 button");
  u8x8.drawString(0, 6, "to continue.");
  while (!debounce) {}
  delay(25);
  debounce = false;
  attachInterrupt(digitalPinToInterrupt(3), pulsar, FALLING);
  while (!avanzar) {
    u8x8.setCursor(12, 2);
    u8x8.print("    ");
    u8x8.setCursor(12, 2);
    u8x8.print(analogRead(A7));
    delay(200);
  }
  avanzar = false;

  u8x8.clear();
  u8x8.draw1x2String(0, 0, "GPS TEST");
  u8x8.drawString(0, 2, "LAT: ");
  u8x8.drawString(0, 3, "LON: ");
  u8x8.drawString(0, 4, "TIME: ");
  u8x8.drawString(0, 5, "Press D3 button");
  u8x8.drawString(0, 6, "to continue.");
  while (!debounce) {}
  delay(25);
  debounce = false;
  attachInterrupt(digitalPinToInterrupt(3), pulsar, FALLING);
  while (!avanzar) {
    while (Serial.available() > 0) {
      if (gps.encode(Serial.read())) {
        u8x8.setCursor(5, 2);
        u8x8.print("          ");
        u8x8.setCursor(5, 3);
        u8x8.print("          ");
        if (gps.location.isValid()) {
          u8x8.setCursor(5, 2);
          u8x8.print(gps.location.lat(), 6);
          u8x8.setCursor(5, 3);
          u8x8.print(gps.location.lng(), 6);
        }
        // prints invalid if no information was recieved in regards to location.
        else
        {
          u8x8.setCursor(5, 2);
          u8x8.print("---");
          u8x8.setCursor(5, 3);
          u8x8.print("---");
        }
        u8x8.setCursor(6, 4);
        u8x8.print("         ");
        if (gps.time.isValid())
        {

          u8x8.setCursor(6, 4);
          if (gps.time.hour() < 10) u8x8.print(F("0"));
          u8x8.print(gps.time.hour());
          u8x8.print(F(":"));
          if (gps.time.minute() < 10) u8x8.print(F("0"));
          u8x8.print(gps.time.minute());
          u8x8.print(F(":"));
          if (gps.time.second() < 10) u8x8.print(F("0"));
          u8x8.print(gps.time.second());

        }
        else
        {
          // Print invalid otherwise.
          u8x8.setCursor(6, 4);
          u8x8.print(F("INVALID"));
        }
        delay(200);
      }
    }
  }
  avanzar = false;

  u8x8.clear();
  u8x8.draw1x2String(0, 0, "COMPASS TEST");
  u8x8.drawString(0, 2, "Heading: ");
  u8x8.drawString(0, 5, "Press D3 button");
  u8x8.drawString(0, 6, "to continue.");
  while (!debounce) {}
  delay(25);
  debounce = false;
  attachInterrupt(digitalPinToInterrupt(3), pulsar, FALLING);
  while (!avanzar) {
    i2c.changeChannel(2);
    delay(300);
    if (mag.begin()) {
      sensors_event_t event;
      mag.getEvent(&event);
      float heading = atan2(event.magnetic.y, event.magnetic.x);
      // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
      // Find yours here: http://www.magnetic-declination.com/
      // Mine is: -0* 37' W, which is ~0.616 Degrees, or (which we need) 0.0108 radians
      // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
      // Formula: (deg + (min / 60.0))
      float declinationAngle = 0.0108;
      heading -= declinationAngle;
      if (heading < 0)
        heading += 2 * PI;

      // Check for wrap due to addition of declination.
      if (heading > 2 * PI)
        heading -= 2 * PI;

      // Convert radians to degrees for readability.
      float headingDegrees = heading * 180 / M_PI;
      i2c.changeChannel(0);
      delay(200);
      u8x8.setCursor(9, 2);
      u8x8.print("       ");
      u8x8.setCursor(9, 2);
      u8x8.print(headingDegrees, 1);
    } else {
      i2c.changeChannel(0);
      delay(200);
      u8x8.setCursor(9, 2);
      u8x8.print("       ");
      u8x8.setCursor(9, 2);
      u8x8.print("---");
    }
    delay(200);
  }
  avanzar = false;

  u8x8.clear();
  u8x8.draw1x2String(0, 0, "ACCEL. TEST");
  u8x8.drawString(0, 2, "X: ");
  u8x8.drawString(0, 3, "Y: ");
  u8x8.drawString(0, 4, "Z: ");
  u8x8.drawString(0, 5, "Press D3 button");
  u8x8.drawString(0, 6, "to continue.");
  while (!debounce) {}
  delay(25);
  debounce = false;
  attachInterrupt(digitalPinToInterrupt(3), pulsar, FALLING);

  while (!avanzar) {
    i2c.changeChannel(3);//Cambio de canal porque interfiere con el OLED
    delay(200);
    Wire.beginTransmission(KX126_DeviceAddress);
    Wire.write(0x1A);
    Wire.write(0x40);
    Wire.endTransmission();

    Wire.beginTransmission(KX126_DeviceAddress);
    Wire.write(0x1F);
    Wire.write(0x02);
    Wire.endTransmission();

    Wire.beginTransmission(KX126_DeviceAddress);
    Wire.write(0x1A);
    Wire.write(0xC0);
    Wire.endTransmission();

    Wire.beginTransmission(KX126_DeviceAddress);
    Wire.write(0x08);
    Wire.endTransmission(false);
    Wire.requestFrom(KX126_DeviceAddress, 6, true);
    unsigned char cnt = 0, data[6];
    while (Wire.available()) {
      data[cnt] = Wire.read();
      cnt++;
    }
    
    Wire.endTransmission();
    KX126_Accel_X_RawOUT = (data[1] << 8) | (data[0]);
    KX126_Accel_Y_RawOUT = (data[3] << 8) | (data[2]);
    KX126_Accel_Z_RawOUT = (data[5] << 8) | (data[4]);

    KX126_Accel_X_OUT = (float)KX126_Accel_X_RawOUT / 16384;
    KX126_Accel_Y_OUT = (float)KX126_Accel_Y_RawOUT / 16384;
    KX126_Accel_Z_OUT = (float)KX126_Accel_Z_RawOUT / 16384;

    i2c.changeChannel(0);
    delay(200);
    u8x8.setCursor(3, 2);
    u8x8.print("             ");
    u8x8.setCursor(3, 3);
    u8x8.print("             ");
    u8x8.setCursor(3, 4);
    u8x8.print("             ");
    u8x8.setCursor(3, 2);
    u8x8.print(KX126_Accel_X_OUT, 2);
    u8x8.setCursor(3, 3);
    u8x8.print(KX126_Accel_Y_OUT, 2);
    u8x8.setCursor(3, 4);
    u8x8.print(KX126_Accel_Z_OUT, 2);
    delay(200);
  }
  avanzar = false;

  u8x8.clear();
  u8x8.draw1x2String(0, 0, "BME280 TEST");
  u8x8.drawString(0, 2, "Temp: ");
  u8x8.drawString(0, 3, "Hum: ");
  u8x8.drawString(0, 4, "Pres: ");
  u8x8.drawString(0, 5, "Press D3 button");
  u8x8.drawString(0, 6, "to continue.");
  while (!debounce) {}
  delay(25);
  debounce = false;
  attachInterrupt(digitalPinToInterrupt(3), pulsar, FALLING);
  while (!avanzar) {
    float temperatura, presion, humedad;
    i2c.changeChannel(1);
    delay(100);
    bme.begin(0x76);
    delay(200);
    temperatura = bme.readTemperature();
    humedad = bme.readHumidity();
    presion = bme.readPressure();
    i2c.changeChannel(0);
    delay(100);
    u8x8.setCursor(7, 2);
    u8x8.print("         ");
    u8x8.setCursor(7, 2);
    u8x8.print(temperatura, 2);
    u8x8.setCursor(7, 3);
    u8x8.print("         ");
    u8x8.setCursor(7, 3);
    u8x8.print(humedad, 1);
    u8x8.setCursor(7, 4);
    u8x8.print("         ");
    u8x8.setCursor(7, 4);
    u8x8.print(presion, 0);
    delay(200);
  }
  avanzar = false;

  u8x8.clear();
  u8x8.draw1x2String(0, 0, "POWER VOLT TEST");
  u8x8.drawString(0, 2, "Voltage: ");
  u8x8.drawString(0, 5, "Press D3 button");
  u8x8.drawString(0, 6, "to continue.");
  while (!debounce) {}
  delay(25);
  debounce = false;
  attachInterrupt(digitalPinToInterrupt(3), pulsar, FALLING);
  while (!avanzar) {
    int batt = (int)(readVcc() / 100);  // readVCC returns  mVolt need just 100mVolt steps
    byte batvalue = (byte)batt; // no problem putting it into a int.
    u8x8.setCursor(9, 2);
    u8x8.print("       ");
    u8x8.setCursor(9, 2);
    u8x8.print((float)batvalue / 10, 1);
  }
  avanzar = false;

  u8x8.clear();
  u8x8.draw1x2String(0, 0, "SCT-013 TEST");
  u8x8.drawString(0, 2, "Power: ");
  u8x8.drawString(0, 5, "Press D3 button");
  u8x8.drawString(0, 6, "to continue.");
  //1V @ 30A
  emon1.current(A1, 30);
  while (!debounce) {}
  delay(25);
  debounce = false;
  attachInterrupt(digitalPinToInterrupt(3), pulsar, FALLING);
  while (!avanzar) {
    //Calcula la corriente
    double Irms = emon1.calcIrms(1480);
    u8x8.setCursor(7, 2);
    u8x8.print("         ");
    u8x8.setCursor(7, 2);
    u8x8.print(Irms * 230, 0);
    delay(200);
  }
  avanzar = false;
}
