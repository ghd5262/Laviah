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
                          bool backgroundVisible,
                          bool tailEnable);
    
    void NextStepAfterDelay(std::string key,
                            bool backgroundVisible,
                            float delay);
    
    void CreateBulletPattern(std::string key,
                             bool backgroundVisible,
                             int tutorialPatternIdx);
    
private:
    CTutorialHelper();
    ~CTutorialHelper();
};