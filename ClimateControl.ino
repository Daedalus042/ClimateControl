#include "ApplianceControl.hpp"

#include "Adafruit_SHT4x.h"
#include "Common.hpp"

void blinkCode(ushort_t xx);

void setup()
{
  // put your setup code here, to run once:

  // Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  // Setup temperature sensor
  Adafruit_SHT4x sht4 = Adafruit_SHT4x();
  if (!sht4.begin())
  {
    while (true)
    {
      digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
      delay(250);                      // wait for a second
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
      delay(250);
    }
  }
  sht4.setPrecision(SHT4X_HIGH_PRECISION);
  sht4.setHeater(SHT4X_NO_HEATER);

  for (int i = 0; i < 3; i++)
  {
    // Serial.println("test");
    delay(1000);
  }

  pinMode(7, OUTPUT);
  HonFan fan(7);
  FriAc ac(7);
  fan.togglePower();
  ac.togglePower();

  sensors_event_t humidity, temp;
  sht4.getEvent(&humidity, &temp); // populate temp and humidity objects with fresh data
  ac.sendTemperatureCelc(temp.temperature);
  humidity.relative_humidity;

  delay(1000);
  fan.togglePower();
  delay(1000);
  ac.togglePower();
  for (int i = 0; i < 6; i++)
  {
    fan.blink();
  }
}

void loop()
{
  blinkCode(reachedEndOfProgram);
}
