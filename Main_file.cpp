#include <stdlib.h>
#include <crtdbg.h>
#include "Game.h"

Game GameInstance;		

int main(int argc, char* args[])
{
	if (!GameInstance.SDLInit())
	{
		Debug::Print("Failed to initialize!\n");
		char stop{}; std::cin >> stop;
	}
	else
	{
		if (!GameInstance.LoadMedia())
		{
			Debug::Print("Failed to load media!\n");
			char stop{}; std::cin >> stop;
		}
		else
		{

			GameInstance.GameLoop(); 

			GameInstance.Close();
			
		}
	}
	
	return 1;
};
