#pragma once

#include <math.h>
#include <SDL.h>

#include "utility.h"

#define COLOR_SUN 0xFFFFCC88

class Sun
{
public:
	static void Create(int x, int y, Uint32 c, double r, double s)
	{
		startX = x;
		startY = y;
		color = c;
		radius = r;
		posX = startX;
		posY = startY;
		speed = s;
	}

	static void Draw(int aimX, int aimY, SDL_Surface* surface)
	{
		posX -= (int)((double)aimX * speed);
		posY -= (int)((double)aimY * speed);

		posX = ClampInt(posX, -WIDTH/2 - radius - 20, WIDTH/2 + radius + 20);
		posY = ClampInt(posY, -HEIGHT/2 - radius - 10, HEIGHT/2 + radius + 10);

		DrawCircle(surface, posX, posY);
	}

	inline static int startX = 0;
	inline static int startY = 0;
	inline static Uint32 color = 0;
	inline static double radius = 0;
	inline static int posX = 0;
	inline static int posY = 0;
	inline static double speed = 0;

private:
	static void DrawCircle(SDL_Surface* surface, int centerX, int centerY)
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

					double distanceToCenterNormalized = sqrt((centerX - pixelX) * (centerX - pixelX) + (centerY - pixelY) * (centerY - pixelY)) / radius;

					Uint32 colorN = InterpolateColor(0xFFFFFFFF, COLOR_SUN, distanceToCenterNormalized);

					SDL_FillRect(surface, &rect, colorN);
				}
			}
		}
	}
};