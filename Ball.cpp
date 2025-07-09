#include "Ball.h"
using enum Sides;
using enum CollisionLinesId;

Ball::Ball(SDL_Point startingPos, int width, int height, float velocity, bool velocityCapped)
	: m_BallRect{ startingPos.x, startingPos.y, startingPos.x + width, startingPos.y + height },
	m_StartingVelocity{velocity}, m_Velocity{velocity}, m_VelocityY{0}, m_VelocityX{0}, m_LastHitVert{NoCollision}, m_LastHitHorizontal{NoCollision},
	m_CurrentHitHorizontal{ NoCollision }, m_CurrentHitVert{ NoCollision }, m_BounceCounter{ -2 /* Needs -1 to work */ }, 
	m_VelocityMult{ 1.005f }, m_LastBounceCounter{ m_BounceCounter },
	m_SpeedUp{ false }, m_VerticalVelocityRandomizer{ 1.0f }, m_VelocityCapped{ velocityCapped }
	// fix initializer order, initialize bounceCounter

{
	setPos({ Window::GetWindowSize().x / 2, Window::GetWindowSize().y / 2 });
}

Ball::~Ball()
{


}

void Ball::setVelocityMult(float mult)
{
	m_VelocityMult = mult;
}

void Ball::setVelocity(float velocity)
{
	m_Velocity = velocity;
}

void Ball::restartBall()
{
	m_AccumulatedTime = 0;
	setPos({ Window::GetWindowSize().x / 2-m_BallRect.w/2, Window::GetWindowSize().y / 2 });
	m_Velocity = m_StartingVelocity;

}

float Ball::getVelocity() const
{
	return m_Velocity * m_VelocityMult;
}

BallSoundEvents Ball::handleSoundEvents()
{
	for (int i{ 0 }; i < (int)TotalBallSoundEvents; i++)
	{
		if (m_SoundEvents[i])
		{
			//Debug::Print(i, "\n");
			m_SoundEvents[i] = false;
			return (BallSoundEvents)i;
		}

	}
	return NoSoundEvent;
}

void Ball::move(Sides startingColisionVert, Sides startingCollisionHorizontal)
{
	// For now will not well work if ball is between stuff

	// This does way too much various functionality
	// For a first game project I will ignore it for now, so I can focus on something else, but this needs fixing

	if (m_ChangeVerticalVelocity)
	{
		m_VerticalVelocityRandomizer = (rand() % 100 + 1) / 100.0f;
		m_ChangeVerticalVelocity = false;
	}

	m_MoveDelay = (m_Time.getTimePassedInMs() / 1000.0f);

	float elapsedTime = 0.0f;
	elapsedTime = ((m_Time.getTimePassedInMs() - m_LastTick) / 1000.0f);
	m_LastTick = (float)m_Time.getTimePassedInMs();


	m_AccumulatedTime += elapsedTime;

	// Crude lag safety:
	if (elapsedTime >= 0.01f)
	{
		elapsedTime = 0.01f;
	}

	if (m_SpeedUp && m_BounceCounter > 1)
	{
		float tempIntMult{ 1.0f };
		tempIntMult*=m_VelocityMult;

		m_Velocity *= tempIntMult;
		m_SpeedUp = false;
	}

	if (m_VelocityCapped && m_Velocity > m_MaxVelocity)
		{
			m_Velocity = m_MaxVelocity;
		}
	
	if (m_MoveDelay > 1 && m_AccumulatedTime > 1)
	{
		m_VelocityX = (m_Velocity * elapsedTime);

		m_VelocityY = (m_Velocity * m_VerticalVelocityRandomizer * elapsedTime);

		if (m_LastHitVert == NoCollision)
		{
			m_LastHitVert = startingColisionVert;
		}
		if (m_LastHitHorizontal == NoCollision)
		{
			m_LastHitHorizontal = startingCollisionHorizontal;
		}

		switch (m_LastHitHorizontal) // Put default case handling
		{
		case Left: m_BallRect.x += (int)m_VelocityX; break;
		case Right: m_BallRect.x -= (int)m_VelocityX; break;
		}
		switch (m_LastHitVert)
		{
		case Top: m_BallRect.y += (int)m_VelocityY; break;
		case Bottom: m_BallRect.y -= (int)m_VelocityY; break;
		}

		if (m_LastHitHorizontal != m_CurrentHitHorizontal)
		{
			m_BounceCounter += 1;
			m_ChangeVerticalVelocity = true;
			if (m_BounceCounter >= 0)
			{
				m_SoundEvents[(int)PaddleHit] = true;
			}
			}
		if (m_LastHitVert != m_CurrentHitVert)
		{
			m_BounceCounter += 1;
			if (m_BounceCounter >= 1)
			{
				m_SoundEvents[(int)WallHit] = true;
			}
		}

		if (m_LastBounceCounter != m_BounceCounter)
		{
			m_SpeedUp = true;
		}

		m_LastBounceCounter = m_BounceCounter;
		m_CurrentHitHorizontal = m_LastHitHorizontal;
		m_CurrentHitVert = m_LastHitVert;
	}

}

