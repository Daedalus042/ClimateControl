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
private:
    bool connect();
    bool disconnect();
    int WifiStatus;
    string _ssid;
    string _password;
    RTCZero rtc = RTCZero();
};

#endif // WebInterface_hpp
