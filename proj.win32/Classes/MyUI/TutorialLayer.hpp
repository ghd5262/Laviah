#pragma once
#include "Popup.h"
#include <vector>
#include <map>

enum TUTORIAL_EVENT {
    TOUCN_BEGIN,
    TOUCH_EXECUTE,
    TOUCH_END,
    BEGIN,
    UPDATE,
    END
};

class CMyButton;
class CTutorialStep;
class CTutorialObject;
class CMessageBox;
class CTutorialLayer : public CPopup{
    typedef std::map<std::string, CTutorialObject*> TUTORIAL_LIST;
    
public:
    static CTutorialLayer* Instance();
    virtual void update(float delta) override;
    void addStepToByTutorialKey(std::string key, CTutorialStep* step);
    void ChangeTutorial(std::string key);
    void ChangeStep(int index);
    void NextStep();

    void Again();
    void Clear();
    
    CC_SYNTHESIZE(bool, m_IsRunning, IsRunning);
protected:
    virtual bool init() override;
    
private:
    CTutorialObject* addNewTutorialObject(std::string key);
    void addStepToTutorialObject(CTutorialObject* tutorial,
                                 CTutorialStep* step);
    CTutorialObject* getTutorialObjectByKey(std::string key);
    
    CTutorialLayer();
    virtual ~CTutorialLayer();
    
private:
    static CTutorialLayer* m_Instance;
    TUTORIAL_LIST m_TutorialList;
    CTutorialObject* m_CurrentTutorial;
};

class CTutorialObject : public cocos2d::Node{
    typedef std::vector<CTutorialStep*> STEP_LIST;
    typedef std::function<void(cocos2d::Node*)> TUTORIAL_LISTENER;
    
public:
    static CTutorialObject* create();
    CTutorialObject* addBeginListener(const TUTORIAL_LISTENER& listener);
    CTutorialObject* addEndListener(const TUTORIAL_LISTENER& listener);
    CTutorialObject* addTutorialStep(CTutorialStep* step);
    CTutorialObject* build(std::string key);

    virtual void update(float delta) override;
    std::string getTutorialKey() const { return m_TutorialKey; }
    CTutorialStep* getStepByIndex(int index) const;
    
    void Begin();
    void End();
    void NextStep();
    void ChangeStep(int index);
    void Again();


    CC_SYNTHESIZE(TUTORIAL_LISTENER, m_BeginListener, BeginListener);
    CC_SYNTHESIZE(TUTORIAL_LISTENER, m_EndListener, EndListener);
    CC_SYNTHESIZE(int , m_CurrentStepIndex, CurrentStepIndex);

private:
    void callListener(TUTORIAL_LISTENER listener);
    void clear();
    
    CTutorialObject()
    : m_BeginListener(nullptr)
    , m_EndListener(nullptr)
    , m_TutorialKey("")
    , m_CurrentStepIndex(0){}
    virtual ~CTutorialObject(){};
    
private:
    STEP_LIST m_StepList;
    std::string m_TutorialKey;
};


class CTutorialStep : public cocos2d::Node{
    typedef std::function<void(cocos2d::Node*)> STEP_LISTENER;
    
public:
    static CTutorialStep* create();
    CTutorialStep* addEventListener(const STEP_LISTENER& listener, TUTORIAL_EVENT type);
    CTutorialStep* addMessageBox(cocos2d::Node* parent, std::string message);
    CTutorialStep* build(std::string tutorialKey);
    
    void Begin();
    void Update() { this->callListener(m_UpdateListener);   };
    void End()    { this->callListener(m_EndListener);      };
    
    CC_SYNTHESIZE(STEP_LISTENER, m_BeginListener, BeginListener);
    CC_SYNTHESIZE(STEP_LISTENER, m_UpdateListener, UpdateListener);
    CC_SYNTHESIZE(STEP_LISTENER, m_EndListener, EndListener);
protected:
    virtual bool init() override;
    
private:
    void callListener(STEP_LISTENER listener);
    void createEventListener();
    void clear();

    CTutorialStep()
    : m_MessageBox(nullptr)
    , m_EventListener(nullptr)
    , m_BeginListener(nullptr)
    , m_UpdateListener(nullptr)
    , m_EndListener(nullptr)
    , m_MessageBoxPosition(Vec2::ZERO)
    , m_CurrentMessageIndex(0){}
    virtual ~CTutorialStep(){};
    
private:
    CMessageBox* m_MessageBox;
    CMyButton* m_EventListener;
    std::vector<std::string> m_MessageList;
    cocos2d::Vec2 m_MessageBoxPosition;
    int m_CurrentMessageIndex;
};