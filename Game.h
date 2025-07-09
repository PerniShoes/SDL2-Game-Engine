#pragma once // NOLINT
#include <stdio.h>
#include <cmath>
#include <string>
#include <sstream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstdlib>

#include "Debug.h"
#include "Renderer.h"
#include "Window.h"
#include "Audio.h"
#include "TextureHandler.h"
#include "TimeHandler.h"
#include "Player.h"
#include "MouseHandler.h"
#include "Ball.h"
#include "EnemyAi.h"

enum class Gamemodes
{
	Vs_Ai = 0,
	Survival = 1,
	EndlessMadness = 2,

	TotalGamemodes = 3
};
using enum Gamemodes;

enum class PlatformColors
{
	almostTransparent = 0,
	Pwhite = 1,
	Pred = 2,
	Pblue = 3,
	Pgreen = 4,
	Pyellow = 5,
	Ppink = 6,
	Ppurple = 7,
	Porange = 8,
	PlightBlue = 9,

	TotalColors = 10
};
using enum PlatformColors;

class Game
{
	public:
		Game(const Game&) = delete;
		Game& operator = (const Game&) = delete;

		Game();	
		~Game();

		bool SDLInit();
		bool LoadMedia();
		void Close();
		void GameLoop();	
		
	private:

		// Game settings:
		Gamemodes m_Gamemode = Vs_Ai;	// Vs_Ai Survival EndlessMadness (EndlessMadness overwrites some other settings)
		// Ai: 
		DifficultySettings m_AiDifficulty = DifficultySettings::medium; // easy medium hard veryHard unbeatable
		bool m_AiDiffIncreasing = false;    // Ai increases in difficulty with time, up to veryHard

		int m_ScoreToWin = 5;				// Up to 2 147 483 647
		float m_BallSpeedIncrease = 1.02f;	// How much the ball speeds up on bounce (1.0f being none 1.05f being 5%)
		bool m_BallVelocityCapped{ true };	// Collision will not work if speed gets high enough
		int m_BoardShrinkingSpeed = 1;		// Between 1-10 (or more if you want insanity). 1 = Off

		// User settings:
		PlatformColors m_PlayerPlatformColor = Ppink; // Pwhite Pred Pblue Pgreen Pyellow Ppink Ppurple Porange PlightBlue almostTransparent
		int m_MusicVolume = 10;		// Between 0 (silence) and 100 (MAX)
		int m_SoundVolume = 30;		// Between 0 (silence) and 100 (MAX)
		int m_ScreenBrightness = 0;	// Between 0 (Default brightness) and 255 (Complete darkness)



		
		// Components used:
		Audio m_Audio;	
		MusicList m_MusicPlaying;
		TextureHandler m_Textures;
		MouseHandler m_Mouse;
		TimeHandler m_Time;
		SDL_Event m_E;
		// Made for Pong:
		Player m_Player;
		Ball m_Ball;
		EnemyAi m_EnemyAi;

		// Functions:
		void Events();
		void HandleMouseEvents();
		void HandleKeyEvents();

		void Logic();
		void ballScoreHandling();
		void ballLogic();
		void playerLogic();
		void enemyLogic();
		void borderSizeHandling();
		void handleMusic();
		void handleSoundEvents();
		void timerLogic();
		void textUILogic();
	

		void Rendering();
		void scoreTextRendering();
		void netLineRendering();
		void renderBoardBoarders();
		void roundEndScreenRender();
		// Brightness:
		void handleScreenBrightness();
		void backGroundBrightness();


		// Variables:
		bool m_Quit;
		bool m_Restart{ false };

		// Board borders:
		SDL_Rect m_BoardBorderTop;
		SDL_Rect m_BoardBorderBottom;
		SDL_Rect m_BoardBorderRightTest;
		int m_StartingBorderGirth{ 20 };
		int m_BorderGirth{ m_StartingBorderGirth };
	
		// For border shrinking/growing:
		float m_AccumulatedTime{ 1.0f };
		float m_LastTick{ 0.0f };
		bool  m_ShrinkSwitch{ false };


		// Score handling:
		std::string scoreString{ "0" };
		int m_PlayerScore{ 0 };
		int m_AiScore{ 0 };


		// Code cleanup:
		// Add const to functions
		// Handle default/all cases in switches (preferably default)



		//		Things to fix:
		// Ball going off screen on higher boarder shrink speeds (around 10) (Rn ball is teleported without direction change, so it stutters)
		// Too much reppetition overall. Like SDL_Color white/black initialized many times
		// Some cleaner way of doing gamemodes???
		//



		//		Things I could add in the future if I wanted to:
		// Ball color changing with velocity, to more and more blue
		// Also add ball impact animations facing the way from which ball came 
		// Also can add impact on paddle but both might be bugged on high speeds
		// Can change which anim used based on speed
		//

};