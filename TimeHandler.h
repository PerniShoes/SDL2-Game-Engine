#pragma once // NOLINT
#include <SDL.h>

#include "Debug.h"
#include <string>
#include <sstream>
//#include <format>

class TimeHandler
{
public:
    TimeHandler();
    ~TimeHandler();

    TimeHandler(const TimeHandler&) = delete;
    TimeHandler& operator = (const TimeHandler&) = delete;

    void startTime();

    // Add day functionality xD????
    const Uint64 getTimePassedInMs();
    const Uint64 getTimePassedInSec();
    const Uint64 getTimePassedInMin();
    const Uint64 getTimePassedInHr(); 

    const std::string getTimePassedFull();
    void restartTime();

private:
    
    Uint64 m_TimePaused; //Not used for now???
    std::string m_FullTimePassed;
};
