
#include "Common.hpp"
#include <Arduino.h>
#include <string>
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>

void printSerial(std::string msg)
{
#if Serial_Available
  Serial.println(msg.c_str());
#endif
}

void blinkCode(ushort_t xx)
{
    // while (true)
    // {
        // blink slow
        for (int i = 0; i < (0xF0 & xx); i += 0x10)
        {
            digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
            delay(1000);                      // wait for a second
            digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
            delay(1000);
        }

        delay(1000);

        // blink fast
        for (int i = 0; i < (0x0F & xx); i++)
        {
            digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
            delay(250);                       // wait for a quarter second
            digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
            delay(250);
        }

        delay(1750);
    // }
}

void blinkCode_ms(ushort_t xx, uint_t timeout_ms)
{
    uint_t starttime = millis();
    while (millis() < (starttime + timeout_ms))
    {
        // blink slow
        for (int i = 0; i < (0xF0 & xx); i += 0x10)
        {
            digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
            delay(1000);                      // wait for a second
            digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
            delay(1000);
        }

        delay(1000);

        // blink fast
        for (int i = 0; i < (0x0F & xx); i++)
        {
            digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
            delay(250);                       // wait for a quarter second
            digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
            delay(250);
        }

        delay(1750);
    }
}

void blinkCode_num(ushort_t xx, uint_t count)
{
    rgbLight(16, 8, 0);
    for (int i = 0; i < count; i++)
    {
        // blink slow
        for (int i = 0; i < (0xF0 & xx); i += 0x10)
        {
            digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
            delay(1000);                      // wait for a second
            digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
            delay(1000);
        }

        delay(1000);

        // blink fast
        for (int i = 0; i < (0x0F & xx); i++)
        {
            digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
            delay(250);                       // wait for a quarter second
            digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
            delay(250);
        }

        delay(1750);
    }
        rgbLight(0, 0, 0);
}

void setupRgbLight()
{
    WiFiDrv::pinMode(25, OUTPUT);  ///< Red LED
    WiFiDrv::pinMode(26, OUTPUT);  ///< Green LED
    WiFiDrv::pinMode(27, OUTPUT);  ///< Blue LED
}

void rgbLight(ushort_t red, ushort_t green, ushort_t blue)
{
    WiFiDrv::analogWrite(25, red);
    WiFiDrv::analogWrite(26, green);
    WiFiDrv::analogWrite(27, blue);
}

timeType secondsToTime(uint_t time)
{
    ushort_t hrs = time / 3600;
    time -= hrs * 3600;
    ushort_t mins = time / 60;
    time -= mins * 60;
    return timeType{(hrs / 3600), (mins / 60), (ushort_t) time};
}
