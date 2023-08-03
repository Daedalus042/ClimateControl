#ifndef Environment_hpp
#define Environment_hpp

#include "typeDefs.hpp"
#include "Common.hpp"
#include <vector>

namespace Climate
{
void twoPartSchedule(uint_t startTime1, float temp1, uint_t startTime2, float temp2, uint_t endTime);

void updateClimateTarget();

struct ScheduleBlockType
{
    float temp;
    uint_t endTime;
};

static int curIndex = 0;  ///< Index of the current schedule 
}; // Climate

#endif // Environment_hpp
