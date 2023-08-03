#ifndef WebInterface_hpp
#define WebInterface_hpp

using namespace std;

#include <Arduino.h>
#include <WiFiNINA.h>
#include "typeDefs.hpp"
#include "private.hpp"
#include <RTCZero.h>
#include <string>

class EpochClass
{
public:
    EpochClass();
    EpochClass(string ssid, string password);
    ~EpochClass();
    void fetchEpoch();
    ulong_t getEpoch();
    void setAlarm(timeType time);
private:
    bool connect();
    bool disconnect();
    int WifiStatus;
    string _ssid;
    string _password;
    static RTCZero rtc();
};

extern EpochClass schedule;

#endif // WebInterface_hpp
