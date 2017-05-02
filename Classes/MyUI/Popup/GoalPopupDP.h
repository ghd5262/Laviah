#pragma once
#include "cocos2d.h"
#include "../Popup.h"
#include "../../DataManager/AchievementDataManager.hpp"

class CGoalPopupDP : public CPopup
{
    typedef std::function<void(CGoalPopupDP*, int)> SKIP_CALLBACK;
public:
    static CGoalPopupDP* create(const sACHIEVEMENT_PARAM achievement, int posIndex);
    CGoalPopupDP* addSkipEventListner(const SKIP_CALLBACK &callback);
    
    //setter & getter
    const sACHIEVEMENT_PARAM getAchievementParam() const { return m_Achievement; }
    
    CC_SYNTHESIZE(int, m_PosIndex, PosIndex);
protected:
    virtual bool init() override;
    
private:
    float getPercent(float value, float max);
    CGoalPopupDP(const sACHIEVEMENT_PARAM achievement, int posIndex)
    : m_Achievement(achievement)
    , m_SkipCallback(nullptr)
    , m_PosIndex(posIndex){};
    virtual ~CGoalPopupDP(){};
    
private:
    const sACHIEVEMENT_PARAM m_Achievement;
    SKIP_CALLBACK m_SkipCallback;
};