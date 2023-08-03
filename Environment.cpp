#include "Environment.hpp"
#include "WebInterface.hpp"

void Climate::twoPartSchedule(uint_t startTime1, float temp1, uint_t startTime2, float temp2, uint_t endTime)
{
    if (startTime1 >= 86400 || startTime2 >= 86400)
    {
        blinkCode(timeOutOf24HrBounds);
    }

    std::vector<ScheduleBlockType> ScheduleBlock;
    /// If climate control doesn't start at midnight, add a down time filler
    if (startTime1 != 0)
    {
        ScheduleBlock.push_back(ScheduleBlockType{-1, startTime1});
    }
    ScheduleBlock.push_back(ScheduleBlockType{temp1, startTime2});
    ScheduleBlock.push_back(ScheduleBlockType{temp2, endTime});
    /// If climate control doesn't end at midnight, add a down time filler
    if (endTime != 86400)
    {
        ScheduleBlock.push_back(ScheduleBlockType{-1, 86400});
    }
}

void Climate::updateClimateTarget()
{
    timeType nextTrigger = secondsToTime(0);
    schedule.setAlarm(nextTrigger);
}
