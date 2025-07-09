#include "Game.h"

Game::Game()
	: m_Quit{ false }, m_EventsQ{ 0 }, m_Ball{ SDL_Point{900,300+ m_BorderGirth}, 20, 20, 700.0f, m_BallVelocityCapped },
	m_Player{ 0 + 40 - 10, 0 + m_BorderGirth, 5 }, 
	m_EnemyAi{ int(Window::GetWindowSize().x - 50.0f), 0 + m_BorderGirth, m_AiDifficulty,m_AiDiffIncreasing }, m_MusicPlaying{ MusicList::StopMusic }

	// fix initializer order, initalize everything
{
	if (m_Gamemode == Gamemodes::EndlessMadness)
	{
		m_ScoreToWin = 69420;
		m_EnemyAi.setDifficulty(DifficultySettings::unbeatable);
		m_BoardShrinkingSpeed = 20;
		m_BallSpeedIncrease = 1.02f;

	}


};

Game::~Game()
{

};

bool Game::SDLInit()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		Debug::Print("SDL could not initialize! SDL Error: %s", SDL_GetError(),"\n");
		success = false;
	}
	else
	{
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			Debug::Print("Warning: Linear texture filtering not enabled!");
		}

		if (Window::GetWindow() == NULL)
		{
			Debug::Print("Window could not be created! SDL Error: %s", SDL_GetError(),"\n");
			success = false;
		}

		else
		{
			if (Renderer::GetRenderer() == NULL)
			{
				Debug::Print("Renderer could not be created! SDL Error: %s", SDL_GetError(),"\n");
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(Renderer::GetRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					Debug::Print("SDL_image could not initialize! SDL_image Error: %s", IMG_GetError(),"\n");
					success = false;
				}
			}
			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
			{
				Debug::Print("SDL_mixer could not initialize! SDL_mixer Error: %s", Mix_GetError(),"\n");	
				success = false;
			}
			if (TTF_Init() == -1)
			{
				Debug::Print("SDL_ttf could not initialize! SDL_ttf Error: %s", TTF_GetError(),"\n");
				success = false;
			}
		}
	}



	return success;
};

bool Game::loadMedia()
{
	bool success = true;
	
	success = m_Audio.loadAudio();
	if (!success)
	{
		Debug::Print("Audio failed to initialize!\n");
	}

	success = m_Textures.loadTextures();
	if (!success)
	{
		Debug::Print("Textures failed to initialize!\n");
	}

	return success;
};

void Game::gameLoop()
{

	while (!m_Quit)
	{
		while (SDL_PollEvent(&m_EventsQ) != 0)
		{
			if (m_EventsQ.type == SDL_QUIT)
			{
				m_Quit = true;
			}
			else {

				events();


			}
	
		}
		logic();
		rendering();
	}


};

void Game::events()
{
	m_Player.handleEvent(m_EventsQ);

	handleMouseEvents();
	handleKeyEvents();

};

void Game::handleKeyEvents()
{
	m_Player.handleEvent(m_EventsQ);
	
	if (m_EventsQ.type == SDL_KEYDOWN)
	{
		switch (m_EventsQ.key.keysym.sym)
		{
		case SDLK_r:
			m_PlayerScore = 0;
			m_AiScore = 0;

			m_Ball.restartBall();
			m_LastTick = 0;
			m_Time.restartTime();

			break;
		case SDLK_ESCAPE:
			m_Quit = true;
			break;
		
		}
	}

};

void Game::handleMouseEvents()
{
	m_Mouse.handleEvents(m_EventsQ);
	
};

void Game::logic()
{
	using enum Gamemodes;
	if (m_Gamemode == Survival)
	{
		borderSizeHandling();
		m_Mouse.updateState();
		handleMusic();
		ballScoreHandling();

		handleSoundEvents();

		playerLogic();
		//enemyLogic();
		ballLogic();
		textUILogic();
		timerLogic();

	}

	if (m_Gamemode == EndlessMadness)
	{
		borderSizeHandling();
		m_Mouse.updateState();
		handleMusic();
		ballScoreHandling();

		handleSoundEvents();

		playerLogic();
		enemyLogic();
		ballLogic();
		textUILogic();
		timerLogic();

	}

	if (m_Gamemode == Vs_Ai)
	{
		borderSizeHandling();
		m_Mouse.updateState();
		handleMusic();
		ballScoreHandling();

		handleSoundEvents();

		playerLogic();
		enemyLogic();
		ballLogic();
		textUILogic();
		timerLogic();

	}

};