void Ball::drawCollisionLines() const
{
	SDL_Color green{ 0,255,0,255 };
	SDL_Color red  { 255,0,0,255 };
	SDL_SetRenderDrawColor(Renderer::GetRenderer(), green.r, green.g, green.b, green.a);
	for (int i{ 0 }; i < TotalLines; i++)
	{
		if (m_CollisionDetected[i] == true)
		{
			SDL_SetRenderDrawColor(Renderer::GetRenderer(), red.r, red.g, red.b, red.a);
		}
		else
		{
			SDL_SetRenderDrawColor(Renderer::GetRenderer(), green.r, green.g, green.b, green.a);
		}
		SDL_RenderDrawLine(Renderer::GetRenderer(),
			m_CollisionLines[i].lineStart.x, m_CollisionLines[i].lineStart.y,
			m_CollisionLines[i].lineEnd.x, m_CollisionLines[i].lineEnd.y);
	}

}

void Ball::setSize(int height, int width)
{
	m_BallRect.h = height;
	m_BallRect.w = width;
}

void Ball::setCollisionLines()
{
	// Collision line getting longer with speed
	int lineLength{ int(m_Velocity /50.0f) };
	// Lines go outwards from ball
	m_CollisionLines[LeftTop].lineStart = { m_BallRect.x,				m_BallRect.y };
	m_CollisionLines[LeftTop].lineEnd   = { m_BallRect.x - lineLength,	m_BallRect.y };
	
	m_CollisionLines[LeftBottom].lineStart = { m_BallRect.x, 				m_BallRect.y + m_BallRect.h };
	m_CollisionLines[LeftBottom].lineEnd   = { m_BallRect.x - lineLength,	m_BallRect.y + m_BallRect.h };

	m_CollisionLines[RightTop].lineStart = { m_BallRect.x + m_BallRect.w,				m_BallRect.y };
	m_CollisionLines[RightTop].lineEnd   = { m_BallRect.x + m_BallRect.w + lineLength,	m_BallRect.y };
	
	m_CollisionLines[RightBottom].lineStart = { m_BallRect.x + m_BallRect.w,				m_BallRect.y + m_BallRect.h };
	m_CollisionLines[RightBottom].lineEnd   = { m_BallRect.x + m_BallRect.w + lineLength,	m_BallRect.y + m_BallRect.h };



	// Lines go outwards from ball
	m_CollisionLines[TopLeft].lineStart = { m_BallRect.x, m_BallRect.y };
	m_CollisionLines[TopLeft].lineEnd   = { m_BallRect.x, m_BallRect.y - lineLength };
	
	m_CollisionLines[TopRight].lineStart = { m_BallRect.x + m_BallRect.w, m_BallRect.y };
	m_CollisionLines[TopRight].lineEnd   = { m_BallRect.x + m_BallRect.w, m_BallRect.y - lineLength };

	m_CollisionLines[BottomLeft].lineStart = { m_BallRect.x, m_BallRect.y + m_BallRect.h };
	m_CollisionLines[BottomLeft].lineEnd   = { m_BallRect.x, m_BallRect.y + m_BallRect.h + lineLength };

	m_CollisionLines[BottomRight].lineStart = { m_BallRect.x + m_BallRect.w, m_BallRect.y + m_BallRect.h };
	m_CollisionLines[BottomRight].lineEnd   = { m_BallRect.x + m_BallRect.w, m_BallRect.y + m_BallRect.h + lineLength };

}

