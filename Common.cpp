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
    RgbLed.setStatic(16, 8, 0);
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
    RgbLed.setStatic(0, 0, 0);
}

RgbLedClass::RgbLedClass()
{
    this.init();
    setStatic(colorType{0, 0, 0});
}

RgbLedClass::RgbLedClass(colorType _color) {
    this.init();
    setStatic(_color);
}

RgbLedClass::RgbLedClass(uint8_t red, uint8_t green, uint8_t blue) : RgbLedClass(colorType{red, green, blue}) {}
// {
//     RgbLed::RgbLed()

//     WiFiDrv::analogWrite(25, color.red);
//     WiFiDrv::analogWrite(26, color.green);
//     WiFiDrv::analogWrite(27, color.blue);
// }

RgbLedClass::RgbLedClass(uint32_t hexCode)
{
    this.init();
    setStatic(hexCode);
}

RgbLedClass::~RgbLedClass()
{
    // Turn LED off
    setStatic(0);
}

void RgbLedClass::setStatic(colorType _color) {
    color = _color;
    isPulsing = false;
    isDimming = false;
    WiFiDrv::analogWrite(25, color.red);
    WiFiDrv::analogWrite(26, color.green);
    WiFiDrv::analogWrite(27, color.blue);
}

void RgbLedClass::setStatic(uint8_t red, uint8_t green, uint8_t blue) : setStatic(colorType{red, green, blue}) {}

void RgbLedClass::setStatic(uint32_t hexCode)
{
    color = colorType{(hexCode >> 0) & 0xFF, (hexCode >> 8) & 0xFF, (hexCode >> 16) & 0xFF}
    isPulsing = false;
    isDimming = false;
    WiFiDrv::analogWrite(25, color.red);
    WiFiDrv::analogWrite(26, color.green);
    WiFiDrv::analogWrite(27, color.blue);
}

void RgbLedClass::setBreathing(uint8_t red, uint8_t green, uint8_t blue)
{
    color = _color;
    isPulsing = true;
    isDimming = true;
    WiFiDrv::analogWrite(25, color.red);
    WiFiDrv::analogWrite(26, color.green);
    WiFiDrv::analogWrite(27, color.blue);
}

void RgbLedClass::setBreathing(colorType color) : setBreathing(colorType{red, green, blue}) {}

void RgbLedClass::setBreathing(uint32_t hexCode)
{
    color = colorType{(hexCode >> 0) & 0xFF, (hexCode >> 8) & 0xFF, (hexCode >> 16) & 0xFF}
    isPulsing = true;
    isDimming = true;
    WiFiDrv::analogWrite(25, color.red);
    WiFiDrv::analogWrite(26, color.green);
    WiFiDrv::analogWrite(27, color.blue);
}

void RgbLedClass::serviceBreathing()
{
    colorType instant{color.red * brightness, color.green * brightness, color.blue * brightness};
    if (isDimming)
    {
        brightness -= 0.0501;
        if (brightness <= 0)
        {
            isDimming = false;
            brightness = 0;
        }
    }
    else
    {
        brightness += 0.0501;
        if (brightness >= 1)
        {
            isDimming = true;
            brightness = 1;
        }
    }
    WiFiDrv::analogWrite(25, instant.red);
    WiFiDrv::analogWrite(26, instant.green);
    WiFiDrv::analogWrite(27, instant.blue);
}

void RgbLedClass::init()
{
    WiFiDrv::pinMode(25, OUTPUT);  ///< Red LED
    WiFiDrv::pinMode(26, OUTPUT);  ///< Green LED
    WiFiDrv::pinMode(27, OUTPUT);  ///< Blue LED
}

timeType secondsToTime(uint_t time)
{
    ushort_t hrs = time / 3600;
    time -= hrs * 3600;
    ushort_t mins = time / 60;
    time -= mins * 60;
    return timeType{(hrs / 3600), (mins / 60), (ushort_t) time};
}
