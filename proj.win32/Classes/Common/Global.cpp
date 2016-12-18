#include "Global.hpp"

Global::Global()
{
    this->Clear();
}

void Global::Clear()
{
    BONUSTIME = 0;
    COMBO = 0;
    CHALLENGECLEAR = 0;
    TOTALSCORE = 0;
    STARSCORE = 0;
    COINSCORE = 0;
    RUNSCORE = 0;
}

Global* Global::Instance()
{
    static Global instance;
    return &instance;
}

