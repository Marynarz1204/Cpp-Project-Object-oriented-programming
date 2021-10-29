#include "Player.h"

Player::Player() :x(0), y(-45.f), height(5), width(5), color(0xff0000), velocityX(0), accelerationX(0), velocityY(0), accelerationY(0) {}

float Player::getX()
{
	return x;
}

float Player::getY()
{
	return y;
}

float Player::getHeight()
{
	return height;
}

float Player::getWidth()
{
	return width;
}

u32 Player::getColor()
{
	return color;
}

bool Player::onGround() 
{
	if (velocityY == 0)
		return true;
	return false;
}

void Player::jump()
{
	accelerationY = 1700.f;
}

void Player::moveRight()
{
	accelerationX = 2000.f;
}

void Player::moveLeft()
{
	accelerationX = -2000.f;
}

void Player::move(float dt)
{
	accelerationX -= velocityX * 10.f;
	x += velocityX * dt + accelerationX * dt * dt * .5f;
	if (x > 37.5)
	{
		x = 37.5;
	}
	if (x < -37.5)
	{
		x = -37.5;
	}
	velocityX += accelerationX * dt;
	accelerationX = 0.f;
	y += velocityY * dt + accelerationY * dt * dt * .5f;
	velocityY += accelerationY * dt;
	accelerationY -= 15000.f * dt;
	if (accelerationY < -1000.f)
	{
		accelerationY = -1000.f;
	}
	if (velocityY < -1000.f)
	{
		velocityY = -1000.f;
	}
}

bool Player::collision(float platformY, float platformLeftX, float platformRightX)
{
	if (y - height / 2 < platformY && x - width / 2 < platformRightX && x + width / 2 > platformLeftX && platformY - y + height / 2 < 5.f && velocityY<=0)
	{
		y = platformY + height/2 - .5f;
		accelerationY = 0;
		velocityY = 0;
		return true;
	}
	return false;
}

bool Player::fail()
{
	if (y < -30)
	{
		return false;
	}
	return true;
}

void Player::restart()
{
	x = 0;
	y = -22.5f;
	velocityX = 0;
	velocityY = 0;
	accelerationX = 0;
	accelerationY = 0;
}

