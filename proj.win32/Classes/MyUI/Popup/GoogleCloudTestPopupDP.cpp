#include "GoogleCloudTestPopupDP.h"
#include "GoogleCloudDataInfoPopup.h"
#include "../MyButton.h"


CGoogleCloudTestPopupDP* CGoogleCloudTestPopupDP::create(std::string key)
{
    CGoogleCloudTestPopupDP *pRet = new(std::nothrow) CGoogleCloudTestPopupDP(key);
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

bool CGoogleCloudTestPopupDP::init()
{
    if (!initVariable())
    return false;
    return true;
}

bool CGoogleCloudTestPopupDP::initVariable()
{
    auto dpBack = LayerColor::create(COLOR::TRANSPARENT, 1080.f, 200.f);
    if (dpBack != nullptr){
        this->setContentSize(dpBack->getContentSize());
        
        dpBack->setIgnoreAnchorPointForPosition(false);
        dpBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        dpBack->setPosition(dpBack->getContentSize() * 0.5f);
        this->addChild(dpBack);
    }
    
    auto dpItemBack = LayerColor::create(COLOR::BRIGHTGRAY_ALPHA, 805.f, 200.f);
    if (dpItemBack != nullptr){
        dpItemBack->setIgnoreAnchorPointForPosition(false);
        dpItemBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        dpItemBack->setPosition(Vec2(0 + dpItemBack->getContentSize().width * 0.5f, 0));
        dpBack->addChild(dpItemBack);
    }
    
    auto dpBuyBtn = CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->Buy(sender);
    })
    ->setLayer(LayerColor::create(COLOR::DARKGRAY_ALPHA, 260, 200))
    ->setContents("Info")
    ->setFont(COLOR::BRIGHTGRAY, 40)
    ->show(dpBack);
    
    dpBuyBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    dpBuyBtn->setPosition(Vec2(dpBack->getContentSize().width -
                               (dpBuyBtn->getContentSize().width * 0.5f), 0));
    
    
    auto googleCloudKey = Label::createWithTTF(m_UserKey, "fonts/malgunbd.ttf", 40);
    if (googleCloudKey != nullptr)
    {
        googleCloudKey->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        googleCloudKey->setPosition(Vec2(dpItemBack->getContentSize().width * 0.25f, dpItemBack->getContentSize().height * 0.8f));
        googleCloudKey->setColor(COLOR::BRIGHTGRAY);
        dpItemBack->addChild(googleCloudKey);
    }
    
    auto googleCloudValue = Label::createWithTTF("Key Value", "fonts/malgun.ttf", 35);
    if (googleCloudValue != nullptr)
    {
        googleCloudValue->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        googleCloudValue->setPosition(Vec2(dpItemBack->getContentSize().width * 0.25f, dpItemBack->getContentSize().height * 0.5f));
        googleCloudValue->setColor(COLOR::BRIGHTGRAY);
        dpItemBack->addChild(googleCloudValue);
    }
    
    return true;
}

void CGoogleCloudTestPopupDP::Buy(Node* sender)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto popup = CGoogleCloudDataInfoPopup::create(m_UserKey)
    ->show(Director::getInstance()->getRunningScene(), 102);
    
    popup->setPosition(visibleSize / 2);
    popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
}

void CGoogleCloudTestPopupDP::DeSelect(Node* sender)
{
    
}
