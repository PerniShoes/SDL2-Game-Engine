#pragma once
#include <SDL.h>


#include "Window.h"
#include "Debug.h"
#include <string>
#include <sstream>

#include "TimeHandler.h"

enum class DifficultySettings
{
    easy = 0,
    medium = 1,
    hard = 2,
    veryHard = 3,
    unbeatable = 4, // Meant to never lose

    AllSettings = 5

};

class EnemyAi
{
    public:
        EnemyAi(int posX = 0, int posY = 0, DifficultySettings difficulty = DifficultySettings::easy, bool diffIncrease = false);
        ~EnemyAi();


        void handleEvent();
        void move(int borderGirth, SDL_Point ballPos, float ballVelocity);
        void setSize(int height, int width);

        void increasingDifficulty(int speed, bool turnOn); // From 1 to 5

        // 300 400 500 600 
        SDL_Point getPosition() const;

        void setDifficulty(DifficultySettings difficulty);


    private:

        void setVelocity(float velocity);

        SDL_Point m_Position;
        int m_Height;
        int m_Width;



        float m_Velocity;
        float m_VelocityY;
        float m_VelocityX; // Sideways movement not added. Variable not used

        TimeHandler m_Time;
        float m_LastTick;

        DifficultySettings m_StartingDifficultySetting;
        DifficultySettings m_DifficultySetting;
        bool m_DifficultyIncreasing;

    };


