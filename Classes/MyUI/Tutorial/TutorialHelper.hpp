#pragma once
#include "../../Common/HSHUtility.h"
#include "TutorialManager.hpp"
#include "TutorialStep.hpp"
#include <iostream>

namespace TUTORIAL_KEY {
    const std::string BEGINER        = "TUTORIAL_BEGINER";
};

class CTutorialHelper{
public:
    static CTutorialHelper* Instance();
    
    void CreateMessageBox(std::string key,
                          std::string msg,
                          bool tailEnable = true,
                          bool backgroundVisible = false);
    
    void NextStepAfterDelay(std::string key,
                            float delay,
                            bool backgroundVisible = false);
    
    void CreateBulletPattern(std::string key,
                             int tutorialPatternIdx,
                             bool backgroundVisible = false);
    
private:
    CTutorialHelper();
    ~CTutorialHelper();
};