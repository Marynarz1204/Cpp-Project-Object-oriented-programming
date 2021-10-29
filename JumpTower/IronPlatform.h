#pragma once
#include "Platform.h"

class IronPlatform : public Platform
{
public:
	IronPlatform();
	IronPlatform(float, float);
	IronPlatform(float, float, float);
	void random();
	void respawn();
};

