
#include "Common.hpp"

void blinkCode(ushort_t xx)
{
    while (true)
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
}
