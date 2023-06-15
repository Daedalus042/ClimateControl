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
    void remoteStart();
    void remoteStop();
    void sendTemperature();
    void sendTemperatureFaren(uint temp);
    void sendTemperatureCelc(uint temp);
private:
    uchar_t pin;
    const ushort_t address = 0xF508;
    void zero();
    void one();
    void byte(uchar_t data);
    void halfWord(ushort_t data);
    void init();
    void stop();
    void command(uchar_t data);
};

#endif // AppCont_hpp
