#include "WoodenPlatform.h"

void WoodenPlatform::random()
{
	x = float(rand() % 60) - 30.f;
}
WoodenPlatform::WoodenPlatform()
{
	y = 100.f;
}

WoodenPlatform::WoodenPlatform(float basey,float s)
{
	speed = s * 400;
	width = 10; 
	height = 1;
	y = basey;
	color = 0xffff00;
	random();
}

WoodenPlatform::WoodenPlatform(float basey, float milestone,float s)
{
	speed = s * 400;
	width = milestone;
	height = 1;
	y = basey;
	color = 0xffff00;
	x = 0;
}

void WoodenPlatform::respawn()
{
	y = 42.f;
	if(width!=40)
		random();
}


