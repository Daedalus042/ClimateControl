#ifndef WebInterface_hpp
#define WebInterface_hpp

using namespace std;

#include <Arduino.h>
#include <WiFiNINA.h>
#include "typeDefs.hpp"
#include "private.hpp"
#include <string>
#include <unistd.h>

class EpochClass
{
public:
    EpochClass();
    EpochClass(string ssid, string password);
    ~EpochClass();
    void getEpoch();
private:
    bool connect();
    bool disconnect();
    int WifiStatus;
    string _ssid;
    string _password;
};

#endif // WebInterface_hpp