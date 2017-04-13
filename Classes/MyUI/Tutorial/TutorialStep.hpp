#include "../Popup.h"

class CTutorialStep : public CPopup {
    typedef std::function<void(CTutorialStep*)>        SINGLE_LISTENER;
    typedef std::function<void(float, CTutorialStep*)> UPDATE_LISTENER;
    
public:
    static CTutorialStep* create();
    CTutorialStep* addTouchListener(const SINGLE_LISTENER& listener);
    CTutorialStep* addBeginListener(const SINGLE_LISTENER& listener);
    CTutorialStep* addUpdateListener(const UPDATE_LISTENER& listener);
    CTutorialStep* addEndListener(const SINGLE_LISTENER& listener);
    CTutorialStep* addMessageBox(std::string message);
    CTutorialStep* build(std::string key);
    
    void Begin();
    void Update(float delta);
    void End();
    
    std::string getTutorialKey() const { return m_TutorialKey; }
    
    CC_SYNTHESIZE_READONLY(cocos2d::LayerColor*, m_MessageLayer, MessageLayer);
    CC_SYNTHESIZE(float, m_Time, Time);
private:
    void createMessageBox();
    void callListener(SINGLE_LISTENER listener);
    void clear();
    
    CTutorialStep()
    : m_TouchListener(nullptr)
    , m_BeginListener(nullptr)
    , m_EndListener(nullptr)
    , m_UpdateListener(nullptr)
    , m_MessageLayer(nullptr)
    , m_TutorialKey("")
    , m_Message("")
    , m_Time(0.f){}
    virtual ~CTutorialStep(){};
    
private:
    SINGLE_LISTENER m_TouchListener;
    SINGLE_LISTENER m_BeginListener;
    SINGLE_LISTENER m_EndListener;
    UPDATE_LISTENER m_UpdateListener;
    std::string m_TutorialKey;
    std::string m_Message;
};