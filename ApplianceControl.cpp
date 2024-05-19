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
    powerIsOn = false;
}

FriAc::FriAc(uchar_t pin){
    this->pin = pin;
    powerIsOn = false;
}

void FriAc::syncPower(bool powerState)
{
    powerIsOn = powerState;
}

bool FriAc::isPowerOn()
{
    return powerIsOn;
}

void FriAc::powerOn()
{
    if (!powerIsOn)
    {
        togglePower();
        powerIsOn = true;
    }
}

void FriAc::powerOff()
{
    if (powerIsOn)
    {
        togglePower();
        powerIsOn = false;
    }
}

void FriAc::togglePower()
{
    command(PowerToggle);
    powerIsOn = !powerIsOn;
}

void FriAc::tempTimerUp()
{
    command(TempOrTimerUp);
}

void FriAc::tempTimerDown()
{
    command(TempOrTimerDown);
}

void FriAc::fanUp()
{
    command(FanUp);
}

void FriAc::fanDown()
{
    command(FanDown);
}

void FriAc::swapFarenCelc()
{
    command(NotDefined);
    // Need to collect this one (IF exists)
}

void FriAc::cool()
{
    command(Cool);
}

void FriAc::energySaver()
{
    command(EnergySaver);
}

void FriAc::fanOnly()
{
    command(FanOnly);
}

void FriAc::sleep()
{
    command(Sleep);
}

void FriAc::autoFan()
{
    command(AutoFan);
}

void FriAc::timer()
{
    command(Timer);
}

void FriAc::remoteStart(uint8_t temp)
{
    longCommand(RemoteStart, temp);
}

void FriAc::remoteStartFaren(float temp)
{
    // TODO: determine what the CRC/mystery bit is and implement here
    // bool crc = temp % 1;

    // Note: Temperature is sent MSB first, opposite to how everything else is sent
    uint8_t adjustedTemp = (temp - 58) * 2;
    uint8_t formattedTemp = 0;
    for (uint8_t i = 0; i < 7; i++)
    {
        formattedTemp += (adjustedTemp & (1 << i)) << (7 - i);
    }
    // formattedTemp |= crc;

    longCommand(RemoteStart, formattedTemp);
}

void FriAc::remoteStartCels(float temp)
{

}

void FriAc::remoteStop()
{
    command(RemoteStop);
}

void FriAc::sendTemperature(uint8_t temp)
{
    longCommand(SendTemp, temp);
}

void FriAc::sendTemperatureFaren(float temp)
{
    // TODO: determine what the CRC/mystery bit is and implement here
    // bool crc = temp % 1;

    // Note: Temperature is sent MSB first, opposite to how everything else is sent
    uint8_t adjustedTemp = (temp - 58) * 2;
    uint8_t formattedTemp = 0;
    for (uint8_t i = 0; i < 7; i++)
    {
        formattedTemp += (adjustedTemp & (1 << i)) << (7 - i);
    }
    // formattedTemp |= crc;

    longCommand(SendTemp, formattedTemp);
}

void FriAc::sendTemperatureCels(float temp)
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

void FriAc::command(commands cmd)
{
    if ( cmd == NotDefined )
    {
        return;
    }
    init();
    halfWord(address);
    byte(cmd);
    byte(~cmd);
    stop();
    delay(100);
}

void FriAc::longCommand(longCommands cmd, uint8_t data)
{
    init();
    halfWord(address);
    byte(cmd);
    byte(data);
    endDoubleWord();
    halfWord(address);
    byte(cmd);
    byte(data);
    stop();
    delay(100);
}

void FriAc::endDoubleWord()
{
    // Frigidair airconditioner indicate end of first double-word sequence
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
}

void FriAc::stop()
{
  // Frigidair airconditioner end sequence
  endDoubleWord();

  delayMicroseconds(96800);
  tone(pin, 37040);
  delayMicroseconds(8800);
  noTone(pin);
  delayMicroseconds(2200);
  tone(pin, 37040);
  delayMicroseconds(550);
  noTone(pin);
}

HonFan fan(IR_PIN);
FriAc ac(IR_PIN);
