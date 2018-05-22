#pragma once
#include "../MyButton.h"
#include "../../Common/HSHUtility.h"
#include "TutorialManager.hpp"
#include "TutorialStep.hpp"
#include <iostream>

namespace TUTORIAL_KEY {
    const std::string BEGINER        = "TUTORIAL_BEGINER";
    const std::string REVIVE         = "TUTORIAL_REVIVE";
};

class CTutorialHelper{
public:
    static CTutorialHelper* Instance();
    
    void CreateMessageBox(std::string key,
                          std::string msg);
    
    void CreateMessageBox(std::string key,
                          std::string msg,
                          cocos2d::Vec2 pos,
                          cocos2d::Vec2 tailPos,
                          eMYBUTTON_STATE action);
    
    void NextStepAfterDelay(std::string key, float delay);
    
    void CreateBulletPattern(std::string key, int patternIdx);
    
    void RotationEnable(std::string key, bool enable);
    
private:
    CTutorialHelper();
    ~CTutorialHelper();
};