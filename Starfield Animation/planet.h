#pragma once

#include <math.h>
#include <SDL.h>
#include "sun.h"

#include "utility.h"

#define COLOR_SUN 0xFFFFCC88

class Planet
{
public:
	Planet() = default;

	Planet(double speed, int min, int max, int startX, int startY, Uint32 color)
	{
		this->way = min;
		this->speed = speed;
		this->min = min;
		this->max = max;
		this->startX = startX;
		this->startY = startY;
		this->color = color;
		this->draw = 1;
		this->posX = startX;
		this->posY = startY;
	}

	void Draw(int aimX, int aimY, SDL_Surface* surface)
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

		posX = ClampInt(posX, -WIDTH / 2 - max - 20, WIDTH / 2 + max + 20);
		posY = ClampInt(posY, -HEIGHT / 2 - max - 10, HEIGHT / 2 + max + 10);

		DrawCircle(surface, posX, posY, (int)way);
	}

	int GetDraw()
	{
		return draw;
	}

private:
	double speed;
	int min, max; 
	int startX, startY; 
	Uint32 color;
	double way;
	int draw;
	int posX, posY;

	void DrawCircle(SDL_Surface* surface, int centerX, int centerY, int radius)
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

};