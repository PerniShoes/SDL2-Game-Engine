#include "MouseHandler.h"

MouseHandler::MouseHandler()
	:m_MousePos{ 0,0 }, m_Buttons{ Released,Released,Released }, m_HoldTimer{ 0.0f,0.0f,0.0f }, m_HoldDelay{ 0.15f }, m_LastTick{ 0 }
{
	


}

MouseHandler::~MouseHandler()
{


}

void MouseHandler::updateState()
{
	updateMousePos();
	updateHoldTimers();
}

void MouseHandler::updateHoldTimers()
{
	float elapsedTime = 0.0f;
	elapsedTime = ((m_Time.getTimePassedInMs() - m_LastTick) / 1000.0f);
	m_LastTick = (float)m_Time.getTimePassedInMs();

	for (int index{ 0 }; index < (int)TotalButtons; index++)
	{
		switch (m_Buttons[index])
		{
		case Pressed: [[fallthrough]];
		case Held: m_HoldTimer[index] += elapsedTime;
			if (m_HoldTimer[index] >= m_HoldDelay)
			{
				m_Buttons[index] = Held;
			}
			break;
		default: m_HoldTimer[index] = 0.0f;
			break;
		}
	}
}

void MouseHandler::updateMousePos()
{
	SDL_GetMouseState(&m_MousePos.x, &m_MousePos.y);
}

bool MouseHandler::isMouseInside(SDL_Rect targetRect) const
{
	bool mouseInside = true;

	if (m_MousePos.x < targetRect.x)
	{
		mouseInside = false;
	}
	else if (m_MousePos.x > targetRect.x + targetRect.w)
	{
		mouseInside = false;
	}
	else if (m_MousePos.y < targetRect.y)
	{
		mouseInside = false;
	}
	else if (m_MousePos.y > targetRect.y + targetRect.h)
	{
		mouseInside = false;
	}

	return mouseInside;
}

void MouseHandler::handleEvents(SDL_Event& e)
{

	if (e.type == SDL_MOUSEBUTTONUP)
	{
		switch (e.button.button)
		{
		case SDL_BUTTON_LEFT: m_Buttons[(int)LeftButton] = Released; break;
		case SDL_BUTTON_RIGHT: m_Buttons[(int)RightButton] = Released; break;
		case SDL_BUTTON_MIDDLE: m_Buttons[(int)MiddleButton] = Released; break;
		}
	}
	else if (e.type == SDL_MOUSEBUTTONDOWN)
	{
		switch (e.button.button)
		{
		case SDL_BUTTON_LEFT: m_Buttons[(int)LeftButton] = Pressed; break;
		case SDL_BUTTON_RIGHT: m_Buttons[(int)RightButton] = Pressed; break;
		case SDL_BUTTON_MIDDLE: m_Buttons[(int)MiddleButton] = Pressed; break;
		}
	}
}

ButtonState MouseHandler::getButtonState(ButtonList button) const
{
	switch(button)
	{
	case LeftButton: return m_Buttons[(int)LeftButton]; break;
	case RightButton: return m_Buttons[(int)RightButton]; break;
	case MiddleButton: return m_Buttons[(int)MiddleButton]; break;
	default: return TotalButtonStates;
	}
}

SDL_Point MouseHandler::getMousePos() const
{
	return m_MousePos;
}
