#include <SDL.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define COLOR_WHITE 0xFFFFFFFF
#define WIDTH 800
#define HEIGHT 600
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

double RandomRange(double min, double max)
{
	return min + ((double)rand() / RAND_MAX) * (max - min);
}

double Clamp(double value, double min, double max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

Star NewStar()
{
	double x = RandomRange(-50, 50);
	double y = RandomRange(-50, 50);
	double vx = x * SPEED;
	double vy = y * SPEED;
	double speedFactor = RandomRange(0, 1);
	double distance = sqrt(x * x + y * y);
	return Star{ x, y, vx, vy, speedFactor, distance};
}

void Update(SDL_Surface* surface, Star* stars)
{
	for(int i = 0; i < STAR_COUNT; i++)
	{
		Star* star = &(stars[i]);
		star->x += star->vx;
		star->y += star->vy;
		star->vx += star->speedFactor * star->vx * ACC;
		star->vy += star->speedFactor * star->vy * ACC;
		double distance = sqrt(star->x * star->x + star->y * star->y);
		double size = (int)(distance / 70) == 0 ? 1 : (int)(distance / 70);
		double averageSpeed = sqrt(star->vx * star->vx + star->vy * star->vy) / 10;
		size *= averageSpeed;
		size = Clamp(size, 1, 10);
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

	for (int i = 0; i < STAR_COUNT; i++)
	{
		stars[i] = NewStar();
	}

	while(is_running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				is_running = 0;
			}
		}

		SDL_FillRect(surface, NULL, 0x00000000);


		Update(surface, stars);

		for (int i = 0; i < STAR_COUNT; i++)
		{
			Star star = stars[i];

			if (abs((int)star.x) > WIDTH / 2 && abs((int)star.y) > HEIGHT / 2)
			{
				stars[i] = NewStar();
			}
		}

		SDL_UpdateWindowSurface(window);
		SDL_Delay(16);
	}


	return 0;
}