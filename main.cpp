#include "Game.h";
#include <chrono>
using namespace std::chrono;


Game *game = nullptr;
int main(int argc,  char *argv[])
{
	const double FPS = 60;
	const double frameDelay = 1000 / FPS; // sets frameDelay = (milliseconds in a second / FPS)
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

		auto start = std::chrono::high_resolution_clock().now();
		game->HandleEvents();
		while (lag >= frameDelay)
		{
			game->Update();
			lag -= frameDelay;
			game->ExecuteQueues(frameDelay - lag);
			start = std::chrono::high_resolution_clock().now();
			//game->ExecuteQueues(6);
		}
		
		auto end = std::chrono::high_resolution_clock().now();
		auto totaltime = std::chrono::duration_cast<milliseconds>(end - start);
		if (totaltime.count() > 10)
		{
			std::cout << totaltime.count() << std::endl;
		}


		game->Render();
		

		
	}
	game->Clean(); // delete game memory

	return 0;
}

