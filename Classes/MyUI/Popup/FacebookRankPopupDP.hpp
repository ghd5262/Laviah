#pragma once
#include "cocos2d.h"
#include "../../Facebook/FacebookManager.hpp"

USING_NS_CC;

class CFacebookRankPopupDP : public cocos2d::ui::Widget
{
public:
    static CFacebookRankPopupDP* create(const FBUSER_PARAM* user, int number);
    
    void Share();
    void Notice();
    void Invite();
    
    //setter & getter
    const FBUSER_PARAM* getUserParam() const { return m_User; }
    
    CC_SYNTHESIZE(std::function<void(int)>, m_SelectDPListener, SelectDPListener)
    
protected:
    virtual bool init() override;
    
private:
    
    CFacebookRankPopupDP(const FBUSER_PARAM* user, int number)
    : m_User(user)
    , m_Number(number)
    , m_SelectDPListener(nullptr){};
    virtual ~CFacebookRankPopupDP(){};
    
private:
    const FBUSER_PARAM* m_User;
    int m_Number;
};