void Game::rendering()
{
	SDL_Color black{ 0, 0, 0, 255 };
	SDL_SetRenderDrawColor(Renderer::GetRenderer(), black.r, black.g, black.b, black.a);
	SDL_RenderClear(Renderer::GetRenderer());

	
	using enum Gamemodes;
	// Background:
	switch (m_Gamemode) // Default case handling
	{
	case EndlessMadness:
		m_Textures.render(BackGroundBlackHole, true); 
		break;
	case Vs_Ai:			
		m_Textures.render(BackgroundDazzlingForest, true); 
		break;
	case Survival:		
		m_Textures.render(BackGroundSpace, true); 
		break;
	}
	backGroundBrightness();
	renderBoardBoarders();
	
	netLineRendering();
	
	// Foreground:
	m_Textures.render(PongBall);

	if (m_Gamemode == Vs_Ai || m_Gamemode == EndlessMadness)
	{
		m_Textures.render(EnemyPong);
		scoreTextRendering();
	}
	m_Textures.render(PongPlayer);
	m_Textures.render(TimeText);
	m_Textures.render(QuitText);
	m_Textures.render(RestartText);

	
	roundEndScreenRender();


	handleScreenBrightness();

	SDL_SetRenderDrawColor(Renderer::GetRenderer(), black.r, black.g, black.b, black.a);
	SDL_RenderSetViewport(Renderer::GetRenderer(), NULL);
	SDL_RenderPresent(Renderer::GetRenderer());
};

void Game::close()
{
	
	m_Textures.~TextureHandler();
	m_Audio.~Audio();
	TTF_Quit();	
	IMG_Quit();	
	SDL_Quit();
};

void Game::borderSizeHandling()
{
	m_BoardBorderBottom = { 0,Window::GetWindowSize().y - m_BorderGirth,
		Window::GetWindowSize().x,Window::GetWindowSize().y 
	};
	m_BoardBorderTop = { 0,0,Window::GetWindowSize().x, 
		m_BorderGirth 
	};
	m_BoardBorderRightTest = { Window::GetWindowSize().x - m_BorderGirth,0+m_BorderGirth, 
		Window::GetWindowSize().x,Window::GetWindowSize().y-m_BorderGirth*2 
	};

	float elapsedTime = 0.0f;
	elapsedTime = ((m_Time.getTimePassedInMs() - m_LastTick) / 1000.0f);
	if (elapsedTime > 0.1f)
	{
		elapsedTime = 0.1f;
	}
	m_LastTick = (float)m_Time.getTimePassedInMs();


	if (m_BoardShrinkingSpeed > 1)
	{
		if (m_ShrinkSwitch)
		{
			m_AccumulatedTime -= elapsedTime * m_BoardShrinkingSpeed;
		}
		else if (!m_ShrinkSwitch)
		{
			m_AccumulatedTime += elapsedTime * m_BoardShrinkingSpeed;
		}
	}

	if (m_BorderGirth > 350)
	{
		m_BorderGirth = 350;
		m_ShrinkSwitch = true;
	}
	else if (m_BorderGirth < 20)
	{
		m_BorderGirth = 20;
		m_ShrinkSwitch = false;
	}

	m_BorderGirth = int(m_StartingBorderGirth * m_AccumulatedTime);


}
void Game::renderBoardBoarders()
{
	SDL_Color white{ 255, 255, 255, 255 };
	SDL_SetRenderDrawBlendMode(Renderer::GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Renderer::GetRenderer(), white.r, white.g, white.b, 100);
	SDL_RenderFillRect(Renderer::GetRenderer(), &m_BoardBorderBottom);
	SDL_RenderFillRect(Renderer::GetRenderer(), &m_BoardBorderTop);
	if (m_Gamemode == Gamemodes::Survival)
	{
		SDL_RenderFillRect(Renderer::GetRenderer(), &m_BoardBorderRightTest);
	}
}


