#include "Environment.hpp"

void Climate::TwoPartSchedule(uint_t startTime1, float temp1, uint_t startTime2, float temp2) {
    if (startTime1 >= 86400 || startTime2 >= 86400) {
        blinkCode(timeOutOf24HrBounds);
    }
}