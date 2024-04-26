#ifndef Common_hpp
#define Common_hpp

#include <Arduino.h>
#include "typeDefs.hpp"

struct timeType
{
    ushort_t hour;
    ushort_t minute;
    ushort_t second;
};

void blinkCode(ushort_t xx);
void blinkCode_ms(ushort_t xx, uint_t timeout_ms);
void blinkCode_num(ushort_t xx, uint_t count);
timeType secondsToTime(uint_t time);

const ushort_t reachedEndOfProgram = 0x11;
const ushort_t ntpUnreachable = 0x22;
const ushort_t timeOutOf24HrBounds = 0x23;
const ushort_t noWiFiModuleFound = 0x32;
const ushort_t WifiFirmwareUpgradeNeeded = 0x33;
const ushort_t WifiNotConnected = 0x34;
const ushort_t noTemperatureSensorFound = 0x42;

#endif // Common_hpp
