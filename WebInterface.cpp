
#include "Common.hpp"
#include "typeDefs.hpp"
#include "WebInterface.hpp"

#include <stdio.h>
#include <time.h>

using namespace std;

namespace online
{
void init()
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

    rtc = RTCZero();
    rtc.begin();
}

void init(string ssid, string password)
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

    rtc = RTCZero();
    rtc.begin();
}

void end()
{
    WiFi.end();
}

void fetchEpoch()
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
        char lineout[50];
        time_t rawtime = epoch;
        struct tm * timeinfo;

        timeinfo = localtime(&rawtime);
        sprintf(lineout, "Time is %s UTC", asctime(timeinfo));
        Serial.println(lineout);
        rawtime = rawtime + 60 * 60 * CDT;
        timeinfo = localtime(&rawtime);
        sprintf(lineout, "Time is %s CDT", asctime(timeinfo));
        Serial.println(lineout);
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

ulong_t getEpoch()
{
    return rtc.getEpoch();
}

void setAlarm(uint8_t HH, uint8_t MM, uint8_t SS)
{
    rtc.setAlarmTime(HH, MM, SS);
}

void setAlarm(timeType time)
{
    rtc.setAlarmTime(time.hour, time.minute, time.second);
}

bool connect()
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

bool disconnect()
{
    WiFi.disconnect();
}
}
