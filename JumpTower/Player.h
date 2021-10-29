#pragma once
#include "utility.cpp"


class Player
{
private:
	float x;
	float y;
	float height;
	float width;
	float velocityX;
	float accelerationX;
	float velocityY;
	float accelerationY;
	u32 color;
public:
	Player();
	float getX();
	float getY();
	float getHeight();
	float getWidth();
	u32 getColor();
	bool onGround();
	void jump();
	void moveRight();
	void moveLeft();
	void move(float);
	bool collision(float, float, float);
	bool fail();
	void restart();
};

