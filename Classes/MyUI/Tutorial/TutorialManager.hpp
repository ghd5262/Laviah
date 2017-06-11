#pragma once
#include "../../Common/HSHUtility.h"
#include <vector>
#include <map>

class CMyButton;
class CMessageBox;
class CTutorialStep;
class CTutorialManager : public cocos2d::Node{
    typedef std::vector<CTutorialStep*>         TUTORIAL;
    typedef std::map<std::string, TUTORIAL*>    TUTORIAL_LIST;
    typedef std::map<std::string, int>          TUTORIAL_SAVED_STEP_LIST;
public:
    static CTutorialManager* Instance();
    void addStep(std::string key, CTutorialStep* tutorial);
    void ChangeTutorial(std::string key);
    void ChangeStep(int index);
    void NextStep();
    void Again();
    void LoadLastSavedStep(std::string key);
    void SaveCurrentStep(std::string key);
    void Clear();
    
    virtual void update(float delta) override;
    
    CC_SYNTHESIZE(bool, m_IsRunning, IsRunning);
    CC_SYNTHESIZE(bool, m_IsRotationEnable, IsRotationEnable);

protected:
    virtual bool init() override;
    
private:
    TUTORIAL* addNewTutorialByKey(std::string key);
    TUTORIAL* getTutorialByKey(std::string key);
    CTutorialStep* getStepFromTutorial(std::string key, int index);
    int getSavedStepIndexByKey(std::string key);
    void stepBegin(std::string key, int index);
    void stepEnd(std::string key, int index);
    void reset();

    CTutorialManager();
    virtual ~CTutorialManager();
    
private:
    static CTutorialManager* m_Instance;
    TUTORIAL_LIST m_TutorialList;
    TUTORIAL_SAVED_STEP_LIST m_SavedStepList;
    TUTORIAL* m_CurrentTutorial;
    std::string m_OriginTutorialKey;
    int m_CurrentStepIndex;
};
