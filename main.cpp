#include "Game.h";

Game *game = nullptr;
int main(int argc,  char *argv[])
{
	const int FPS = 60;
	const int frameDelay = 1000 / FPS; // sets frameDelay = (milliseconds in a second / FPS)
	// this means that there should be a new frame approx. every 13 milliseconds

	Uint32 frameStart;
	int frameTime;

	game = new Game();
	game->Init("BoidEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false);

	while (game->Running())
	{
		frameStart = SDL_GetTicks(); // get total ticks (milliseconds) since initialization

		game->HandleEvents();
		game->Update();
		game->Render();

		frameTime = SDL_GetTicks() - frameStart; // get ticks passed since last tick by subtracting frameStart ticks from current ticks
		if (frameTime < frameDelay) // if ticks passed since last tick are smaller than 13 milliseconds
		{
			SDL_Delay(frameDelay - frameTime); // delay the next frame by 13 - frameTime
		}
		std::cout << frameDelay - frameTime << std::endl;
	}
	game->Clean(); // delete game memory

	return 0;
}