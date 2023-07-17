
#include "Common.hpp"
#include "typeDefs.hpp"
#include "WebInterface.hpp"

using namespace std;

EpochClass::EpochClass()
{
    _ssid = mySsid;
    _password = myPassword;
    WifiStatus = WL_IDLE_STATUS;

    WiFiClient client;

    if (WiFi.status() == WL_NO_MODULE)
    {
        blinkCode(noWiFiModuleFound);
    }

    string fv = (string) WiFi.firmwareVersion();
    if (fv < WIFI_FIRMWARE_LATEST_VERSION)
    {
        blinkCode(WifiFirmwareUpgradeNeeded);
    }

    rtc.begin();
}

EpochClass::EpochClass(string ssid, string password)
{
    _ssid = ssid;
    _password = password;
    WifiStatus = WL_IDLE_STATUS;

    WiFiClient client;

    if (WiFi.status() == WL_NO_MODULE)
    {
        blinkCode(noWiFiModuleFound);
    }

    string fv = (string) WiFi.firmwareVersion();
    if (fv < WIFI_FIRMWARE_LATEST_VERSION)
    {
        blinkCode(WifiFirmwareUpgradeNeeded);
    }

    rtc.begin();
}

EpochClass::~EpochClass()
{
    WiFi.end();
}

void EpochClass::fetchEpoch()
{
    connect();

    ulong_t epoch;
#if Serial_Available
    Serial.println("fetch");
#endif

    for (int i = 0; i < 10; i++)
    {
        epoch = WiFi.getTime();
#if Serial_Available
        Serial.println(epoch);
#endif

        if (epoch != 0) { break; }
    }

    if (epoch == 0)
    {
        blinkCode_num(ntpUnreachable, 5);
    }
    else
    {
        rtc.setEpoch(epoch);

#if Serial_Available
        Serial.print(rtc.getHours());
        Serial.print(" -- ");
        Serial.println(rtc.getHours() + CDT);
#endif
    }

    disconnect();
}

ulong_t EpochClass::getEpoch()
{
    return rtc.getEpoch();
}

bool EpochClass::connect()
{
    for (int i = 0; (i < 10) && (WifiStatus != WL_CONNECTED); i++)
    {
        WifiStatus = WiFi.begin(_ssid.c_str(), _password.c_str());
        blinkCode_num(WifiNotConnected, 3);
        delay(10000);
    }

    if (WifiStatus != WL_CONNECTED)
    {
        blinkCode_ms(WifiNotConnected, 180000);
        return false;
    }
    else
    {
        return true;
    }
}

bool EpochClass::disconnect()
{
    WiFi.disconnect();
}
