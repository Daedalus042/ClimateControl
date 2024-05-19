#include "Environment.hpp"
#include "ApplianceControl.hpp"
#include "WebInterface.hpp"

#include <time.h>

namespace Climate
{
void scheduledUpdate()
{
    ScheduleBlockType lastStatus = *curClimate;
    // Catch updating the day
    if (curClimate == weeklySchedule[today].end())
    {
        today = static_cast<days>((today + 1) % 7);
        curClimate = weeklySchedule[today].begin();
    }
    else
    {
        curClimate++;
    }

    if ( curClimate->temp != lastStatus.temp && curClimate->temp != useLastTemperature )
    {
        updateTargetTemp(curClimate->temp, curClimate->temp - lastStatus.temp);
    }

    if ( curClimate->mode != lastStatus.mode && curClimate->mode != UsePreviousMode )
    {
        updateMode(curClimate->mode);
    }

    if ( curClimate->fanSpeed != lastStatus.fanSpeed && curClimate->fanSpeed != UsePreviousFan )
    {
        updateFan(curClimate->fanSpeed);
    }

    if (curClimate == weeklySchedule[today].end())
    {
        time_t baseTime = online::getEpoch();
        baseTime += 60 * 60 * CDT;
        time_t nextDay = baseTime + (60 * 60 * 24);  ///< Allow for accounting for 1st day of month wrapping using the struct
        struct tm * formattedBaseTime = localtime(&baseTime);
        struct tm * formattedNextDay = localtime(&nextDay);
        // setAlarmDay(formattedNextDay->tm_mday)
        online::setAlarm(19, 0, 0);
    }
    else
    {
        auto nextEvent = curClimate;
        nextEvent++;
        time_t nextAlarmRaw = nextEvent->startTime - CDT;
        struct tm * nextAlarm = localtime(&nextAlarmRaw);
        online::setAlarm(nextAlarm->tm_hour, nextAlarm->tm_min, nextAlarm->tm_sec);
    }
}

void updateTargetTemp(float temp, float delta)
{
    if constexpr (useCelsius)
        updateTargetTempC(temp, delta);
    else
        updateTargetTempF(temp, delta);
}

void updateTargetTempC(float temp, float delta)
{
    if (temp > 0)
        for (uint16_t i = 0; i < delta; i++)
        {
            ac.tempTimerUp();
        }
    else
        for (uint16_t i = 0; i < fabsf(delta); i++)
        {
            ac.tempTimerDown();
        }
}

void updateTargetTempF(float temp, float delta)
{
    if (temp > 0)
        for (uint16_t i = 0; i < delta; i++)
        {
            ac.tempTimerUp();
        }
    else
        for (uint16_t i = 0; i < fabsf(delta); i++)
        {
            ac.tempTimerDown();
        }
}

void updateMode(modes mode)
{
    switch (mode)
    {
    case PowerOff:
        ac.powerOff();
        break;
    case Cool:
        break;
    case EnergySaver:
        break;
    case FanOnly:
        break;
    default:
        break;
    }
}

void updateFan(fanSpeeds fanSpeed)
{

}

}

// void Climate::twoPartSchedule(uint_t startTime1, float temp1, uint_t startTime2, float temp2, uint_t endTime)
// {
//     if (startTime1 >= 86400 || startTime2 >= 86400)
//     {
//         blinkCode(timeOutOf24HrBounds);
//     }

//     std::vector<ScheduleBlockType> ScheduleBlock;
//     /// If climate control doesn't start at midnight, add a down time filler
//     if (startTime1 != 0)
//     {
//         ScheduleBlock.push_back(ScheduleBlockType{-1, startTime1});
//     }
//     ScheduleBlock.push_back(ScheduleBlockType{temp1, startTime2});
//     ScheduleBlock.push_back(ScheduleBlockType{temp2, endTime});
//     /// If climate control doesn't end at midnight, add a down time filler
//     if (endTime != 86400)
//     {
//         ScheduleBlock.push_back(ScheduleBlockType{-1, 86400});
//     }
// }

// void Climate::updateClimateTarget()
// {
//     // timeType nextTrigger = secondsToTime(0);
//     // schedule::setAlarm(nextTrigger);
// }
