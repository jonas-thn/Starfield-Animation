#pragma once

#include <math.h>
#include <SDL.h>
#include "sun.h"

#define WIDTH 800
#define HEIGHT 600
#define COLOR_SUN 0xFFFFCC88

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
		this->posX = startX;
		this->posY = startY;
	}

	void Draw(int aimX, int aimY)
	{
		way += speed;
		way = Clamp(way, min, max);

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

		posX -= (int)((double)aimX * speed * way * speedFactor);
		posY -= (int)((double)aimY * speed * way * speedFactor);

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
	int posX;
	int posY;

	double Clamp(double value, double min, double max)
	{
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

	int IsNear(double a, double b, double epsilon)
	{
		return fabs(a - b) < epsilon;
	}

	double Max(double a, double b)
	{
		return (a > b) ? a : b;
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
					double pixelX = centerX + dx;
					double pixelY = centerY + dy;

					SDL_Rect rect = { WIDTH / 2 + pixelX, HEIGHT / 2 + pixelY, 1, 1 };

					double distancePixelSun = sqrt((Sun::posX - pixelX) * (Sun::posX - pixelX) + (Sun::posY - pixelY) * (Sun::posY - pixelY));
					double distanceCenterSun = sqrt((Sun::posX - centerX) * (Sun::posX - centerX) + (Sun::posY - centerY) * (Sun::posY - centerY));

					double maxDist = distanceCenterSun + radius;
					double minDist = Max(0.0, distanceCenterSun - radius);

					double normalizedDistance = 1 - (distancePixelSun - minDist) / (maxDist - minDist);

					Uint32 colorShadow = InterpolateColor(0x00000000, color, normalizedDistance);

					SDL_FillRect(surface, &rect, colorShadow);
					
				}
			}
		}
	}

	Uint32 InterpolateColor(Uint32 color1, Uint32 color2, double t)
	{
		if (t < 0.0) t = 0.0;
		if (t > 1.0) t = 1.0;

		Uint8 a1 = (color1 >> 24) & 0xFF;
		Uint8 r1 = (color1 >> 16) & 0xFF;
		Uint8 g1 = (color1 >> 8) & 0xFF;
		Uint8 b1 = (color1 >> 0) & 0xFF;

		Uint8 a2 = (color2 >> 24) & 0xFF;
		Uint8 r2 = (color2 >> 16) & 0xFF;
		Uint8 g2 = (color2 >> 8) & 0xFF;
		Uint8 b2 = (color2 >> 0) & 0xFF;

		Uint8 a = (Uint8)((1.0 - t) * a1 + t * a2);
		Uint8 r = (Uint8)((1.0 - t) * r1 + t * r2);
		Uint8 g = (Uint8)((1.0 - t) * g1 + t * g2);
		Uint8 b = (Uint8)((1.0 - t) * b1 + t * b2);

		return (a << 24) | (r << 16) | (g << 8) | b;
	}
};