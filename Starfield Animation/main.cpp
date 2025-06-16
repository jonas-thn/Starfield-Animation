#include <SDL.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char* argv[]) 
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("Starfield Animation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, NULL);
	

	SDL_Delay(1000);

	return 0;
}