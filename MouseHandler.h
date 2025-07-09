#pragma once
#include "SDL.h"
#include "Debug.h"
#include "TimeHandler.h"

enum class ButtonState
{
    Pressed = 0,
    Released = 1,
    Held = 2,

    TotalButtonStates = 3
};
using enum ButtonState;

enum class ButtonList
{
    LeftButton = 0,
    RightButton = 1,
    MiddleButton = 2,

    TotalButtons = 3
};
using enum ButtonList;

class MouseHandler
{
public:
    MouseHandler();
    ~MouseHandler()=default; 

    void handleEvents(SDL_Event& e);

    bool isMouseInside(SDL_Rect targetRect) const;
    ButtonState getButtonState(ButtonList button) const;
    SDL_Point getMousePos() const;

    void setMouseHoldDelay(float delay);

    // Needs to run everyframe
    void updateState();

private:
    void updateHoldTimers();
    void updateMousePos();

    SDL_Point m_MousePos;

    ButtonState m_Buttons[(int)TotalButtons];
    float m_HoldTimer[(int)TotalButtons];
    float m_HoldDelay;

    float m_LastTick;
    TimeHandler m_Time;

};
