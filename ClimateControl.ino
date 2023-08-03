#include "Adafruit_SHT4x.h"
#include "ApplianceControl.hpp"
#include "Common.hpp"
#include "typeDefs.hpp"
#include "WebInterface.hpp"

void setup()
{
  // put your setup code here, to run once:

#if Serial_Available
  Serial.begin(115200);
#endif
  pinMode(LED_BUILTIN, OUTPUT);

  // Setup temperature sensor
  Adafruit_SHT4x sht4 = Adafruit_SHT4x();
  if (!sht4.begin())
  {
    blinkCode(noTemperatureSensorFound);
  }
  sht4.setPrecision(SHT4X_HIGH_PRECISION);
  sht4.setHeater(SHT4X_NO_HEATER);

  for (int i = 0; i < 3; i++)
  {
    delay(1000);
  }

  pinMode(7, OUTPUT);
  HonFan fan(7);
  FriAc ac(7);
  fan.togglePower();
  ac.togglePower();

  schedule.fetchEpoch();

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
