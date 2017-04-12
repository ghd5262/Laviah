#pragma once
#include "../../Common/HSHUtility.h"
#include "TutorialManager.hpp"
#include "TutorialStep.hpp"
#include <iostream>

namespace TUTORIAL_KEY {
    std::string BEGINER        = "TUTORIAL_BEGINER";
};

class CTutorialHelper{
public:
    static CTutorialHelper* Instance();
    
    void CreateMessageBox(std::string msg, std::string key);
    void NextStepAfterDelay(float delay, std::string key);
    
private:
    CTutorialHelper();
    ~CTutorialHelper();
};