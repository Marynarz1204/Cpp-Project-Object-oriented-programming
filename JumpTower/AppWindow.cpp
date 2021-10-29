#include <stdio.h>
#include "AppWindow.h"

#define is_down(b) input->button[b].is_down
#define pressed(b) (input->button[b].is_down && input->button[b].change)
#define released(b) (!input->button[b].is_down && input->button[b].change)

#define platforms(p) if (start == true){ \
p[i%10].setSpeed(getDT());\
how_many += (p[i % 10].fall());} \
if (player.collision(p[i % 10].getY() + p[i % 10].getHeight(),p[i % 10].getX() - p[i % 10].getWidth(), p[i % 10].getX() + p[i % 10].getWidth()))\
{\
	score.setScore(i);\
}\
draw_rectangle(p[i % 10].getX(), p[i % 10].getY(), p[i % 10].getWidth(), p[i % 10].getHeight(), p[i % 10].getColor());\


void AppWindow::onCreate()
{
	clear_screen(0);
	draw_player(player);
	platformW[0] = WoodenPlatform(-26.f,40.f, getDT());
	platformW[1] = WoodenPlatform(-9.f, getDT());
	platformW[2] = WoodenPlatform(8.f, getDT());
	platformW[3] = WoodenPlatform(25.f, getDT());
	for (int i = 4; i < 10; i++)
	{
		platformW[i] = WoodenPlatform(42.f,getDT());
	}
	platformS[0] = StonePlatform(42.f, 40.f, getDT());
	for (int i = 1; i < 10; i++)
	{
		platformS[i] = StonePlatform(42.f, getDT());
	}
	platformI[0] = IronPlatform(42.f, 40.f, getDT());
	for (int i = 1; i < 10; i++)
	{
		platformI[i] = IronPlatform(42.f, getDT());
	}
	for (int i = 0; i < 10; i++)
	{
		draw_rectangle(platformW[i].getX(), platformW[i].getY(), platformW[i].getWidth(), platformW[i].getHeight(), platformW[i].getColor());
	}
	draw_rectangle(50, 0, 10, 200, 0x0886bd);
	draw_rectangle(-50, 0, 10, 200, 0x0886bd);
	start = false;
	amount_of_platforms = 30;
	j = 0;
}

void AppWindow::onUpdate(Input* input)
{
	if (player.fail())
	{
		if (is_down(BUTTON_LEFT))
		{
			player.moveLeft();
		}
		if (is_down(BUTTON_RIGHT))
		{
			player.moveRight();
		}
		if (pressed(BUTTON_UP))
		{
			if(player.onGround())
				player.jump();
			start = true;
		}
		player.move(getDT());
		clear_screen(0);
		int how_many = 0;
		for (int i = j; i < j + 5; i++)
		{
			if ((i % amount_of_platforms) < 10)
			{
				platforms(platformW);
			}
			if ((i%amount_of_platforms) >= 10 && (i % amount_of_platforms) < 20)
			{
				platforms(platformS);
			}
			if ((i % amount_of_platforms) >= 20)
			{
				platforms(platformI);
			}
		}
		j += how_many;
		draw_player(player);
		draw_number(score.getScore(),-35,25,.7f,0xffffff);
	}
	else
	{
		draw_number(score.getScore(), -35, 25, .7f, 0xffffff);
		if (score.getScore() != 0)
		{
			draw_text("YOUR SCORE:",-35,18,.8,0xff0000);
			draw_number(score.getScore(), 22, 15.5, 1.1, 0xff0000);
		}
		draw_text("TOP SCORES:",-35,10,.8f,0xff0000);
		draw_number(1, -35, 0, .7f, 0xff0000);
		draw_text("ST:",-32,1,.4f,0xff0000);
		draw_number(scoreboard.get1st(), -22, 0, .7f, 0xff0000);
		draw_number(2, -35, -5, .7f, 0xff0000);
		draw_text("ND:",-32,-4,.4f,0xff0000);
		draw_number(scoreboard.get2nd(), -22, -5, .7f, 0xff0000);
		draw_number(3, -35, -10, .7f, 0xff0000);
		draw_text("RD:",-32,-9,.4f,0xff0000);
		draw_number(scoreboard.get3rd(), -22, -10, .7f, 0xff0000);
		draw_text("PRESS R TO START",-35,-17,0.8f,0xff0000);
	}
	if (pressed(BUTTON_R))
	{
		player.restart();
		scoreboard.setScores(score.getScore());
		score.reset();
		onCreate();
	}
	if (is_down(BUTTON_ESC))
	{
		scoreboard.setScores(score.getScore());
		m_is_run = 0;
	}
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
}

