#include "../Popup.h"

class CMyButton;
class CTutorialStep : public CPopup {
    typedef std::function<void(CTutorialStep*)>        SINGLE_LISTENER;
    typedef std::function<void(float, CTutorialStep*)> UPDATE_LISTENER;
    
public:
    static CTutorialStep* create();
    CTutorialStep* addBeginListener(const SINGLE_LISTENER& listener);
    CTutorialStep* addUpdateListener(const UPDATE_LISTENER& listener);
    CTutorialStep* addEndListener(const SINGLE_LISTENER& listener);
    CTutorialStep* addButton(CMyButton* button);
    CTutorialStep* addMessageBox(std::string message, bool tailEnable = false);
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
    : m_BeginListener(nullptr)
    , m_EndListener(nullptr)
    , m_UpdateListener(nullptr)
    , m_Button(nullptr)
    , m_MessageLayer(nullptr)
    , m_TutorialKey("")
    , m_Message("")
    , m_Time(0.f)
    , m_MessageBoxTail(false){}

public:
    virtual ~CTutorialStep(){};

private:
    SINGLE_LISTENER m_BeginListener;
    SINGLE_LISTENER m_EndListener;
    UPDATE_LISTENER m_UpdateListener;
    CMyButton* m_Button;
    std::string m_TutorialKey;
    std::string m_Message;
    bool m_MessageBoxTail;
};