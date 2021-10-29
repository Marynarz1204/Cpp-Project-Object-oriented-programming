#pragma once
#include "utility.cpp"
#include <stdlib.h>
#include <time.h> 

class Platform
{
protected:
	float x;
	float y;
	float width;
	float height;
	float speed;
	u32 color;
public:
	float getTopX();
	float getX();
	float getY();
	float getWidth();
	float getHeight();
	u32 getColor();
	virtual void random() = 0;
	int fall();
	virtual void respawn();
	void setSpeed(float s);
};

