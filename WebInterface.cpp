
#include "Common.hpp"
#include "WebInterface.hpp"

using namespace std;

EpochClass::EpochClass() {
    _ssid = mySsid;
    _password = myPassword;
    int WifiStatus = WL_IDLE_STATUS;

    WiFiClient client;

    if (WiFi.status() == WL_NO_MODULE) {
        blinkCode(noWiFiModuleFound);
    }
    
    string fv = <string> WiFi.firmwareVersion();
    if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
        blinkCode(WifiFirmwareUpgradeNeeded);
    }
}

EpochClass::EpochClass(string ssid, string password) {
    _ssid = ssid;
    _password = password;
    int WifiStatus = WL_IDLE_STATUS;

    WiFiClient client;

    if (WiFi.status() == WL_NO_MODULE) {
        blinkCode(noWiFiModuleFound);
    }
    
    string fv = <string> WiFi.firmwareVersion();
    if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
        blinkCode(WifiFirmwareUpgradeNeeded);
    }
}

EpochClass::~EpochClass() {
    WiFi.end();
}

void EpochClass::getEpoch() {
    connect();

    disconnect();
}

bool EpochClass::connect() {
    for (int i = 0; (i < 10) && (WifiStatus != WL_CONNECTED); i++) {
        WifiStatus = WiFi.begin(_ssid, _password);
        blinkCode_num(WifiNotConnected, 3);
        wait(10);
    }

    if (WifiStatus != WL_CONNECTED) {
        blinkCode_ms(WifiNotConnected, 180000);
        return false;
    } else {
        return true;
    }
}

bool EpochClass::disconnect() {
    WiFi.disconnect();
}