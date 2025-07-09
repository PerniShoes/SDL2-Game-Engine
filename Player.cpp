#include "Player.h"


Player::Player(int posX, int posY, float velocity)
    :m_Position{ posX,posY }, m_Height{ 10 }, m_Width{ 10 }, m_Velocity{ velocity }, m_VelocityY{ 0 }, m_VelocityX{ 0 }
{


}

Player::~Player()
{

}

SDL_Point Player::getPosition() const
{
    return m_Position;
}

void Player::move(int borderGirth, bool automated, SDL_Point ballPos, float ballVelocity)
{

    float elapsedTime = 0.0f;
    elapsedTime = ((m_Time.getTimePassedInMs() - m_LastTick) / 1000.0f);
    m_LastTick = (float)m_Time.getTimePassedInMs();

    if (automated)
    {
        if (m_VelocityY > ballVelocity - 250.0f)
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
            if (ballPos.y > m_Position.y + m_Height)
            {
                m_VelocityY = m_VelocityY * 1.0f;
            }
            else if (ballPos.y < m_Position.y)
            {
                m_VelocityY = m_VelocityY * -1.0f;
            }

            m_Position.y += int(m_VelocityY * elapsedTime);


            if (m_Position.y + m_Height > Window::GetWindowSize().y - borderGirth)
            {
                m_Position.y = Window::GetWindowSize().y - borderGirth - m_Height;
            }

            if (m_Position.y < 0 + borderGirth)
            {
                m_Position.y = 0 + borderGirth;
            }
        }

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
        m_Position.y += int(m_VelocityY * elapsedTime);
        if (m_Position.y < 0 + borderGirth)
        {
            m_Position.y = 0 + borderGirth;
        }
        if (m_Position.y + m_Height > Window::GetWindowSize().y - borderGirth)
        {
            m_Position.y = Window::GetWindowSize().y - borderGirth - m_Height;
        }
    }
}

void Player::setSize(int height, int width)
{
    m_Height = height;
    m_Width = width;
}

void Player::handleEvent(SDL_Event& e)
{
  
        if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_UP: 
                m_VelocityY -= m_Velocity; 
                break;
            case SDLK_DOWN: 
                m_VelocityY += m_Velocity; 
                break;
                //case SDLK_LEFT: m_VelocityX -= m_Velocity; break;
                //case SDLK_RIGHT: m_VelocityX += m_Velocity; break;
            }
        }

        else if (e.type == SDL_KEYUP && e.key.repeat == 0)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_UP: 
                m_VelocityY += m_Velocity; 
                break;
            case SDLK_DOWN: 
                m_VelocityY -= m_Velocity; 
                break;
                // case SDLK_LEFT: m_VelocityX += m_Velocity; break;
                //case SDLK_RIGHT: m_VelocityX -= m_Velocity; break;
            }

        }
    
}

void Player::setVelocity(float velocity)
{
    m_Velocity = velocity;
}