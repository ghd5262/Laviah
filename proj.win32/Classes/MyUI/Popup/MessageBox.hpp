#pragma once
#include "../Popup.h"

class CMyButton;
class CMessageBox : public CPopup
{
public:
    static CMessageBox* create();
    CMessageBox* setLayer(cocos2d::LayerColor* layer);
    CMessageBox* setTailEnable(bool enable);
    virtual CPopup* show(Node* parent, int zOrder = 0) override;
    
    void changeMessage(std::string message);
    
protected:
    virtual bool init() override;
    
private:
    CMessageBox()
    : m_Layer(nullptr)
    , m_LayerBG(nullptr)
    , m_TailEnable(false){};
    virtual ~CMessageBox(){};
    
private:
    CMyButton* m_Layer;
    cocos2d::LayerColor* m_LayerBG;
    bool m_TailEnable;
};