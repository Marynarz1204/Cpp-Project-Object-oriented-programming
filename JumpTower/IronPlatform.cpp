#include "IronPlatform.h"

void IronPlatform::random()
{
	x = float(rand() % 66) - 33;
}
IronPlatform::IronPlatform()
{
	y = 100.f;
}

IronPlatform::IronPlatform(float basey, float s)
{
	speed = s * 100;
	width = 7;
	height = 1;
	y = basey;
	color = 0xFFFFFF;
	random();
}

IronPlatform::IronPlatform(float basey, float milestone, float s)
{
	speed = s * 100;
	width = milestone;
	height = 1;
	y = basey;
	color = 0xffffff;
	x = 0;
}

void IronPlatform::respawn()
{
	y = 42.f;
	if(width!=40)
		random();
}


