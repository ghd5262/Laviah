#include "MessageBox.hpp"
#include "../MyButton.h"
#include "../../Scene/GameScene.h"

CMessageBox* CMessageBox::create()
{
    CMessageBox *pRet = new(std::nothrow) CMessageBox();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool CMessageBox::init()
{
    if (!CPopup::init()) return false;
    return true;
}

CPopup* CMessageBox::show(Node* parent, int zOrder/* = 0*/)
{
    if(m_LayerBG)
    {
        this->setContentSize(m_LayerBG->getContentSize());
        
        m_Layer = CMyButton::create()
        ->addEventListener([=](Node* sender){
            this->retain();
            if(m_TouchListener) m_TouchListener(this);
            this->release();
        })
        ->setLayer(m_LayerBG)
        ->setEnableSound(false)
        ->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
        ->setContents(this->getMessage())
        ->setButtonPosition(this->getContentSize() / 2)
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->show(this);
    }
    
    if(m_TailEnable)
    {
        
    }
    
    return 	CPopup::show(parent, zOrder);
}

CMessageBox* CMessageBox::addTouchListener(const TOUCH_LISTENER& listener)
{
    m_TouchListener = listener;
    return this;
}

CMessageBox* CMessageBox::setLayer(cocos2d::LayerColor* layer)
{
    m_LayerBG = layer;
    return this;
}

CMessageBox* CMessageBox::setTailEnable(bool enable)
{
    m_TailEnable = enable;
    return this;
}

void CMessageBox::changeMessage(std::string message)
{
    m_Layer->changeContents(message);
}

