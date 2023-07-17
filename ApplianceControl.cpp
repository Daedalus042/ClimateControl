#include "ApplianceControl.hpp"
#include <Arduino.h>
#include "typeDefs.hpp"

HonFan::HonFan()
{
    pin = 1;
}

HonFan::HonFan(uchar_t pin)
{
    this->pin = pin;
}

void HonFan::togglePower()
{
    command(0x4A);
}

void HonFan::changeSpeed()
{
    command(0xC4);
}

void HonFan::changePattern()
{
    command(0x89);
}

void HonFan::changeTimer()
{
    command(0x15);
}

void HonFan::toggleOscillation()
{
    command(0x38);
}

void HonFan::zero()
{
  // Honeywell fan zero
  tone(pin, 37040);
  delayMicroseconds(400);
  noTone(pin);
  delayMicroseconds(1600);
}

void HonFan::one()
{
  // Honeywell fan one
  tone(pin, 37040);
  delayMicroseconds(400);
  noTone(pin);
  delayMicroseconds(2400);
}

void HonFan::byte(uchar_t data)
{
    for (int i = 0; i < 8; i++)
    {
        if ((data >> i) & 1)
        {
            one();
        }
        else
        {
            zero();
        }
    }
}

void HonFan::init()
{
  // Fan alert
  tone(pin, 37040);
  delayMicroseconds(4400);
  noTone(pin);
  delayMicroseconds(4400);
}

void HonFan::command(uchar_t data)
{
    init();
    byte(address);
    byte(data);
    zero();
    init();
    byte(address);
    byte(data);
    tone(pin, 37040);
    delayMicroseconds(400);
    stop();
    delay(100);
}

void HonFan::stop()
{
  // Honeywell fan end sequence
  noTone(pin);
  delayMicroseconds(14000);
  tone(pin, 37040);
  delayMicroseconds(8900);
  noTone(pin);
  delayMicroseconds(2200);
  tone(pin, 37040);
  delayMicroseconds(400);
  noTone(pin);
  delayMicroseconds(14000);
  tone(pin, 37040);
  delayMicroseconds(8900);
  noTone(pin);
  delayMicroseconds(2200);
  tone(pin, 37040);
  delayMicroseconds(400);
  noTone(pin);
}

void HonFan::blink()
{
    digitalWrite(pin, HIGH);
    delay(250);
    digitalWrite(pin, LOW);
    delay(250);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

FriAc::FriAc(){
    pin = 1;
}

FriAc::FriAc(uchar_t pin){
    this->pin = pin;
}

void FriAc::togglePower()
{
    command(0x11);
}

void FriAc::tempTimerUp()
{
    command(0x0E);
}

void FriAc::tempTimerDown()
{
    command(0x0D);
}

void FriAc::fanUp()
{
    command(0x01);
}

void FriAc::fanDown()
{
    command(0x04);
}

void FriAc::swapFarenCelc()
{
    command(-1);
    // Need to collect this one
}

void FriAc::cool()
{
    command(0x09);
}

void FriAc::energySaver()
{
    command(0x02);
}

void FriAc::fanOnly()
{
    command(0x07);
}

void FriAc::sleep()
{
    command(0x00);
}

void FriAc::autoFan()
{
    command(0x0F);
}

void FriAc::timer()
{
    command(0x06);
}

void FriAc::remoteStart()
{
    // init();
    // halfWord(address);
    // byte(data);
    // byte(~data);
    // stop();
    delay(100);
}

void FriAc::remoteStop()
{
    command(0x7F);
}

void FriAc::sendTemperature()
{
}

void FriAc::sendTemperatureFaren(uint_t temp)
{
}

void FriAc::sendTemperatureCelc(uint_t temp)
{
}

void FriAc::zero()
{
  // Frigidair airconditioner zero
  tone(pin, 37040);
  delayMicroseconds(550);
  noTone(pin);
  delayMicroseconds(550);
}

void FriAc::one()
{
  // Frigidair airconditioner one
  tone(pin, 37040);
  delayMicroseconds(550);
  noTone(pin);
  delayMicroseconds(1700);
}

void FriAc::byte(uchar_t data)
{
    for (int i = 0; i < 8; i++)
    {
        if ((data >> i) & 1)
        {
            one();
        }
        else
        {
            zero();
        }
    }
}

void FriAc::halfWord(ushort_t data)
{
    for (int i = 0; i < 16; i++)
    {
        if ((data >> i) & 1)
        {
            one();
        }
        else
        {
            zero();
        }
    }
}

void FriAc::init()
{
  // Frigidair airconditioner alert
  tone(pin, 37040);
  delayMicroseconds(8800);
  noTone(pin);
  delayMicroseconds(4400);
}

void FriAc::command(uchar_t data)
{
    init();
    halfWord(address);
    byte(data);
    byte(~data);
    stop();
    delay(100);
}

void FriAc::stop()
{
  // Frigidair airconditioner end sequence
  tone(pin, 37040);
  delayMicroseconds(550);
  noTone(pin);
  delayMicroseconds(41800);
  tone(pin, 37040);
  delayMicroseconds(8800);
  noTone(pin);
  delayMicroseconds(2200);
  tone(pin, 37040);
  delayMicroseconds(550);
  noTone(pin);

  delayMicroseconds(96800);
  tone(pin, 37040);
  delayMicroseconds(8800);
  noTone(pin);
  delayMicroseconds(2200);
  tone(pin, 37040);
  delayMicroseconds(550);
  noTone(pin);
}
