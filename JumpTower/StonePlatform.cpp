#include "StonePlatform.h"

void StonePlatform::random()
{
	x = float(rand() % 64) - 32;
}
StonePlatform::StonePlatform()
{
	y = 100.f;
}

StonePlatform::StonePlatform(float basey, float s)
{
	speed = s * 100;
	width = 8;
	height = 1;
	y = basey;
	color = 0x999999;
	random();
}

StonePlatform::StonePlatform(float basey, float milestone, float s)
{
	speed = s * 100;
	width = milestone;
	height = 1;
	y = basey;
	color = 0x999999;
	x = 0;
}

void StonePlatform::respawn()
{
	y = 42.f;
	if(width!=40)
		random();
}