void Game::handleMusic()
{
	using enum Gamemodes;
	using enum MusicList;
	if (m_MusicPlaying == StopMusic)
	{
		if (m_Gamemode == EndlessMadness)
		{
			m_Audio.playMusic(EisenfunkPong, m_MusicVolume);
		}
		else if (m_Gamemode == Survival)
		{
			m_Audio.playMusic(HeliosLexica, m_MusicVolume);
		}
		else
		{
			m_Audio.playMusic(PressPlayMusic, m_MusicVolume);
		}
	}

}

void Game::enemyLogic()
{
	m_EnemyAi.setSize(m_Textures.getRect(EnemyPong)->h, m_Textures.getRect(EnemyPong)->w);
	m_EnemyAi.move(m_BorderGirth, m_Ball.getPosition(), m_Ball.getVelocity());
	m_Textures.setPos(EnemyPong, m_EnemyAi.getPosition());
}
void Game::playerLogic()
{
	if (m_Gamemode == Gamemodes::EndlessMadness)
	{
		m_Textures.setScale(PongPlayer, 0.7f);
		m_Player.setVelocity(600.0f);
	}
	else
	{
		m_Textures.setScale(PongPlayer, 1.0f);
		m_Player.setVelocity(500.0f);
	}

	m_Player.setSize(m_Textures.getRect(PongPlayer)->h, m_Textures.getRect(PongPlayer)->w);

	m_Player.move(m_BorderGirth        /*, true, m_Ball.getPosition(), m_Ball.getVelocity()   */      );
	m_Textures.setPos(PongPlayer, m_Player.getPosition());
	using enum PlatformColors;
	switch (m_PlayerPlatformColor)
	{
	case white:
		m_Textures.setColor(PongPlayer, SDL_Color{ 255,255,255 }); 
		break;
	case red:
		m_Textures.setColor(PongPlayer, SDL_Color{ 255,0,0 }); 
		break;
	case blue:
		m_Textures.setColor(PongPlayer, SDL_Color{ 0,0,255 }); 
		break;
	case green:
		m_Textures.setColor(PongPlayer, SDL_Color{ 0,255,0 }); 
		break;
	case yellow:
		m_Textures.setColor(PongPlayer, SDL_Color{ 255,255,0 }); 
		break;
	case pink:
		m_Textures.setColor(PongPlayer, SDL_Color{ 255,51,204 }); 
		break;
	case purple:
		m_Textures.setColor(PongPlayer, SDL_Color{ 153,0,204 });
		break;
	case lightBlue:
		m_Textures.setColor(PongPlayer, SDL_Color{ 153,204,255 }); 
		break;
	case orange:
		m_Textures.setColor(PongPlayer, SDL_Color{ 255,153,51 }); 
		break;
	case almostTransparent:
		m_Textures.setColor(PongPlayer, SDL_Color{ 255,255,255 });
		m_Textures.setAlpha(PongPlayer, 5);
		break;

	}

}
void Game::ballLogic()
{
	// Too fast boarder growth handling:
	if (m_Ball.getPosition().y < 0 + m_BorderGirth)
	{
		m_Ball.setPos({ m_Ball.getPosition().x, 0 + m_BorderGirth + int(m_Textures.getRect(PongBall)->h*1.02)  });
	}
	if (m_Ball.getPosition().y + m_Textures.getRect(PongBall)->h > Window::GetWindowSize().y - m_BorderGirth)
	{
		m_Ball.setPos({ m_Ball.getPosition().x, Window::GetWindowSize().y - m_BorderGirth - int(m_Textures.getRect(PongBall)->h*1.02) });
	}


	m_Ball.setVelocityMult(m_BallSpeedIncrease);
	m_Ball.move(Sides::Top, Sides::Right);

	m_Ball.handleCollision(m_Textures.getRect(EnemyPong));
	if (m_Gamemode == Gamemodes::Survival)
	{
		m_Ball.handleCollision(&m_BoardBorderRightTest);
	}
	m_Ball.handleCollision(&m_BoardBorderBottom);
	m_Ball.handleCollision(&m_BoardBorderTop);
	m_Ball.handleCollision(m_Textures.getRect(PongPlayer));
	m_Ball.setSize(m_Textures.getRect(PongBall)->h, m_Textures.getRect(PongBall)->w);
	m_Ball.setCollisionLines();
	m_Textures.setPos(PongBall, m_Ball.getPosition());

}
void Game::timerLogic()
{
	m_Textures.changeText(TimeText, m_Time.getTimePassedFull());
	m_Textures.setPos(TimeText, SDL_Point{
		(Window::GetWindowSize().x - 250 - m_BorderGirth),
		20 + m_BorderGirth });

}

