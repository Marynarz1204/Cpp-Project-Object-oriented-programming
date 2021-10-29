#pragma once
#include "Window.h"
#include "Player.h"
#include "WoodenPlatform.h"
#include "StonePlatform.h"
#include "IronPlatform.h"
#include "Score.h"
#include "Scoreboard.h"

class AppWindow: public Window
{
private:
	bool start;
	Player player;
	WoodenPlatform platformW[10];
	StonePlatform platformS[10];
	IronPlatform platformI[10];
	Score score;
	Scoreboard scoreboard;
	int amount_of_platforms;
	int j;
public:
	virtual void onCreate() override;
	virtual void onUpdate(Input*) override;
	virtual void onDestroy() override;
};

