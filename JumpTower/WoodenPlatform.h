#pragma once
#include "Platform.h"

class WoodenPlatform: public Platform
{
public:
	WoodenPlatform();
	WoodenPlatform(float, float s);
	WoodenPlatform(float, float, float s);
	void random();
	void respawn();
};

