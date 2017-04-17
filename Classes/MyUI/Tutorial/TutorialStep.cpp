#include "TutorialStep.hpp"
#include "TutorialManager.hpp"
#include "../MyButton.h"

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

    if(m_TouchListener)
    {
        auto btn = CMyButton::create()
        ->addEventListener([=](Node* sender){
            CCLOG("Tutorial step touched");
            this->retain();
            if(m_TouchListener){
                m_TouchListener(this);
            }
            this->release();
        })
        ->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
        ->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080, 1920))
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->setButtonPosition(this->getContentSize() / 2)
        ->show(this);
        
        btn->setSwallowTouches(false);
    }
    
    // if there is message, create message box
    if(m_Message != ""){
        this->createMessageBox();
        m_MessageLayer->setScale(0.f);
        this->setOpenAnimation([=](Node* sender){
            m_MessageLayer->runAction(EaseElasticOut::create(ScaleTo::create(0.5f, 1.0f), 0.5f));
        });
        
        this->setCloseAnimation([=](Node* sender){
            auto scale   = ScaleTo::create(0.5f, 0.2f);
            auto elastic = EaseElasticIn::create(scale, 1);
            m_MessageLayer->runAction(elastic);
            this->popupTouchEnable(true);
        });
    }
    
    CTutorialManager::Instance()->addTutorial(m_TutorialKey, this);
    return this;
}

CTutorialStep* CTutorialStep::addMessageBox(std::string message, bool tailEnable/* = false*/)
{
    m_Message = message;
    m_MessageBoxTail = tailEnable;
    return this;
}

CTutorialStep* CTutorialStep::addTouchListener(const SINGLE_LISTENER& listener)
{
    m_TouchListener = listener;
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

void CTutorialStep::Begin()
{
    // call begin function
    this->callListener(m_BeginListener);
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
    
    m_MessageLayer = LayerColor::create(COLOR::DARKGRAY_ALPHA, width, height);
    m_MessageLayer->setOpacity(255);
    m_MessageLayer->setIgnoreAnchorPointForPosition(false);
    m_MessageLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_MessageLayer->setPosition(Vec2(this->getContentSize().width * 0.5f,
                                     this->getContentSize().height * 0.65f));
    this->addChild(m_MessageLayer);

    label->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    label->setDimensions(m_MessageLayer->getContentSize().width * 0.9f,
                         m_MessageLayer->getContentSize().height * 0.9f);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->setPosition(m_MessageLayer->getContentSize() / 2);
    m_MessageLayer->addChild(label);
    
    if(m_MessageBoxTail)
    {
        auto tail = Sprite::create("messageBoxTail.png");
        tail->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        tail->setPosition(Vec2(m_MessageLayer->getContentSize().width * 0.5f, 0));
        tail->setColor(m_MessageLayer->getColor());
        tail->setOpacity(m_MessageLayer->getOpacity());
        m_MessageLayer->addChild(tail);
    }
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