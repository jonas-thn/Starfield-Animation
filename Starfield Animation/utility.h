#pragma once

#include <SDL.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>

// alpha, r, g, b
#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_ORANGE 0xFFFFAA00

#define COLOR_LIGHT_BROWN   0xFFCD853F  
#define COLOR_DARK_BROWN    0xFF654321  

#define COLOR_LIGHT_BLUE    0xFFADD8E6  
#define COLOR_DARK_BLUE     0xFF00008B  
#define COLOR_SKY_BLUE      0xFF87CEEB  
#define COLOR_TEAL          0xFF008080  

#define COLOR_LAVENDER      0xFFE6E6FA  
#define COLOR_VIOLET        0xFF8A2BE2  
#define COLOR_INDIGO        0xFF4B0082		

#define COLOR_LIGHT_GRAY    0xFFD3D3D3
#define COLOR_DARK_GRAY     0xFFA9A9A9
#define COLOR_SLATE_GRAY    0xFF708090

#define COLOR_LIGHT_GREEN   0xFF90EE90
#define COLOR_DARK_GREEN    0xFF006400
#define COLOR_LIME          0xFF32CD32  
#define COLOR_OLIVE         0xFF808000  


#define WIDTH 800
#define HEIGHT 600

Uint32 SelectRandomColor()
{
	Uint32 colors[16] =
	{
		COLOR_LIGHT_BROWN,
		COLOR_DARK_BROWN ,
		COLOR_LIGHT_BLUE ,
		COLOR_DARK_BLUE	 ,
		COLOR_SKY_BLUE	 ,
		COLOR_TEAL		 ,
		COLOR_LAVENDER	 ,
		COLOR_VIOLET	 ,
		COLOR_INDIGO	 ,
		COLOR_LIGHT_GRAY ,
		COLOR_DARK_GRAY	 ,
		COLOR_SLATE_GRAY ,
		COLOR_LIGHT_GREEN,
		COLOR_DARK_GREEN ,
		COLOR_LIME		 ,
		COLOR_OLIVE
	};

	int r = rand() % 16;
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

double Max(double a, double b)
{
	return (a > b) ? a : b;
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