#pragma once // NOLINT
#include <SDL.h>
#include "Debug.h"
#include "Window.h"

class Renderer
{
public:

	Renderer(const Renderer&) = delete;
	Renderer& operator = (const Renderer&) = delete;	
	 
	static Renderer& Get()
	{
		//Debug::Print("Get function called\n");
		static Renderer s_Instance;
		return s_Instance;	
	}

	static SDL_Renderer* GetRenderer() { return Get().GetRendererImpl(); };


private:

	Renderer() 
	{
		Renderer_Instance = SDL_CreateRenderer(Window::GetWindow(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		//Debug::Print("Renderer constructor called\n");
	};

	~Renderer() 
	{
		SDL_DestroyRenderer(Renderer_Instance);	
		Renderer_Instance = NULL;
	};

	SDL_Renderer* GetRendererImpl() { return Renderer_Instance; };

	SDL_Renderer* Renderer_Instance;

};




