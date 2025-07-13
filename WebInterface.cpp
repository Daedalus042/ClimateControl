
#include "Common.hpp"
#include "typeDefs.hpp"
#include "WebInterface.hpp"

#include <WiFiNINA.h>
#include <stdio.h>
#include <time.h>
#include <string>

using namespace std;

namespace online
{
void init()
{
    _ssid = mySsid;
    _password = myPassword;
    WifiStatus = WL_IDLE_STATUS;

    client = server.available();
    server.begin();

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
    printSerial("Fetch epoch from the network");

    for (int i = 0; i < 10; i++)
    {
        epoch = WiFi.getTime();
        printSerial(to_string(epoch));
        char lineout[50];
        time_t rawtime = epoch;
        struct tm * timeinfo;

        timeinfo = localtime(&rawtime);
        sprintf(lineout, "Time is %s UTC", asctime(timeinfo));
        printSerial(lineout);
        rawtime = rawtime + 60 * 60 * CDT;
        timeinfo = localtime(&rawtime);
        sprintf(lineout, "Time is %s CDT", asctime(timeinfo));
        printSerial(lineout);

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
        printSerial(to_string(rtc.getHours()).append(" -- ").append(to_string(rtc.getHours() + CDT)));
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

void updateWebClient() {
    client = server.available();
    if (client)
        printSerial("new client");           // print a message out the serial port
    else
        return;

    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
        if (client.available()) {             // if there's bytes to read from the client,
                char c = client.read();             // read a byte, then
                Serial.write(c);                    // print it out the serial monitor
                if (c == '\n') {                    // if the byte is a newline character

                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() == 0) {

                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                        // and a content-type so the client knows what's coming, then a blank line:
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println();

                        //create the links
                        client.print("Click <a href=\"/H\">here</a> turn the LED on<br>");
                        client.print("Click <a href=\"/L\">here</a> turn the LED off<br>");

                        int randomReading = analogRead(A1);
                        client.print("Random reading from analog pin: ");
                        client.print(randomReading);

                        // The HTTP response ends with another blank line:
                        client.println();
                        // break out of the while loop:
                        break;
                    }
                    else {      // if you got a newline, then clear currentLine:
                        currentLine = "";
                    }
                }
                else if (c != '\r') {    // if you got anything else but a carriage return character,
                    currentLine += c;      // add it to the end of the currentLine
                }

                if (currentLine.endsWith("GET /H")) {
                    blinkCode_num(reachedEndOfProgram, 2);
                }
                if (currentLine.endsWith("GET /L")) {
                    blinkCode_num(reachedEndOfProgram, 4);
                }

        }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
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
