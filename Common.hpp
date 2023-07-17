#ifndef Common_hpp
#define Common_hpp

#include <Arduino.h>
#include "typeDefs.hpp"

void blinkCode(ushort_t xx);
void blinkCode_ms(ushort_t xx, uint_t timeout_ms);
void blinkCode_num(ushort_t xx, uint_t count);

const ushort_t ntpUnreachable = 0x22;
const ushort_t timeOutOf24HrBounds = 0x23;
const ushort_t noWiFiModuleFound = 0x32;
const ushort_t WifiFirmwareUpgradeNeeded = 0x33;
const ushort_t WifiNotConnected = 0x34;
const ushort_t noTemperatureSensorFound = 0x42;
const ushort_t reachedEndOfProgram = 0x52;

#endif // Common_hpp
