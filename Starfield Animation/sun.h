#pragma once

#include <math.h>
#include <SDL.h>

#define WIDTH 800
#define HEIGHT 600

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

		DrawCircle(surface, posX, posY, radius, color);
	}

	inline static int startX = 0;
	inline static int startY = 0;
	inline static Uint32 color = 0;
	inline static double radius = 0;
	inline static int posX = 0;
	inline static int posY = 0;
	inline static double speed = 0;

private:
	static void DrawCircle(SDL_Surface* surface, int centerX, int centerY, int radius, Uint32 color)
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
};