void Ball::setPos(SDL_Point pos) // Debug
{
	m_BallRect.x = pos.x;
	m_BallRect.y = pos.y;
}

void Ball::detectCollision(SDL_Rect* targetRect) 
{

	if (targetRect != NULL)
	{
		
		// Left side:
		for (int i{ 0 }; i < TotalLines / 4; i++)
		{
			if ((m_CollisionLines[i].lineEnd.x <= targetRect->x + targetRect->w && m_CollisionLines[i].lineStart.x >= targetRect->x + targetRect->w)
				&&
				(m_CollisionLines[i].lineEnd.y >= targetRect->y && m_CollisionLines[i].lineEnd.y <= targetRect->y + targetRect->h))
			{
				m_CollisionDetected[i] = true;
			}
			else
			{
				m_CollisionDetected[i] = false;
			}
		}

		// Right side:
		for (int i{ TotalLines / 4 }; i < TotalLines / 2; i++)
		{
			if ((m_CollisionLines[i].lineEnd.x >= targetRect->x && m_CollisionLines[i].lineStart.x <= targetRect->x)
				&&
				(m_CollisionLines[i].lineEnd.y >= targetRect->y && m_CollisionLines[i].lineEnd.y <= targetRect->y + targetRect->h))
			{
				m_CollisionDetected[i] = true;
			}
			else
			{
				m_CollisionDetected[i] = false;
			}
		}

		// Top:         Probabbly should change mult v to div to be consistent
		for (int i{ TotalLines / 2 }; i < int(int(TotalLines) * 0.75); i++)
		{
			if ((m_CollisionLines[i].lineEnd.y <= targetRect->y + targetRect->h && m_CollisionLines[i].lineStart.y >= targetRect->y + targetRect->h)
				&&
				(m_CollisionLines[i].lineEnd.x >= targetRect->x && m_CollisionLines[i].lineEnd.x <= targetRect->x + targetRect->w))
			{
				m_CollisionDetected[i] = true;
			}
			else
			{
				m_CollisionDetected[i] = false;
			}
		}

		// Bottom:
		for (int i{ int(int(TotalLines) * 0.75) }; i < TotalLines; i++)
		{
			if ((m_CollisionLines[i].lineEnd.y >= targetRect->y && m_CollisionLines[i].lineStart.y <= targetRect->y)
				&&
				(m_CollisionLines[i].lineEnd.x >= targetRect->x && m_CollisionLines[i].lineEnd.x <= targetRect->x + targetRect->w))
			{
				m_CollisionDetected[i] = true;
			}
			else
			{
				m_CollisionDetected[i] = false;
			}
		}

	}
	else
	{
		Debug::Print("Trying to detect collision with NULL rect \n");
	}
}

SDL_Point Ball::getPosition() const
{
	return SDL_Point{ m_BallRect.x,m_BallRect.y };
}

void Ball::handleCollision(SDL_Rect* targetRect)
{
	detectCollision(targetRect);

	// For now will not work well if ball is between stuff
	if (m_CollisionDetected[LeftTop] || m_CollisionDetected[LeftBottom])
	{
		m_LastHitHorizontal = Left;
	}
	else if (m_CollisionDetected[RightTop] || m_CollisionDetected[RightBottom])
	{
		m_LastHitHorizontal = Right;
	}
	else if (m_CollisionDetected[TopRight] || m_CollisionDetected[TopLeft])
	{
		m_LastHitVert = Top;
	}
	else if (m_CollisionDetected[BottomRight] || m_CollisionDetected[BottomLeft])
	{
		m_LastHitVert = Bottom;
	}

}