void Game::backGroundBrightness()
{
	SDL_Color black{ 0, 0, 0, 255 };
	SDL_Rect fullScreen{ 0,0, Window::GetWindowSize().x,Window::GetWindowSize().y };

	SDL_SetRenderDrawBlendMode(Renderer::GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Renderer::GetRenderer(), black.r, black.g, black.b, 100);
	SDL_RenderFillRect(Renderer::GetRenderer(), &fullScreen);

}
void Game::handleScreenBrightness()
{
	SDL_Color black{ 0, 0, 0, 255 };
	SDL_Rect fullScreen{ 0,0, Window::GetWindowSize().x,Window::GetWindowSize().y };
	SDL_SetRenderDrawBlendMode(Renderer::GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Renderer::GetRenderer(), black.r, black.g, black.b, m_ScreenBrightness);
	SDL_RenderFillRect(Renderer::GetRenderer(), &fullScreen);
}

void Game::scoreTextRendering()
{
	if (m_Gamemode != Gamemodes::Survival)
	{
		// 9 being nine zeros in 2 147 483 647 (max score)
		int spacing{ 0 };
		int placeHolderCalc{ 0 };
		placeHolderCalc = m_PlayerScore;

		int maxScoreLength{ 9 };

		for (int i{ 0 }; i < maxScoreLength; i++)
		{
			placeHolderCalc /= 10 * i+1;
			if (placeHolderCalc >= 1)
			{
				spacing = 1 * i;
			}
			else
			{
				break;
			}
		}

		m_Textures.setPos(ScoreText, { int(Window::GetWindowSize().x / 2 - 75.0f - 35.0f - (70.0f * spacing)), int(Window::GetWindowSize().y * 0.05f) + m_BorderGirth });
		m_Textures.changeText(ScoreText, std::to_string(m_PlayerScore));
		m_Textures.render(ScoreText);

		m_Textures.setPos(ScoreText, { int(Window::GetWindowSize().x / 2 + 75.0f), int(Window::GetWindowSize().y * 0.05f) + m_BorderGirth });
		m_Textures.changeText(ScoreText, std::to_string(m_AiScore));
		m_Textures.render(ScoreText);
	}
}

void Game::netLineRendering()
{
	SDL_Color white{ 255, 255, 255, 255 };
	SDL_SetRenderDrawColor(Renderer::GetRenderer(), white.r, white.g, white.b, 100);
	int middleLineSquareSize{ 8 };
	SDL_Rect middleScreenLine{ int(Window::GetWindowSize().x / 2) - middleLineSquareSize / 2,0, middleLineSquareSize,middleLineSquareSize };
	int indexMax{ (Window::GetWindowSize().y - m_BorderGirth * 2) / (middleLineSquareSize * 2) };
	for (int i{ 0 }; i < indexMax; i++)
	{
		middleScreenLine.y = middleLineSquareSize + m_BorderGirth + (((middleLineSquareSize * 2) * i));
		SDL_RenderFillRect(Renderer::GetRenderer(), &middleScreenLine);
	}
}

void Game::ballScoreHandling()
{
	using enum SoundsList;
	if (m_Ball.getPosition().x >= Window::GetWindowSize().x +50)
	{
		m_Ball.restartBall();
		m_PlayerScore += 1;
		m_Audio.playSound(Score_sound, m_SoundVolume);
	}
	else if (m_Ball.getPosition().x < 0 - 50)
	{
		m_Ball.restartBall();
		m_AiScore += 1;
		m_Audio.playSound(Score_sound, m_SoundVolume);
	}
	
}

void Game::handleSoundEvents()
{
	using enum SoundsList;
	switch ((int)m_Ball.handleSoundEvents())
	{
	case (int)PaddleHit: 
		m_Audio.playSound(PaddleSoundEffect, m_SoundVolume); 
		break;
	case (int)WallHit: 
		m_Audio.playSound(WallHitSoundEffect, m_SoundVolume); 
		break;
	default: 
		break;
	}

}

