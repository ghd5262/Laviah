#pragma once
#include "Popup.h"
#include <vector>
#include <map>

class CMyButton;
class CMessageBox;
class CTutorialObject;
class CTutorialLayer : public CPopup{
    typedef std::vector<CTutorialObject*>       TUTORIAL;
    typedef std::map<std::string, TUTORIAL*>    TUTORIAL_LIST;
    
public:
    static CTutorialLayer* Instance();
    void addTutorial(std::string key, CTutorialObject* tutorial);
    void ChangeTutorial(std::string key);
    void ChangeStep(int index);
    void NextStep();
    void Again();
    
    virtual void update(float delta) override;
    
    CC_SYNTHESIZE(bool, m_IsRunning, IsRunning);
protected:
    virtual bool init() override;
    
private:
    TUTORIAL* addNewTutorialByKey(std::string key);
    TUTORIAL* getTutorialByKey(std::string key);
    CTutorialObject* getStepFromTutorial(std::string key, int index);
    void stepBegin(std::string key, int index);
    void stepEnd(std::string key, int index);
    void clear();

    CTutorialLayer();
    virtual ~CTutorialLayer();
    
private:
    static CTutorialLayer* m_Instance;
    TUTORIAL_LIST m_TutorialList;
    TUTORIAL* m_CurrentTutorial;
    int m_CurrentStepIndex;
};

class CTutorialObject : public cocos2d::Node{
    typedef std::function<void(cocos2d::Node*)> SINGLE_LISTENER;
    typedef std::function<void(float)>          UPDATE_LISTENER;

public:
    static CTutorialObject* create();
    CTutorialObject* addTouchListener(const SINGLE_LISTENER& listener);
    CTutorialObject* addBeginListener(const SINGLE_LISTENER& listener);
    CTutorialObject* addUpdateListener(const UPDATE_LISTENER& listener);
    CTutorialObject* addEndListener(const SINGLE_LISTENER& listener);
    CTutorialObject* addMessageBox(std::string message);
    CTutorialObject* setTouchEnable(bool enable);
    CTutorialObject* build(std::string key);

    void Begin();
    void Update(float delta);
    void End();

    std::string getTutorialKey() const { return m_TutorialKey; }

private:
    void createMessageBox();
    void callListener(SINGLE_LISTENER listener);
    void clear();
    
    CTutorialObject()
    : m_TouchListener(nullptr)
    , m_BeginListener(nullptr)
    , m_EndListener(nullptr)
    , m_UpdateListener(nullptr)
    , m_MessageBox(nullptr)
    , m_TutorialKey("")
    , m_Message("")
    , m_TouchEnable(true){}
    virtual ~CTutorialObject(){};
    
private:
    SINGLE_LISTENER m_TouchListener;
    SINGLE_LISTENER m_BeginListener;
    SINGLE_LISTENER m_EndListener;
    UPDATE_LISTENER m_UpdateListener;
    CMessageBox* m_MessageBox;
    std::string m_TutorialKey;
    std::string m_Message;
    bool m_TouchEnable;
};