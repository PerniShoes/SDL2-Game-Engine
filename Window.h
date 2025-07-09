#pragma once // NOLINT
#include <SDL.h>
#include "Debug.h"

class Window
{
public:

	Window(const Window&) = delete;
	Window& operator = (const Window&) = delete;
	 
	static Window& Get()
	{
		//Debug::Print("Get function called\n");
		static Window s_Instance;
		return s_Instance;	
	}

	static SDL_Window* GetWindow() { return Get().GetWindowImpl();};
	static SDL_Point GetWindowSize() { return Get().GetWindowSizeImpl();};

private:

	Window() 
	{
		Window_Instance = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, s_SCREEN_WIDTH, s_SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		//Debug::Print("Constructor called\n");
	};

	~Window()
	{
		SDL_DestroyWindow(Window_Instance);
		Window_Instance = NULL;
	};

	SDL_Point GetWindowSizeImpl() { return SDL_Point{ s_SCREEN_WIDTH,s_SCREEN_HEIGHT }; };
	SDL_Window* GetWindowImpl() { return Window_Instance; };
	
	SDL_Window* Window_Instance;
	static const int s_SCREEN_WIDTH = 1280;
	static const int s_SCREEN_HEIGHT = 900;
};

