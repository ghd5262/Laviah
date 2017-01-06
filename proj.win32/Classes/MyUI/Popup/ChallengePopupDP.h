#pragma once
#include "cocos2d.h"
#include "../Popup.h"
#include "../../DataManager/ChallengeDataManager.hpp"

class CChallengePopupDP : public CPopup
{
    typedef std::function<void(CChallengePopupDP*, int)> SKIP_CALLBACK;
public:
    static CChallengePopupDP* create(const sCHALLENGE_PARAM challenge, int posIndex);
    CPopup* addSkipEventListner(const SKIP_CALLBACK &callback);
    
    //setter & getter
    const sCHALLENGE_PARAM getChallengeParam() const { return m_Challenge; }
    
    CC_SYNTHESIZE(int, m_PosIndex, PosIndex);
protected:
    virtual bool init() override;
    bool initVariable();
    
private:
    float getPercent(float value, float max);
    CChallengePopupDP(const sCHALLENGE_PARAM challenge, int posIndex)
    : m_Challenge(challenge)
    , m_SkipCallback(nullptr)
    , m_PosIndex(posIndex){};
    virtual ~CChallengePopupDP(){};
    
private:
    const sCHALLENGE_PARAM m_Challenge;
    SKIP_CALLBACK m_SkipCallback;
};