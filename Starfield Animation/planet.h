#pragma once

#include <math.h>
#include <SDL.h>

#define WIDTH 800
#define HEIGHT 600

class Planet
{
public:
	Planet(double speed, int min, int max, int startX, int startY, SDL_Surface* surface, Uint32 color)
	{
		this->way = min;
		this->speed = speed;
		this->min = min;
		this->max = max;
		this->startX = startX;
		this->startY = startY;
		this->surface = surface;
		this->color = color;
		this->draw = 1;
	}

	int IsNear(double a, double b, double epsilon)
	{
		return fabs(a - b) < epsilon;
	}

	void DrawCircle(SDL_Surface* surface, int centerX, int centerY, int radius, Uint32 color)
	{
		for (int w = 0; w < radius * 2; w++)
		{
			for (int h = 0; h < radius * 2; h++)
			{
				int dx = radius - w;
				int dy = radius - h;
				if ((dx * dx + dy * dy) <= (radius * radius))
				{
					SDL_Rect rect = { WIDTH / 2 + centerX + dx, HEIGHT / 2 + centerY + dy, 1, 1 };
					SDL_FillRect(surface, &rect, color);
				}
			}
		}
	}

	void DrawPlanet(int aimX, int aimY)
	{
		way += speed;
		way = Clamp(way, min, max);

		static int posX = startX;
		static int posY = startY;

		double speedFactor = 0.001;

		if (IsNear(way, max, 1))
		{
			if ((posX + way < -WIDTH / 2 || posX - way > WIDTH / 2) ||
				(posY + way < -HEIGHT / 2 || posY - way > HEIGHT / 2))
			{
				draw = 0;
			}
		}

		if (!draw)
		{
			return;
		}

		posX -= aimX * speed * way * speedFactor;
		posY -= aimY * speed * way * speedFactor;

		DrawCircle(surface, posX, posY, (int)way, color);
	}

private:
	double speed;
	int min, max; 
	int startX, startY; 
	SDL_Surface* surface;
	Uint32 color;
	double way;
	int draw;

	double Clamp(double value, double min, double max)
	{
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}
};