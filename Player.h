#pragma once
#include <SDL.h>


#include "Window.h"
#include "Debug.h"
#include <string>
#include <sstream>

#include "TimeHandler.h"


class Player
{
public:
    Player(int posX = 0, int posY = 0, float velocity = 5.0f);
    ~Player();


    void handleEvent(SDL_Event& e);
    // Ball position and vellocity only needed for automated movement
    void move(int borderGirth, bool automated = false, SDL_Point ballPos = { 0, 0 }, float ballVelocity = 0.0f );
    void setSize(int height, int width);

    SDL_Point getPosition() const;
    void setVelocity(float velocity);

private:

  
    SDL_Point m_Position;
    int m_Height;
    int m_Width;

    float m_Velocity;
    float m_VelocityY;
    float m_VelocityX;

    TimeHandler m_Time;
    float m_LastTick;

};

