#include "Game.h";

Game *game = nullptr;
int main(int argc,  char *argv[])
{
	const int FPS = 60;
	const int frameDelay = 1000 / FPS; // sets frameDelay = (milliseconds in a second / FPS)
	// this means that there should be a new frame approx. every 13 milliseconds

	double prevTime = SDL_GetTicks();
	double lag = 0.0f;

	game = new Game();
	game->Init("BoidEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false);

	while (game->Running())
	{
		double currTime = SDL_GetTicks();
		double elapsed = currTime - prevTime;
		prevTime = currTime;
		lag += elapsed;

		game->HandleEvents();
		while (lag >= frameDelay)
		{
			game->Update();
			lag -= frameDelay;
		}
		game->Render();
	}
	game->Clean(); // delete game memory

	return 0;
}