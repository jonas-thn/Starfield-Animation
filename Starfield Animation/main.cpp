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

typedef struct
{
	double x, y;
	double vx, vy;
	double speedFactor;
	double distance;
} Planet;



void Bezier2D(double t, double x0, double y0, double cx, double cy, double x1, double y1, double* outX, double* outY)
{
	double u = 1 - t;
	*outX = u * u * x0 + 2 * u * t * cx + t * t * x1;
	*outY = u * u * y0 + 2 * u * t * cy + t * t * y1;
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
	int radius = RandomRange(10, 100);
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
	int mouseX = 0;
	int mouseY = 0;

	for (int i = 0; i < STAR_COUNT; i++)
	{
		stars[i] = NewStar(mouseX, mouseY);
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

			if(event.type == SDL_MOUSEMOTION)
			{
				mouseX = ClampInt(event.motion.x, 0, WIDTH) - WIDTH / 2;
				mouseY = ClampInt(event.motion.y, 0, HEIGHT) - HEIGHT / 2;
			}

			
		}

		SDL_FillRect(surface, NULL, 0x00000000);


		Update(surface, stars, mouseX, mouseY);

		for (int i = 0; i < STAR_COUNT; i++)
		{
			Star star = stars[i];

			if (abs((int)star.x) > WIDTH / 2 && abs((int)star.y) > HEIGHT / 2)
			{
				stars[i] = NewStar(mouseX, mouseY);
			}
		}

		printf("MouseX: %d, MouseY: %d\n", mouseX, mouseY);

		SDL_UpdateWindowSurface(window);
		SDL_Delay(16);
	}


	return 0;
}