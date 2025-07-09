#include "EnemyAi.h"


EnemyAi::EnemyAi(int posX, int posY, DifficultySettings difficulty, bool diffIncrease)
    :m_Position{ posX,posY }, m_Height{ 10 }, m_Width{ 10 }, m_Velocity{ 300.0f }, m_VelocityY{ 0 }, m_VelocityX{ 0 }, m_DifficultyIncreasing{diffIncrease}
    // fix initializer order, and initialize everything
{
    m_DifficultySetting = { difficulty };
    m_StartingDifficultySetting = { difficulty };

}

EnemyAi::~EnemyAi()
{


}

void EnemyAi::setDifficulty(DifficultySettings difficulty)
{
    m_DifficultySetting = difficulty;

}

SDL_Point EnemyAi::getPosition() const
{
    return m_Position;
}

void EnemyAi::setSize(int height, int width)
{
    m_Height = height;
    m_Width = width;
}

void EnemyAi::setVelocity(float velocity)
{
    m_Velocity = velocity;
}

void EnemyAi::move(int borderGirth, SDL_Point ballPos, float ballVelocity)
{
    using enum DifficultySettings;

    if (m_DifficultySetting != unbeatable)
    {
        if (m_Time.getTimePassedInMin() == 0)
        {
            m_DifficultySetting = m_StartingDifficultySetting;
        }
        else if (m_Time.getTimePassedInMin() > (int)veryHard)
        {
            m_DifficultySetting = veryHard;
        }
        else
        {
            m_DifficultySetting = static_cast<DifficultySettings>((int)m_StartingDifficultySetting + m_Time.getTimePassedInMin());
        }
    }

    setVelocity(440.0f+(160.f*(float)m_DifficultySetting));
    m_VelocityY = m_Velocity;
 
    float elapsedTime = 0.0f;
    elapsedTime = ((m_Time.getTimePassedInMs() - m_LastTick) / 1000.0f);
    m_LastTick = (float)m_Time.getTimePassedInMs();


    if (m_DifficultySetting == unbeatable || m_VelocityY > ballVelocity-250.0f)
    {
        m_Position.y = ballPos.y - (m_Height / 2);

        if (m_Position.y + m_Height > Window::GetWindowSize().y - borderGirth)
        {
            m_Position.y = Window::GetWindowSize().y - borderGirth - m_Height;
        }

        if (m_Position.y < 0 + borderGirth)
        {
            m_Position.y = 0 + borderGirth;
        }


    }
    else
    {
        if (ballPos.y > m_Position.y + int(m_Height*0.75f))
        {
            m_Position.y += int(m_VelocityY * elapsedTime);
        }
        else if (ballPos.y < m_Position.y)
        {
            m_Position.y -= int(m_VelocityY * elapsedTime);
        }


        if (m_Position.y + m_Height > Window::GetWindowSize().y - borderGirth)
        {
            m_Position.y = Window::GetWindowSize().y - borderGirth - m_Height;
        }

        if (m_Position.y < 0 + borderGirth)
        {
            m_Position.y = 0 + borderGirth;
        }
    }
    
}