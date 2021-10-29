#pragma once
#include "Platform.h"

class StonePlatform : public Platform
{
public:
	StonePlatform();
	StonePlatform(float, float);
	StonePlatform(float, float, float);
	void random();
	void respawn();
};

