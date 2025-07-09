#include <stdlib.h>
#include <crtdbg.h>
#include "Game.h"

//Game GameInstance;		

int main(int argc, char* args[])
{
	Game GameInstance;



	if (!GameInstance.SDLInit())
	{
		Debug::Print("Failed to initialize!\n");
		char stop{}; 
		std::cin >> stop;
	}
	else
	{
		if (!GameInstance.loadMedia())
		{
			Debug::Print("Failed to load media!\n");
			char stop{}; 
			std::cin >> stop;
		}
		else
		{

			GameInstance.gameLoop();

			GameInstance.close();
			
		}
	}
	
	return 0;
};
