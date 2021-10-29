#pragma once

using namespace std;

class Scoreboard
{
private:
	int first;
	int second;
	int third;
public:
	int get1st();
	int get2nd();
	int get3rd();
	void setScores(int score);
	Scoreboard();
	~Scoreboard();
};

