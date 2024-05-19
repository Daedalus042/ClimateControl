#ifndef Environment_hpp
#define Environment_hpp

#include "typeDefs.hpp"
#include "Common.hpp"
#include <vector>

namespace Climate
{
// Public members

enum days : uint8_t { Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday };  ///< Days of the week to index to
enum modes { PowerOff, Cool, EnergySaver, FanOnly, UsePreviousMode = -1 };  ///< What mode to run the unit in
enum fanSpeeds { Auto, Low, Medium, High, UsePreviousFan = -1 };  ///< Fan speed to run the unit at
const float useLastTemperature = nanf(0);

void scheduledUpdate();

void updateTargetTemp(float temp, float delta);

void updateTargetTempC(float temp, float delta);

void updateTargetTempF(float temp, float delta);

void updateMode(modes mode);

void updateFan(fanSpeeds fanSpeed);

struct ScheduleBlockType
{
    float temp;
    time_t startTime;
    modes mode;
    fanSpeeds fanSpeed;
};

std::vector<ScheduleBlockType> weeklySchedule[7];  ///< The normal weekly schedule

ScheduleBlockType away[2];  ///< An away structure with a start and end date & time

static days today = Sunday;  ///< Current Day of the week
static auto curClimate = weeklySchedule[today].begin();  ///< Pointer to the currently active member
}; // Climate

#endif // Environment_hpp