void Game::textUILogic()
{
	m_Textures.setColor(QuitText, SDL_Color{ 0,0,0 });
	m_Textures.setPos(QuitText, SDL_Point{ 0,0 });

	m_Textures.setColor(RestartText, SDL_Color{ 0,0,0 });
	m_Textures.setPos(RestartText, SDL_Point{ Window::GetWindowSize().x - 150,0});
}

void Game::roundEndScreenRender()
{
	using enum Gamemodes;
	float scale{ 3.0f };
	m_Textures.setPos(EndScreenText, SDL_Point{ Window::GetWindowSize().x / 2- int(105*scale), Window::GetWindowSize().y / 2 -int(70*scale) });
	m_Textures.setScale(EndScreenText, scale);

	if (m_Gamemode == Survival)
	{
		
		if (m_AiScore == 0)
		{
			scoreString = m_Time.getTimePassedFull();
		}
		if (m_AiScore >= 1)
		{
			
			std::string ballVelocityCap{ "No" };
			if (m_BallVelocityCapped)
			{
				ballVelocityCap = "Yes";
			}

			float scale{ 1.0f };
			m_Textures.setScale(EndScreenText, scale);

			m_Textures.setPos(EndScreenText, SDL_Point{ int(Window::GetWindowSize().x * 0.15f),100 - int(25 * scale)*2 });

			m_Textures.changeText(EndScreenText, "Time survived: "+ scoreString);

			m_Textures.render(EndScreenText);

			m_Textures.setPos(EndScreenText, SDL_Point{int(Window::GetWindowSize().x*0.15f),100});
			m_Textures.changeText(EndScreenText, "Your settings");
			m_Textures.render(EndScreenText);

			m_Textures.setPos(EndScreenText, SDL_Point{ int(Window::GetWindowSize().x * 0.15f),100 + int(25 * scale) });
			m_Textures.changeText(EndScreenText, "BallVelocity cap: " + ballVelocityCap);
			m_Textures.render(EndScreenText);

			std::stringstream ballSpeedIncreaseString;
			ballSpeedIncreaseString << m_BallSpeedIncrease;

			m_Textures.setPos(EndScreenText, SDL_Point{ int(Window::GetWindowSize().x * 0.15f),100 + int(25 * scale)*2 });
			m_Textures.changeText(EndScreenText, "Ball velocity increase: " + ballSpeedIncreaseString.str());
			m_Textures.render(EndScreenText);

			
			std::string transparent{ "No" };
			if (m_PlayerPlatformColor == PlatformColors::almostTransparent)
			{
				transparent = "Yes";
			}

			m_Textures.setPos(EndScreenText, SDL_Point{ int(Window::GetWindowSize().x * 0.15f),100 + int(25 * scale) * 3 });
			m_Textures.changeText(EndScreenText, "Almost transparent: " + transparent);
			m_Textures.render(EndScreenText);

			m_Textures.setPos(EndScreenText, SDL_Point{ int(Window::GetWindowSize().x * 0.15f),100 + int(25 * scale) * 4 });
			m_Textures.changeText(EndScreenText, "Board shrinking speed: " + std::to_string(m_BoardShrinkingSpeed));
			m_Textures.render(EndScreenText);
			// ballvelcap, ballspeed incres, board shrinking speed almost transparte?

			m_Ball.restartBall();
			m_Textures.render(EndScreenText);
		}
	}
	
	if (m_Gamemode == EndlessMadness)
	{
		if (m_PlayerScore >= 69420)
		{
			m_Textures.setPos(EndScreenText, SDL_Point{ 0,100 });
			m_Textures.changeText(EndScreenText, "H O W ?");
			m_Textures.render(EndScreenText);

			m_Ball.restartBall();
			m_Textures.render(EndScreenText);
		}
	}


	if (m_Gamemode == Vs_Ai)
	{
		if (m_AiScore >= m_ScoreToWin)
		{
			m_Textures.changeText(EndScreenText, "YOU LOSE GET GUD");
			m_Ball.restartBall();
			m_Textures.render(EndScreenText);
			
		}
		else if (m_PlayerScore >= m_ScoreToWin)
		{
			m_Textures.changeText(EndScreenText, "YOU WIN BIG WIN");
			m_Ball.restartBall();
			m_Textures.render(EndScreenText);
		}


	}

}

