#include "GoogleCloudTestAddKeyDP.h"
#include "../MyButton.h"
#include "../TextField.h"
#include "../Popup.h"
#include "../../DataManager/UserDataManager.h"

CGoogleCloudTestAddKeyDP* CGoogleCloudTestAddKeyDP::create()
{
    CGoogleCloudTestAddKeyDP *pRet = new(std::nothrow) CGoogleCloudTestAddKeyDP();
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

bool CGoogleCloudTestAddKeyDP::init()
{
    if (!initVariable())
        return false;
    return true;
}

bool CGoogleCloudTestAddKeyDP::initVariable()
{
    auto dpBack = LayerColor::create(Color4B(0, 0, 0, 0), 1080.f, 200.f);
    if (dpBack != nullptr){
        dpBack->setIgnoreAnchorPointForPosition(false);
        dpBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        dpBack->setPosition(Vec2::ZERO);
        this->addChild(dpBack);
    }
    
    auto dpItemBack = LayerColor::create(COLOR::BRIGHTGRAY_ALPHA, 805.f, 200.f);
    if (dpItemBack != nullptr){
        dpItemBack->setIgnoreAnchorPointForPosition(false);
        dpItemBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        dpItemBack->setPosition(Vec2(0 + dpItemBack->getContentSize().width * 0.5f, 0));
        dpBack->addChild(dpItemBack);
    }
    
    auto dpWhiteBack = LayerColor::create(Color4B(255, 255, 255, 255), dpItemBack->getContentSize().width * 0.9f, dpItemBack->getContentSize().height * 0.7f);
    
    if (dpWhiteBack != nullptr){
        dpWhiteBack->setIgnoreAnchorPointForPosition(false);
        dpWhiteBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        dpWhiteBack->setPosition(Vec2(dpItemBack->getContentSize().width * 0.5f, dpItemBack->getContentSize().height * 0.5f));
        dpItemBack->addChild(dpWhiteBack);
    }
    
    m_TextField = CTextField::create("add key", "fonts/malgunbd.ttf", 40, Vec2(dpWhiteBack->getContentSize().width * 0.5f, dpWhiteBack->getContentSize().height * 0.5f));
    if(m_TextField != nullptr)
    {
        m_TextField->setDimensions(Size(dpWhiteBack->getContentSize().width * 0.9f, dpWhiteBack->getContentSize().height * 0.9f));
        m_TextField->setTextColor(Color4B::BLACK);
        dpWhiteBack->addChild(m_TextField);
    }
    
    
    auto dpAddKeyBtn = CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->Buy(sender);
    })
    ->setLayer(LayerColor::create(COLOR::DARKGRAY_ALPHA, 260, 200))
    ->setContents("ADD KEY")
    ->setFont(COLOR::BRIGHTGRAY, 40)
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(dpBack);
    
    dpAddKeyBtn->setPosition(Vec2(dpBack->getContentSize().width -
                                  (dpAddKeyBtn->getContentSize().width * 0.5f), 0));
    
    return true;
}

void CGoogleCloudTestAddKeyDP::Buy(Node* sender)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    CPopup::create()
    ->setPositiveButton([=](Node* sender){
        //CUserDataManager::Instance()->addKey(m_TextField->getText());
        m_TextField->setText("");
    }, "Yes")
    ->setNegativeButton([=](Node* sender){
    
    }, "No")
    ->setDefaultAnimation(ePOPUP_ANIMATION::OPEN_CENTER, ePOPUP_ANIMATION::CLOSE_CENTER)
    ->setMessage("Are you sure you want to save the key?")
    ->setMessageFont(Color3B::BLACK, 40)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(visibleSize / 2)
    ->show(Director::getInstance()->getRunningScene());
}
