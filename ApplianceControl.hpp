#ifndef AppCont_hpp
#define AppCont_hpp

#include <Arduino.h>
#include "typeDefs.hpp"

class HonFan {
public:
    HonFan();
    HonFan(uchar_t pin);
    void togglePower();
    void changeSpeed();
    void changePattern();
    void changeTimer();
    void toggleOscillation();
    void blink();
private:
    uchar_t pin;
    const uchar_t address = 0x40;
    void zero();
    void one();
    void byte(uchar_t data);
    void init();
    void stop();
    void command(uchar_t data);
};

class FriAc {
public:
    FriAc();
    FriAc(uchar_t pin);
    void syncPower(bool powerState);
    bool isPowerOn();
    void powerOn();
    void powerOff();
    void togglePower();
    void tempTimerUp();
    void tempTimerDown();
    void fanUp();
    void fanDown();
    void swapFarenCelc();
    void cool();
    void energySaver();
    void fanOnly();
    void sleep();
    void autoFan();
    void timer();
    void remoteStartFaren(float temp);
    void remoteStartCels(float temp);
    void remoteStop();
    void sendTemperatureFaren(float temp);
    void sendTemperatureCels(float temp);

private:
    enum commands { PowerToggle =     0x11,
                            TempOrTimerUp =   0x0E,
                            TempOrTimerDown = 0x0D,
                            FanUp =           0x01,
                            FanDown =         0x04,
                            Cool =            0x09,
                            EnergySaver =     0x02,
                            FanOnly =         0x07,
                            Sleep =           0x00,
                            AutoFan =         0x0F,
                            Timer =           0x06,
                            RemoteStop =      0xEF,
                            NotDefined =      -1 };
    enum longCommands { RemoteStart = 0xAF,
                                SendTemp =    0x6F };
    // Sleep           = 0x00  |  0x_0 =
    // FanUp           = 0x01  |  0x11 = PowerToggle
    // EnergySaver     = 0x02  |  0x_2 =
    //                 = 0x03  |  0x_3 =
    // FanDown         = 0x04  |  0x_4 =
    //                 = 0x05  |  0x_5 =
    // Timer           = 0x06  |  0x_6 =
    // FanOnly         = 0x07  |  0x_7 =
    //                 = 0x08  |  0x_8 =
    // Cool            = 0x09  |  0x_9 =
    //                 = 0x0A  |  0x_A =
    //                 = 0x0B  |  0x_B =
    //                 = 0x0C  |  0x_C =
    // TempOrTimerDown = 0x0D  |  0x_D =
    // TempOrTimerUp   = 0x0E  |  0x_E =
    // AutoFan         = 0x0F  |  0xEF = RemoteStop

    uchar_t pin;
    bool powerIsOn;
    const ushort_t address = 0xF508;
    void zero();
    void one();
    void byte(uchar_t data);
    void halfWord(ushort_t data);
    void init();
    void endDoubleWord();
    void stop();
    void command(commands cmd);
    void longCommand(longCommands cmd, uint8_t temperatureData);
    void remoteStart(uint8_t temp);
    void sendTemperature(uint8_t temp);
};

extern HonFan fan;
extern FriAc ac;

#endif // AppCont_hpp
