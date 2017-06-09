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
                          bool tailEnable = true);
    
    void CreateMessageBoxWithRotation(std::string key,
                                      std::string msg,
                                      bool tailEnable = true);
    
    void NextStepAfterDelay(std::string key, float delay);
    
    void CreateBulletPattern(std::string key, int patternIdx);
    
private:
    CTutorialHelper();
    ~CTutorialHelper();
};