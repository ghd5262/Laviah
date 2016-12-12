#pragma once

#include <stdio.h>

class Global{
public:
    static Global* Instance();
    void Clear();
    
private:
    Global();
    ~Global(){};
    
public:
    int BONUSTIME;        
    int ALIENGET;         
    int CHALLENGECLEAR;   
    int TOTALSCORE;       
    int STARSCORE;        
    int COINSCORE;        
    int RUNSCORE;
};

#define GLOBAL Global::Instance()