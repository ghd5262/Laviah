#pragma once
#include "cocos2d.h"
#include <iostream>
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
class CTutorialStep : public cocos2d::Node{
    typedef std::function<void(cocos2d::Node*)> TUTORIAL_LISTENER;
    
public:
    static CTutorialStep* create();
    CTutorialStep* addEventListener(const TUTORIAL_LISTENER& listener, TUTORIAL_EVENT type);
    CTutorialStep* addBubble(cocos2d::LayerColor* layer, std::string content, cocos2d::Node* anchor);
    CTutorialStep* build(std::string tutorialKey);
    
    void Begin()  { this->callListener(m_BeginListener);    };
    void Update() { this->callListener(m_UpdateListener);   };
    void End()    { this->callListener(m_EndListener);      };
    
    CC_SYNTHESIZE(TUTORIAL_LISTENER, m_BeginListener, BeginListener);
    CC_SYNTHESIZE(TUTORIAL_LISTENER, m_UpdateListener, UpdateListener);
    CC_SYNTHESIZE(TUTORIAL_LISTENER, m_EndListener, EndListener);
private:
    void callListener(TUTORIAL_LISTENER listener);
    
    CTutorialStep()
    : m_EventListener(nullptr)
    , m_BeginListener(nullptr)
    , m_UpdateListener(nullptr)
    , m_EndListener(nullptr){}
    virtual ~CTutorialStep(){};
    
private:
    CMyButton* m_EventListener;
};

class CTutorialManager : public cocos2d::Node{
    typedef CTutorialStep STEP;
    typedef std::vector<STEP*> TUTORIAL;
    typedef std::map<std::string, TUTORIAL*> TUTORIAL_LIST;
    
public:
    static CTutorialManager* Instance();
    virtual void update(float delta) override;
    void addStepToByTutorialKey(std::string key, STEP* step);
    void ChangeStep(std::string key, int index = -1);
    void Again();
    void Clear();
    
    CC_SYNTHESIZE(bool, m_IsRunning, IsRunning);
    CC_SYNTHESIZE(int , m_CurrentIndex, CurrentIndex);
    CC_SYNTHESIZE(std::string, m_CurrentKey, CurrentKey);
protected:
    virtual bool init() override;
    
private:
    TUTORIAL* addNewTutorial(std::string key);
    void addStepToTutorial(TUTORIAL* tutorial, STEP* step);
    CTutorialManager();
    virtual ~CTutorialManager();
    
private:
    static CTutorialManager* m_Instance;
    STEP* m_CurrentStep;
    TUTORIAL_LIST m_TutorialList;
};