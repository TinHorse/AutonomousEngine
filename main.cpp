#include "Game.h";

Game *game = nullptr;
int main(int argc,  char *argv[])
{
	const double FPS = 60;
	const double frameDelay = 1000 / FPS; // sets frameDelay = (milliseconds in a second / FPS)
	// this means that there should be a new frame approx. every 13 milliseconds

	game = new Game();
	game->Init("BoidEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false);

	double previous = SDL_GetTicks();
	double lag = 0.f;
	double current;
	double elapsed;

	while (game->Running())
	{
		current = SDL_GetTicks(); // get total ticks (milliseconds) since initialization
		elapsed = current - previous;
		previous = current;
		lag += elapsed;
		game->HandleEvents();

		//std::cout << elapsed << std::endl;
		while (lag >= frameDelay) // if ticks passed since last tick are smaller than 13 milliseconds
		{
			game->Update();
			lag -= frameDelay;
		}
		game->Render();
	}
	game->Clean(); // delete game memory

	return 0;
}