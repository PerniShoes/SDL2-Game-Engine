#include "TimeHandler.h"


TimeHandler::TimeHandler()
    :m_TimePaused{ 0 }, m_FullTimePassed{""}
{

}

TimeHandler::~TimeHandler() 
{

}


void TimeHandler::startTime()
{
    // Split into start pause restart time and so on

}


const Uint64 TimeHandler::getTimePassedInMs()
{
    return (SDL_GetTicks64()) - m_TimePaused;
}

const Uint64 TimeHandler::getTimePassedInSec()
{  
    return ((SDL_GetTicks64() - m_TimePaused) / 1000);
}

const Uint64 TimeHandler::getTimePassedInMin()
{
    return ((SDL_GetTicks64() - m_TimePaused) / 60000);
}

const Uint64 TimeHandler::getTimePassedInHr()
{
    return ((SDL_GetTicks64() - m_TimePaused) / 3600000);
}

const std::string TimeHandler::getTimePassedFull()
{
    std::string miliseconds;
    std::string seconds;
    std::string minutes;
    std::string hours;
    std::string space{ "" };


    if (((SDL_GetTicks64() - m_TimePaused) / 3600000 % 60)  >= 10)
    {
        hours = std::to_string(SDL_GetTicks64() / 3600000 % 60) + ":";
    }
    else if (((SDL_GetTicks64() - m_TimePaused) / 3600000 % 60) >= 1)
    {
        hours = "0" + std::to_string((SDL_GetTicks64() - m_TimePaused) / 3600000 % 60) + ":";
    }
    else
    {
        hours = "";
    }

    if (((SDL_GetTicks64() - m_TimePaused) / 60000 % 60) >= 10)
    {
        minutes = std::to_string((SDL_GetTicks64() - m_TimePaused) / 60000 % 60) + ":";
    }
    else if (((SDL_GetTicks64() - m_TimePaused) / 60000 % 60) >= 1)
    {
        minutes = "0" + std::to_string((SDL_GetTicks64() - m_TimePaused) / 60000 % 60) + ":";
    }
    else
    {
        minutes ="";
    }
   
    if (((SDL_GetTicks64() - m_TimePaused) / 1000 % 60) >= 10)
    {
        seconds = std::to_string((SDL_GetTicks64() - m_TimePaused) / 1000 % 60);
    }
    else
    {
        seconds = "0" + std::to_string((SDL_GetTicks64() - m_TimePaused) / 1000 % 60);
    }


    miliseconds = ":" + std::to_string(((SDL_GetTicks64() - m_TimePaused) % 1000) / 10);


    if (((SDL_GetTicks64() - m_TimePaused) / 3600000) >= 1)
    {
        space = "";
    }
    else if (((SDL_GetTicks64() - m_TimePaused) / 60000) >= 1)
    {
        space = "     ";
    }
    else
    {
        space = "          ";
    }


    m_FullTimePassed = space + hours + minutes + seconds + miliseconds;


    return m_FullTimePassed;
}

void TimeHandler::restartTime()
{
    m_TimePaused = SDL_GetTicks64();

}