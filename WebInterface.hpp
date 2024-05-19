#ifndef WebInterface_hpp
#define WebInterface_hpp

using namespace std;

#include <Arduino.h>
#include <WiFiNINA.h>
#include "typeDefs.hpp"
#include "private.hpp"
#include <RTCZero.h>
#include <string>

namespace online
{
    // public
    void init();
    void init(string ssid, string password);
    void end();
    void fetchEpoch();
    ulong_t getEpoch();
    void setAlarm(uint8_t HH, uint8_t MM, uint8_t SS);
    void setAlarm(timeType time);

    // Private
    static bool connect();
    static bool disconnect();
    static int WifiStatus;
    static string _ssid;
    static string _password;
    static RTCZero rtc;
};

#endif // WebInterface_hpp
