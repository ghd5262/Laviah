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
    
public:
    static CTutorialManager* Instance();
    void addTutorial(std::string key, CTutorialStep* tutorial);
    void ChangeTutorial(std::string key);
    void ChangeStep(int index);
    void NextStep();
    void Again();
    void Clear();
    
    virtual void update(float delta) override;
    
    CC_SYNTHESIZE(bool, m_IsRunning, IsRunning);
protected:
    virtual bool init() override;
    
private:
    TUTORIAL* addNewTutorialByKey(std::string key);
    TUTORIAL* getTutorialByKey(std::string key);
    CTutorialStep* getStepFromTutorial(std::string key, int index);
    void stepBegin(std::string key, int index);
    void stepEnd(std::string key, int index);
    void reset();

    CTutorialManager();
    virtual ~CTutorialManager();
    
private:
    static CTutorialManager* m_Instance;
    TUTORIAL_LIST m_TutorialList;
    TUTORIAL* m_CurrentTutorial;
    int m_CurrentStepIndex;
};
