#include "Scoreboard.h"
#include <Windows.h>
#include <fstream>

int Scoreboard::get1st()
{
    return first;
}

int Scoreboard::get2nd()
{
    return second;
}

int Scoreboard::get3rd()
{
    return third;
}

Scoreboard::Scoreboard()
{
    ifstream load;
    load.open("ScoreBoard.dat", ios::out | ios::binary);
    first = 0;
    second = 0;
    third = 0;
    if (!load.eof())
    {
        load >> first >> second >> third;
    }
    load.close();
}

Scoreboard::~Scoreboard()
{
    ofstream save;
    save.open("ScoreBoard.dat", ios::out | ios::binary | ios::trunc);
    save << first << endl << second << endl << third << endl;
}

void Scoreboard::setScores(int score)
{
    if (score > first)
    {
        third = second;
        second = first;
        first = score;
    }
    else if (score > second)
    {
        third = second;
        second = score;
    }
    else if (score > third)
    {
        third = score;
    }
}
