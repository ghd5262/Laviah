#pragma once
#include "ui/UIWidget.h"

struct FBUSER_PARAM;
class CFacebookRankPopupDP : public cocos2d::ui::Widget
{
public:
    static CFacebookRankPopupDP* create(const FBUSER_PARAM* user, int number, bool me = false);
    
    void FBShare();
    void TWShare();
    void Notice();
    void Invite();
    
    //setter & getter
    const FBUSER_PARAM* getUserParam() const { return m_User; }
    
    CC_SYNTHESIZE(std::function<void(int)>, m_SelectDPListener, SelectDPListener)
    
protected:
    virtual bool init() override;
    
private:
    
    CFacebookRankPopupDP(const FBUSER_PARAM* user, int number, bool me)
    : m_User(user)
    , m_Number(number)
    , m_SelectDPListener(nullptr)
    , m_IsMyData(me){};
    virtual ~CFacebookRankPopupDP(){};
    
private:
    const FBUSER_PARAM* m_User;
    int m_Number;
    bool m_IsMyData;
};
