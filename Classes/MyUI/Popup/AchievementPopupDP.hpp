#pragma once
#include "cocos2d.h"
#include "../../Common/HSHUtility.h"

USING_NS_CC;
struct ACHIEVEMENT;
class CMyButton;
class CAchievementPopupDP : public cocos2d::ui::Widget
{
public:
    static CAchievementPopupDP* create(const ACHIEVEMENT* data);
    
    void Reward();
    
    //setter & getter
    const ACHIEVEMENT* getAchievementData() const { return m_AchievementData; }
    
    CC_SYNTHESIZE(std::function<void(int)>, m_SelectDPListener, SelectDPListener)
    
protected:
    virtual bool init() override;
    
private:
    
    CAchievementPopupDP(const ACHIEVEMENT* data)
    : m_AchievementData(data)
    , m_SelectDPListener(nullptr)
    , m_RewardBtn(nullptr){};
    virtual ~CAchievementPopupDP(){};
    
private:
    const ACHIEVEMENT* m_AchievementData;
    CMyButton* m_RewardBtn;
};
