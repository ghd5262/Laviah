#pragma once
#include "cocos2d.h"
#include "../Popup.h"
#include "../../DataManager/ChallengeDataManager.hpp"

class CChallengePopupDP : public CPopup
{
    typedef std::function<void(CChallengePopupDP*)> SKIP_CALLBACK;
public:
    static CChallengePopupDP* create(const sCHALLENGE_PARAM challenge);
    CPopup* addSkipEventListner(const SKIP_CALLBACK &callback);
    
    //setter & getter
    const sCHALLENGE_PARAM getChallengeParam() const { return m_Challenge; }
    
protected:
    virtual bool init() override;
    bool initVariable();
    
private:
    
    CChallengePopupDP(const sCHALLENGE_PARAM challenge)
    : m_Challenge(challenge)
    , m_SkipCallback(nullptr){};
    virtual ~CChallengePopupDP(){};
    
private:
    const sCHALLENGE_PARAM m_Challenge;
    SKIP_CALLBACK m_SkipCallback;
};