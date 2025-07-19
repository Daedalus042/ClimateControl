#ifndef Common_hpp
#define Common_hpp

#include <Arduino.h>
#include "typeDefs.hpp"
#include <string>
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>

struct timeType
{
    ushort_t hour;
    ushort_t minute;
    ushort_t second;
};

struct colorType
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

void printSerial(std::string msg);

void blinkCode(ushort_t xx);
void blinkCode_ms(ushort_t xx, uint_t timeout_ms);
void blinkCode_num(ushort_t xx, uint_t count);

class RgbLedClass {
public:
    RgbLedClass();
    RgbLedClass(colorType _color);
    RgbLedClass(uint8_t red, uint8_t green, uint8_t blue);
    RgbLedClass(uint32_t hexCode);
    ~RgbLedClass();
    void setStatic(colorType _color);
    void setStatic(uint8_t red, uint8_t green, uint8_t blue);
    void setStatic(uint32_t hexCode);
    void setBreathing(colorType _color);
    void setBreathing(uint8_t red, uint8_t green, uint8_t blue);
    void setBreathing(uint32_t hexCode);
    void serviceBreathing();

private:
    static colorType color;
    static bool isPulsing;
    static bool isDimming;
    static float brightness;
    void inline init();
};

timeType secondsToTime(uint_t time);
extern RgbLedClass RgbLed();

const ushort_t reachedEndOfProgram = 0x11;
const ushort_t ntpUnreachable = 0x22;
const ushort_t timeOutOf24HrBounds = 0x23;
const ushort_t noWiFiModuleFound = 0x32;
const ushort_t WifiFirmwareUpgradeNeeded = 0x33;
const ushort_t WifiNotConnected = 0x34;
const ushort_t noTemperatureSensorFound = 0x42;

#endif // Common_hpp
