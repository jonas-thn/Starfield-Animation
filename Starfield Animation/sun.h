#pragma once

#include <math.h>
#include <SDL.h>

#define WIDTH 800
#define HEIGHT 600
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
					double pixelX = centerX + dx;
					double pixelY = centerY + dy;

					SDL_Rect rect = { WIDTH / 2 + pixelX, HEIGHT / 2 + pixelY, 1, 1 };

					double distanceToCenterNormalized = sqrt((centerX - pixelX) * (centerX - pixelX) + (centerY - pixelY) * (centerY - pixelY)) / radius;

					Uint32 colorN = InterpolateColor(color, COLOR_SUN, distanceToCenterNormalized);

					SDL_FillRect(surface, &rect, colorN);
				}
			}
		}
	}

	static Uint32 InterpolateColor(Uint32 color1, Uint32 color2, double t)
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