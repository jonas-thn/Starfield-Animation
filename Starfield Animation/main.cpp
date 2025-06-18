#include <SDL.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "planet.h"
#include "sun.h"
#include <vector>

// alpha, r, g, b
#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_BROWN 0xFFA52A2A
#define COLOR_BLUE 0xFF0000FF
#define COLOR_ORANGE 0xFFFFAA00
#define COLOR_PURPLE 0xFF800080
#define COLOR_GRAY 0xFF808080
#define COLOR_GREEN 0xFF00FF00

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

int ClampInt(int value, int min, int max);
double Clamp(double value, double min, double max);

Uint32 SelectRandomColor()
{
	Uint32 colors[] = {
		COLOR_BROWN,
		COLOR_BLUE,
		COLOR_PURPLE,
		COLOR_GRAY,
		COLOR_GREEN
	};

	int r = rand() % 5;
	return colors[r];
}

int IsNear(double a, double b, double epsilon)
{
	return fabs(a - b) < epsilon;
}

void RandomPositionOnEdge(int& pointX, int& pointY)
{
	int edge = rand() % 4; 

	switch (edge) {
	case 0: // Top
		pointX = rand() % WIDTH;
		pointY = 0;
		break;
	case 1: // Bottom
		pointX = rand() % WIDTH;
		pointY = HEIGHT - 1;
		break;
	case 2: // Left
		pointX = 0;
		pointY = rand() % HEIGHT;
		break;
	case 3: // Right
		pointX = WIDTH - 1;
		pointY = rand() % HEIGHT;
		break;
	}
}

double RandomRange(double min, double max)
{
	return min + ((double)rand() / RAND_MAX) * (max - min);
}

int ClampInt(int value, int min, int max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

double Clamp(double value, double min, double max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

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


void Update(SDL_Surface* surface, Star* stars, int mouseX, int mouseY)
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
	std::vector<Planet> planets(5);

	for(int i = 0; i < planets.size(); i++)
	{
		planets[i] = Planet(RandomRange(0.05, 0.2), 1, RandomRange(200, 300), RandomRange(-WIDTH / 2, WIDTH / 2), RandomRange(-HEIGHT, HEIGHT), surface, SelectRandomColor());
	}

	Planet brown = Planet(0.2, 10, 200, -300, -250, surface, COLOR_BROWN);
	Planet blue = Planet(0.1, 1, 200, -50, 0, surface, COLOR_BLUE);

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


		Update(surface, stars, aimX, aimY);

		for (int i = 0; i < STAR_COUNT; i++)
		{
			Star star = stars[i];

			if (abs((int)star.x) > WIDTH / 2 && abs((int)star.y) > HEIGHT / 2)
			{
				stars[i] = NewStar(aimX, aimY);
			}
		}

		Sun::Draw(aimX, aimY, surface);

		for(int i = 0; i < planets.size(); i++)
		{
			if (planets[i].GetDraw() == 0)
			{
				planets[i] = Planet(RandomRange(0.05, 0.2), 1, RandomRange(200, 300), RandomRange(-WIDTH / 2, WIDTH / 2), RandomRange(-HEIGHT, HEIGHT), surface, SelectRandomColor());
			}

			planets[i].Draw(aimX, aimY);
		}
		
		printf("AimX: %d, AimY: %d\n", aimX, aimY);

		SDL_UpdateWindowSurface(window);
		SDL_Delay(16);
	}


	return 0;
}