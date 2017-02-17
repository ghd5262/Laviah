#pragma once
#include "cocos2d.h"
#include <iostream>
#include <vector>
#include <map>

class CTutorialStep{
    typedef std::function<void(void)> START_LISTENER;
public:
    CTutorialStep()
    : m_StartListener(nullptr){}
    ~CTutorialStep(){};
    
    CC_SYNTHESIZE(START_LISTENER, m_StartListener, StartListener);
    CC_SYNTHESIZE(int, m_Index, Index);
};

class CTutorialManager{
    typedef CTutorialStep STEP;
    typedef std::vector<STEP> TUTORIAL;
    typedef std::map<std::string, TUTORIAL*> TUTORIAL_LIST;
    
public:
    static CTutorialManager* Instance();
    void addStepToByTutorialKey(std::string key, STEP step);
    void RunAtFirst(std::string key);
    void RunByIndex(std::string key, int index);
    void End(std::string key = "", int index = 0);
    void Clear();
    
    CC_SYNTHESIZE(int, m_CurrentIndex, CurrentIndex);
    CC_SYNTHESIZE(std::string, m_CurrentKey, CurrentKey);
    
private:
    TUTORIAL* addNewTutorial(std::string key);
    void addStepToTutorial(TUTORIAL* tutorial, STEP step);
    CTutorialManager();
    ~CTutorialManager();
    
private:
    TUTORIAL_LIST m_TutorialList;
};