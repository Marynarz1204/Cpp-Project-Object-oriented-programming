#include "Score.h"

Score::Score() :score(0) {};
void Score::setScore(int s)
{
	if(s>score)
		score = s;
}

void Score::reset()
{
	score = 0;
}

int Score::getScore()
{
	return score;
}
