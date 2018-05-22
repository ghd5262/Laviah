#include "TutorialStep.hpp"
#include "TutorialManager.hpp"
#include "../MyButton.h"
//#include <array>

CTutorialStep* CTutorialStep::create()
{
    CTutorialStep *pRet = new(std::nothrow) CTutorialStep();
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

CTutorialStep* CTutorialStep::build(std::string key)
{
    m_TutorialKey = key;
    this->setContentSize(_director->getWinSize());
    
    // if there is message, create message box
    if(m_Message != ""){
        this->createMessageBox();
        m_MessageLayer->setScale(0.f);
        
        this->setOpenAnimation([=](Node* sender){
            auto scale   = ScaleTo::create(0.5f, 1.0f);
            auto elastic = EaseElasticOut::create(scale, 0.5f);
            m_MessageLayer->runAction(elastic);
        });
        
        this->setCloseAnimation([=](Node* sender){
            auto scale   = ScaleTo::create(0.5f, 0.2f);
            auto elastic = EaseElasticIn::create(scale, 1);
            m_MessageLayer->runAction(elastic);
        });
    }
    
    if(m_Button){
        auto messageSize = m_MessageLayer->getContentSize();
        m_Button->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA,
                                              messageSize.width, messageSize.height));
        m_Button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_Button->setPosition(messageSize / 2);
        m_Button->show(m_MessageLayer);
    }
    CTutorialManager::Instance()->addStep(m_TutorialKey, this);
    return this;
}

CTutorialStep* CTutorialStep::addBeginListener(const SINGLE_LISTENER& listener)
{
    m_BeginListener = listener;
    return this;
}

CTutorialStep* CTutorialStep::addUpdateListener(const UPDATE_LISTENER& listener)
{
    m_UpdateListener = listener;
    return this;
}

CTutorialStep* CTutorialStep::addEndListener(const SINGLE_LISTENER& listener)
{
    m_EndListener = listener;
    return this;
}

CTutorialStep* CTutorialStep::addButton(CMyButton* button)
{
    m_Button = button;
    return this;
}

CTutorialStep* CTutorialStep::addMessageBox(std::string message)
{
    m_Message = message;
    return this;
}

CTutorialStep* CTutorialStep::setMessageBoxPosition(cocos2d::Vec2 position)
{
    m_MessageBoxPosition = position;
    return this;
}

CTutorialStep* CTutorialStep::setTailPosition(cocos2d::Vec2 tailPosition)
{
    m_MessageBoxTailPosition = tailPosition;
    return this;
}

CTutorialStep* CTutorialStep::SaveStepEnable(bool enable)
{
    m_SaveStepEnable = enable;
    return this;
}

void CTutorialStep::Begin()
{
    // call begin function
    this->callListener(m_BeginListener);
    
    // Save step
    if(m_SaveStepEnable)
        CTutorialManager::Instance()->SaveCurrentStep(m_TutorialKey);
}

void CTutorialStep::Update(float delta)
{
    m_Time += delta;
    
    // call update function
    if(m_UpdateListener) m_UpdateListener(delta, this);
}

void CTutorialStep::End()
{
    // call end function
    this->callListener(m_EndListener);
}

void CTutorialStep::createMessageBox()
{
    if(m_MessageLayer != nullptr) return;
    
    auto label  = Label::createWithSystemFont(m_Message, FONT::MALGUNBD, 40);
    auto size   = label->getContentSize();
    auto width  = 600;
    auto line   = (int)((size.width / width) + 2);
    line        =  std::max<int>(3, line);
    auto height = line * 65;
    
    m_MessageLayer = LayerColor::create(COLOR::DARKGRAY_ALPHA, width * 1.1f, height * 1.1f);
    m_MessageLayer->setOpacity(255);
    m_MessageLayer->setIgnoreAnchorPointForPosition(false);
    m_MessageLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_MessageLayer->setPosition(m_MessageBoxPosition);
    this->addChild(m_MessageLayer);

    label->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    label->setDimensions(m_MessageLayer->getContentSize().width * 0.9f,
                         m_MessageLayer->getContentSize().height * 0.9f);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->setPosition(m_MessageLayer->getContentSize() / 2);
    m_MessageLayer->addChild(label);
    
    if(m_MessageBoxTailPosition != cocos2d::Vec2::ZERO)
    {
        auto tail = Sprite::create("messageBoxTail.png");
        tail->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        tail->setPosition(Vec2(m_MessageLayer->getContentSize().width * m_MessageBoxTailPosition.x,
                               m_MessageLayer->getContentSize().height * m_MessageBoxTailPosition.y));
        tail->setColor(m_MessageLayer->getColor());
        tail->setOpacity(m_MessageLayer->getOpacity());
        m_MessageLayer->addChild(tail);
    }
    
    auto nextStepIcon = Sprite::create("tutorialNextStepIcon.png");
    nextStepIcon->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    nextStepIcon->setPosition(Vec2(m_MessageLayer->getContentSize().width, 0));
//    nextStepIcon->setOpacity(255 * 0.4f);
    m_MessageLayer->addChild(nextStepIcon);
}

void CTutorialStep::callListener(SINGLE_LISTENER listener)
{
    if(listener)
    {
        this->retain();
        listener(this);
        this->release();
    }
}
