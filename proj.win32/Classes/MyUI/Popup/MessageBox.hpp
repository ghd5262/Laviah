#pragma once
#include "../Popup.h"

class CMyButton;
class CMessageBox : public CPopup
{
    typedef std::function<void(cocos2d::Node*)> TOUCH_LISTENER;
    
public:
    static CMessageBox* create();
    CMessageBox* addTouchListener(const TOUCH_LISTENER& listener);
    CMessageBox* setLayer(cocos2d::LayerColor* layer);
    CMessageBox* setTailEnable(bool enable);
    virtual CPopup* show(Node* parent, int zOrder = 0) override;
    
    void changeMessage(std::string message);
    
protected:
    virtual bool init() override;
    
private:
    CMessageBox()
    : m_TouchListener(nullptr)
    , m_Layer(nullptr)
    , m_LayerBG(nullptr)
    , m_TailEnable(false){};
    virtual ~CMessageBox(){};
    
private:
    TOUCH_LISTENER m_TouchListener;
    CMyButton* m_Layer;
    cocos2d::LayerColor* m_LayerBG;
    bool m_TailEnable;
};