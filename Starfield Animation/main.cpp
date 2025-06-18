#include <SDL.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "planet.h"
#include "sun.h"
#include <vector>

#include "utility.h"

#define STAR_COUNT 1000
#define SPEED 0.01
#define ACC 0.1

typedef struct
{
	double x, y;
	double vx, vy;
	double speedFactor;
	double distance;
} Star;

Star NewStar(int mouseX, int mouseY)
{
	int radius = RandomRange(50, 100);
	double x = RandomRange(-radius, radius) + mouseX;
	double y = RandomRange(-radius, radius) + mouseY;
	double vx = x * SPEED - (mouseX * 0.01);
	double vy = y * SPEED - (mouseY * 0.01);
	double speedFactor = RandomRange(0, 1);
	double distance = sqrt(x * x + y * y);
	return Star{ x, y, vx, vy, speedFactor, distance};
}


void DrawStars(SDL_Surface* surface, Star* stars, int mouseX, int mouseY)
{
	for(int i = 0; i < STAR_COUNT; i++)
	{
		Star* star = &(stars[i]);
		star->x += star->vx - (mouseX * 0.01);
		star->y += star->vy - (mouseY * 0.01);
		star->vx += star->speedFactor * star->vx * ACC;
		star->vy += star->speedFactor * star->vy * ACC;
		double distance = sqrt(star->x * star->x + star->y * star->y);
		double size = (int)(distance / 70) == 0 ? 1 : (int)(distance / 70);
		double averageSpeed = sqrt(star->vx * star->vx + star->vy * star->vy) / 10;
		size *= averageSpeed;
		size = Clamp(size, 1, 12);
		SDL_Rect rect = { star->x + WIDTH / 2, star->y + HEIGHT / 2, size, size };
		SDL_FillRect(surface, &rect, 0xFFFFFFFF);
	}
}

int main(int argc, char* argv[]) 
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("Starfield Animation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, NULL);
	SDL_Surface* surface = SDL_GetWindowSurface(window);

	srand(time(NULL));

	int is_running = 1;
	Star stars[STAR_COUNT];
	int aimX = 0;
	int aimY = 0;

	Sun::Create(0, 0, COLOR_ORANGE, 100, 0.005);
	std::vector<Planet> planets(4);

	for(int i = 0; i < planets.size(); i++)
	{
		planets[i] = Planet(RandomRange(0.05, 0.2), 1, RandomRange(200, 300), RandomRange(-WIDTH / 2, WIDTH / 2), RandomRange(-HEIGHT, HEIGHT), SelectRandomColor());
	}

	for (int i = 0; i < STAR_COUNT; i++)
	{
		stars[i] = NewStar(aimX, aimY);
	}

	while(is_running)
	{
		static int left = 0;
		static int right = 0;
		static int top = 0;
		static int bottom = 0;

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				is_running = 0;
			}

			if(event.type == SDL_MOUSEMOTION)
			{
				/*mouseX = ClampInt(event.motion.x, 0, WIDTH) - WIDTH / 2;
				mouseY = ClampInt(event.motion.y, 0, HEIGHT) - HEIGHT / 2;*/
			}

			if (event.type == SDL_KEYDOWN)
			{
				if(event.key.keysym.sym == SDLK_ESCAPE)
				{
					is_running = 0;
				}

				if (event.key.keysym.sym == SDLK_w)
				{
					top = 1;
				}
				if (event.key.keysym.sym == SDLK_s)
				{
					bottom = 1;
				}
				if (event.key.keysym.sym == SDLK_a)
				{
					left = 1;
				}
				if (event.key.keysym.sym == SDLK_d)
				{
					right = 1;
				}
			}

			if (event.type == SDL_KEYUP)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					is_running = 0;
				}

				if (event.key.keysym.sym == SDLK_w)
				{
					top = 0;
				}
				if (event.key.keysym.sym == SDLK_s)
				{
					bottom = 0;
				}
				if (event.key.keysym.sym == SDLK_a)
				{
					left = 0;
				}
				if (event.key.keysym.sym == SDLK_d)
				{
					right = 0;
				}
			}
		}

		int aimSpeed = 3;

		aimY -= (top - bottom) * aimSpeed;
		aimX -= (left - right) * aimSpeed;

		int aimOffset = 25;
		aimX = ClampInt(aimX, -WIDTH / 2 + aimOffset, WIDTH / 2 - aimOffset);
		aimY = ClampInt(aimY, -HEIGHT / 2, HEIGHT / 2);

		SDL_FillRect(surface, NULL, 0x00000000);

		Sun::Draw(aimX, aimY, surface);

		DrawStars(surface, stars, aimX, aimY);

		for (int i = 0; i < STAR_COUNT; i++)
		{
			Star star = stars[i];

			if (abs((int)star.x) > WIDTH / 2 && abs((int)star.y) > HEIGHT / 2)
			{
				stars[i] = NewStar(aimX, aimY);
			}
		}

		for(int i = 0; i < planets.size(); i++)
		{
			if (planets[i].GetDraw() == 0)
			{
				planets[i] = Planet(RandomRange(0.05, 0.2), 1, RandomRange(200, 300), RandomRange(-WIDTH / 2, WIDTH / 2), RandomRange(-HEIGHT, HEIGHT), SelectRandomColor());
			}

			planets[i].Draw(aimX, aimY, surface);
		}
		
		SDL_UpdateWindowSurface(window);
		SDL_Delay(16);
	}

	return 0;
}