#pragma once
#include "SDL.h"
#include "Debug.h"
#include "TimeHandler.h"
#include "Window.h"
#include "Renderer.h"
#include <cstdlib>


enum class Sides
{
    NoCollision = -1, //NoCollision should probably be 0 (default)
    Left = 0,
    Right = 1,
    Bottom = 2,
    Top = 3,

    SidesTotal 
};

namespace CollisionLinesIds
{
    enum
    {
        // Collision detection assumes:
        // -Even amount of lines per side
        // -Lines in enum being in order: Left, Right, Top, Bottom

        // Vertical
        LeftTop = 0,
        LeftBottom = 1,

        RightTop = 2,
        RightBottom = 3,

        // Horizontal
        TopLeft = 4,
        TopRight = 5,

        BottomLeft = 6,
        BottomRight = 7,

        TotalLines

    };

}

enum class BallSoundEvents
{
    NoSoundEvent = -1,
    PaddleHit = 0,
    WallHit = 1,

    TotalBallSoundEvents = 2

};
using enum BallSoundEvents;

class Ball
{
    
public:
    Ball(SDL_Point startingPos = {0, 0}, int width = 20, int height = 20, float velocity = 5.0f, bool velocityCapped = true);
    ~Ball();


    void move(Sides startingCollisionVert = Sides::NoCollision, Sides startingCollisionHorizontal = Sides::NoCollision);

    void handleEvent(SDL_Event& e);

    void handleCollision(SDL_Rect* targetRect);
    
    void drawCollisionLines()const;
    void restartBall();

    void setSize(int height, int width);

    void setVelocity(float velocity); //Debug only, delete after

    void setVelocityMult(float mult = 1.005f); // How much ball speeds up on bounce (1.0f being none 1.05f being 5%)

    BallSoundEvents handleSoundEvents();

    // Debug
    void setPos(SDL_Point pos); 

    float getVelocity() const;
    SDL_Point getPosition() const;
    void setCollisionLines();

private:
    // Works with even amount of lines per side
    void detectCollision(SDL_Rect* targetRect);
   
    SDL_Rect m_BallRect;

    float m_MaxVelocity{ 6000 };
    bool  m_VelocityCapped{ true };
    float m_StartingVelocity;
    float m_Velocity;
    float m_VelocityY;
    float m_VelocityX;

    struct Line
    {
        SDL_Point lineStart;
        SDL_Point lineEnd;
    };

    // Lines go outwards from ball
    Line m_CollisionLines[CollisionLinesIds::TotalLines]{};
    bool m_CollisionDetected[CollisionLinesIds::TotalLines];

    Sides m_LastHitVert;
    Sides m_LastHitHorizontal;
    Sides m_CurrentHitHorizontal;
    Sides m_CurrentHitVert;
    

    bool m_SoundEvents[(int)TotalBallSoundEvents];


    bool m_ChangeVerticalVelocity{ false };
    float m_VerticalVelocityRandomizer;

    int m_LastBounceCounter;
    int m_BounceCounter;
    float m_VelocityMult;
    bool m_SpeedUp;


    TimeHandler m_Time;
    float m_LastTick;
    float m_MoveDelay;
    float m_AccumulatedTime;

};

