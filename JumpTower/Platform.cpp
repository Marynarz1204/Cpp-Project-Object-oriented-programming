#include "Platform.h"

float Platform::getTopX()
{
	return x;
}

float Platform::getX()
{
	return x;
}

float Platform::getY()
{
	return y;
}

float Platform::getHeight()
{
	return height;
}

float Platform::getWidth()
{
	return width;
}

u32 Platform::getColor()
{
	return color;
}

int Platform::fall()
{
	y -= speed;
	if (y < -43.f)
	{
		respawn();
		return 1;
	}
	return 0;
}

void Platform::respawn()
{
	if (width != 40)
		random();
}

void Platform::setSpeed(float s)
{
	speed = 30 * s;
}

