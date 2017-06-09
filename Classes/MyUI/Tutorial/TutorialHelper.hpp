#pragma once
#include "../MyButton.h"
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
                          eMYBUTTON_STATE action = eMYBUTTON_STATE::END);
    
    void NextStepAfterDelay(std::string key, float delay);
    
    void CreateBulletPattern(std::string key, int patternIdx);
    
    void RotationEnable(std::string key, bool enable);
    
private:
    CTutorialHelper();
    ~CTutorialHelper();
};