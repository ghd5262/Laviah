#include "Popup.h"
#include "MyButton.h"
#include "../Scene/GameScene.h"
#include "../Scene/LoadingScene.h"

using namespace cocos2d;
using namespace cocos2d::ui;

CPopup::CALLBACK_STACK CPopup::m_DefaultCallbackStack;

CPopup::CPopup()
: m_PositiveButtonCallBack(nullptr)
, m_NegativeButtonCallBack(nullptr)
, m_DefaultCallBack(nullptr)
, m_OpenAnimationCallBack(nullptr)
, m_CloseAnimationCallBack(nullptr)
, m_EmptyBackground(nullptr)
, m_TouchDisable(nullptr)
, m_Message("")
, m_PositiveButtonName("")
, m_NegativeButtonName("")
, m_MessageFontColor(COLOR::DARKGRAY)
, m_ButtonFontColor(Color3B::WHITE)
, m_MessageFontSize(40)
, m_ButtonFontSize(40)
, m_AnchorPoint(Vec2::ZERO)
, m_Position(Vec2::ZERO)
, m_PopupOpenAnimation(ePOPUP_ANIMATION::NONE)
, m_PopupCloseAnimation(ePOPUP_ANIMATION::NONE)
, m_BackgroundColor(COLOR::BRIGHT_WHITEGRAY_ALPHA)
, m_BackgroundVisible(true)
, m_DefaultCallbackCleanUp(true)
, m_DefaultCallbackEnable(true)
{
	this->setContentSize(Director::getInstance()->getVisibleSize());
}

CPopup::~CPopup()
{
    // if there is still default callback, remove it
    this->removeDefaultCallbackFromStack();
}

CPopup* CPopup::create()
{
    CPopup *pRet = new(std::nothrow) CPopup();
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

bool CPopup::init()
{
	if (!Node::init())
		return false;
	return true;
}

CPopup* CPopup::show(Node* parent, int zOrder/* = 0*/)
{
    this->popupTouchDisable();
    
    if (m_BackgroundVisible)
        this->backgroundTouchDisable();
    
	if (!m_DefaultCallBack && m_DefaultCallbackEnable) {
		this->setDefaultCallback([=](Node* sender){
			this->popupClose();
		});
	}
    
	if (m_PositiveButtonCallBack || m_NegativeButtonCallBack){
		auto defaultBG = LayerColor::create(COLOR::WHITEGRAY_ALPHA, 1080.f, 570.f);
		defaultBG->setIgnoreAnchorPointForPosition(false);
		defaultBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		defaultBG->setPosition(this->getContentSize() / 2);
		this->addChild(defaultBG);

		auto createButton = [=](const NODE_CALLBACK &callback,
			Color4B btnColor, std::string name) -> CMyButton* {

			return CMyButton::create()
				->addEventListener(callback)
                ->setButtonSingleUse(true)
				->setLayer(LayerColor::create(btnColor, 430, 150))
				->setContents(name)
				->setFont(m_ButtonFontColor, m_ButtonFontSize)
				->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
				->setButtonPosition(Vec2(defaultBG->getContentSize().width * 0.5f,
                                         defaultBG->getContentSize().height * 0.25f))
				->show(defaultBG);
		};

		CMyButton* positiveButton = nullptr;
		CMyButton* negativeButton = nullptr;

		if (m_PositiveButtonCallBack != nullptr)
		{
			positiveButton = createButton([=](Node* sender){
				this->retain();
				m_PositiveButtonCallBack(this);
				this->popupClose();
				this->release();
			}, COLOR::BRIGHTRED_ALPHA, m_PositiveButtonName);
		}

		if (m_NegativeButtonCallBack != nullptr)
		{
			negativeButton = createButton([=](Node* sender){
				this->retain();
				m_NegativeButtonCallBack(this);
				this->popupClose();
				this->release();
			}, COLOR::DARKGRAY_ALPHA, m_NegativeButtonName);
		}

		if (positiveButton && negativeButton)
		{
			positiveButton->setPosition(Vec2(defaultBG->getContentSize().width * 0.275f, defaultBG->getContentSize().height * 0.25f));
			negativeButton->setPosition(Vec2(defaultBG->getContentSize().width * 0.725f, defaultBG->getContentSize().height * 0.25f));
		}

		if (m_Message != ""){
            auto message = Label::createWithSystemFont(m_Message, FONT::MALGUNBD, m_MessageFontSize,
                                                Size(defaultBG->getContentSize().width * 0.85f,
                                                     defaultBG->getContentSize().height),
                                                TextHAlignment::CENTER,
                                                TextVAlignment::CENTER);
			message->setColor(m_MessageFontColor);
			message->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			message->setPosition(Vec2(defaultBG->getContentSize().width * 0.5f,
                                      defaultBG->getContentSize().height * 0.6f));
			defaultBG->addChild(message);
		}
	}
    
	this->popupOpenAnimation();
    this->setAnchorPoint(m_AnchorPoint);
    this->setPosition(m_Position);
    this->setCascadeOpacityEnabled(true);
	if (parent != nullptr)
		parent->addChild(this, zOrder);

	return this;
}

CPopup* CPopup::setPositiveButton(const NODE_CALLBACK &callback, std::string btnName)
{
	m_PositiveButtonCallBack = callback;
	m_PositiveButtonName = btnName;

	return this;
}

CPopup* CPopup::setNegativeButton(const NODE_CALLBACK &callback, std::string btnName)
{
	m_NegativeButtonCallBack = callback;
	m_NegativeButtonName = btnName;

	return this;
}

CPopup* CPopup::setDefaultCallback(const NODE_CALLBACK &callback, bool cleanUp/* = true*/)
{
    m_DefaultCallBack = callback;
	m_DefaultCallbackCleanUp = cleanUp;
    
    if (m_DefaultCallBack){
        m_DefaultCallbackStack.push_back(sDEFAULT_CALLBACK([=](Node* sender){
            this->retain();
            m_DefaultCallBack(this);
            if (m_DefaultCallbackCleanUp) //cleanup 일때 stack에서는 따로 안지워도 되는지 확인바람 -> 호출 후 지우고 있음
                m_DefaultCallBack = nullptr;
            this->release();
        }, this));
    }
    
    return this;
}

CPopup* CPopup::setOpenAnimation(const NODE_CALLBACK &callback)
{
	m_OpenAnimationCallBack = callback;

	return this;
}

CPopup* CPopup::setCloseAnimation(const NODE_CALLBACK &callback)
{
	m_CloseAnimationCallBack = callback;

	return this;
}

CPopup* CPopup::setDefaultAnimation(ePOPUP_ANIMATION open, ePOPUP_ANIMATION close)
{
	m_PopupOpenAnimation = open;
	m_PopupCloseAnimation = close;

	return this;
}

CPopup* CPopup::setMessage(std::string message)
{
	m_Message = message;

	return this;
}

CPopup* CPopup::setMessageFont(Color3B fontColor, int size)
{
	m_MessageFontColor = fontColor;
	m_MessageFontSize = size;

	return this;
}

CPopup* CPopup::setButtonFont(Color3B fontColor, int size)
{
	m_ButtonFontColor = fontColor;
	m_ButtonFontSize = size;

	return this;
}

CPopup* CPopup::setPopupPosition(cocos2d::Vec2 position)
{
    m_Position = position;
    
    return this;
}

CPopup* CPopup::setPopupAnchorPoint(cocos2d::Vec2 anchorPoint)
{
    m_AnchorPoint = anchorPoint;
    
    return this;
}

CPopup* CPopup::setBackgroundColor(Color4B color)
{
	m_BackgroundColor = color;
	return this;
}

CPopup* CPopup::setBackgroundVisible(bool visible)
{
    m_BackgroundVisible = visible;
    return this;
}

CPopup* CPopup::setDefaultCallbackEnable(bool enable)
{
    m_DefaultCallbackEnable = enable;
    return this;
}

void CPopup::DefaultCallback()
{
    if(CPopup::m_DefaultCallbackStack.size() <= 0) return;
    
    auto callback = CPopup::m_DefaultCallbackStack.back();
	if (callback._sender->getDefaultCallbackCleanUp())
		CPopup::m_DefaultCallbackStack.pop_back();
    callback._callback(callback._sender);
}

void CPopup::popupOpenAnimation()
{
	Vec2 OriginPosition = this->getPosition();
	Size ScreenSize = Director::getInstance()->getVisibleSize();

	Vec2 posArray[] = {
		Vec2(ScreenSize.width + OriginPosition.x, OriginPosition.y),
		Vec2(-ScreenSize.width + OriginPosition.x, OriginPosition.y),
		Vec2(OriginPosition.x, ScreenSize.height + OriginPosition.y),
		Vec2(OriginPosition.x, -ScreenSize.height + OriginPosition.y),
	};

	auto expotential = [=](Vec2 pos)->EaseExponentialOut*{
		this->setPosition(pos);
		return EaseExponentialOut::create(MoveTo::create(0.5f, OriginPosition));
	};

	FiniteTimeAction* action = nullptr;

	switch (m_PopupOpenAnimation){

	case ePOPUP_ANIMATION::NONE:
		action = CallFunc::create([=](){
			if (m_OpenAnimationCallBack != nullptr){
				this->retain();
				m_OpenAnimationCallBack(this);
				this->release();
			}
		}); break;
	case ePOPUP_ANIMATION::OPEN_LEFT: action = expotential(posArray[0]); break;
	case ePOPUP_ANIMATION::OPEN_RIGHT:action = expotential(posArray[1]); break;
	case ePOPUP_ANIMATION::OPEN_DOWN: action = expotential(posArray[2]); break;
	case ePOPUP_ANIMATION::OPEN_UP:   action = expotential(posArray[3]); break;
	case ePOPUP_ANIMATION::OPEN_CENTER:
	{
		this->setScale(0.f);
		action = EaseElasticOut::create(ScaleTo::create(0.5f, 1.0f), 0.5f);
	} break;
	}

	if (m_BackgroundVisible)
	{
		auto originOpacity = m_EmptyBackground->getOpacity();
		m_EmptyBackground->setOpacity(0);
		auto fadeInAction = FadeTo::create(0.3f, originOpacity);
		m_EmptyBackground->runAction(fadeInAction);
	}
    
    auto touchEnable = CallFunc::create([=](){
        this->popupTouchEnable(true);
    });
    
    this->runAction(Sequence::createWithTwoActions(action, touchEnable));
}

void CPopup::popupClose(float delayTime/* = 0.5f*/)
{
	Vec2 OriginPosition = this->getPosition();
	Size ScreenSize = Director::getInstance()->getVisibleSize();
    
	Vec2 posArray[] = {
		Vec2(-ScreenSize.width + OriginPosition.x, OriginPosition.y),
		Vec2(ScreenSize.width + OriginPosition.x, OriginPosition.y),
		Vec2(OriginPosition.x, OriginPosition.y - ScreenSize.height),
		Vec2(OriginPosition.x, OriginPosition.y + ScreenSize.height)
	};

	auto ease = [=](Vec2 pos) -> EaseSineIn*{
		return EaseSineIn::create(MoveTo::create(0.35f, pos));
	};

	FiniteTimeAction* action = nullptr;

    if (m_CloseAnimationCallBack == nullptr)
        delayTime = 0.f;
    
	switch (m_PopupCloseAnimation){

	case ePOPUP_ANIMATION::NONE:
		action = CallFunc::create([=](){
			if (m_CloseAnimationCallBack != nullptr){
				this->retain();
				m_CloseAnimationCallBack(this);
				this->release();
			}
		}); break;
	case ePOPUP_ANIMATION::CLOSE_LEFT: action = ease(posArray[0]); break;
	case ePOPUP_ANIMATION::CLOSE_RIGHT:action = ease(posArray[1]); break;
	case ePOPUP_ANIMATION::CLOSE_DOWN: action = ease(posArray[2]); break;
	case ePOPUP_ANIMATION::CLOSE_UP:   action = ease(posArray[3]); break;
	case ePOPUP_ANIMATION::CLOSE_CENTER:
	{
		action = EaseElasticIn::create(ScaleTo::create(0.4f, 0.2f), 1);
	} break;
	}

	if (m_BackgroundVisible)
	{
		auto fadeOutAction = FadeTo::create(0.5f, 1);
		m_EmptyBackground->runAction(fadeOutAction);
	}
    
    auto touchDisable = CallFunc::create([=](){
        // if there is still default callback, remove it (when called popupClose.)
        this->removeDefaultCallbackFromStack();
        this->popupTouchEnable(false);
    });
    
    auto remove = CallFunc::create([=](){
        this->removeFromParent();
    });
    
	this->runAction(Sequence::create(touchDisable,
                                     action,
                                     DelayTime::create(delayTime),
                                     remove, NULL));
	
}

void CPopup::popupTouchEnable(bool enable)
{
    if(!m_TouchDisable) return;
    
    m_TouchDisable->setVisible(!enable);
}

void CPopup::changeDefaultCallback(const NODE_CALLBACK &callback)
{
    if(m_DefaultCallBack) m_DefaultCallBack = callback;
}

void CPopup::backgroundTouchDisable()
{
	//auto touchDisable = Widget::create();
	//touchDisable->setTouchEnabled(true);
	//touchDisable->setContentSize(_director->getVisibleSize());
	//touchDisable->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	//touchDisable->setPosition(this->getContentSize() / 2);
	//addChild(touchDisable);

	if (m_EmptyBackground) return;

    m_EmptyBackground = CMyButton::create()
    ->addEventListener([](Node* sender){})
    ->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
    ->setLayer(LayerColor::create(m_BackgroundColor, 1080, 1920))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(this->getContentSize() / 2)
    ->show(this, -1);
}

void CPopup::popupTouchDisable()
{
    if (m_TouchDisable) return;
    
    m_TouchDisable = CMyButton::create()
    ->addEventListener([](Node* sender){})
    ->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
    ->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080, 1920))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(this->getContentSize() / 2)
    ->show(this, ZORDER::POPUP);
}

void CPopup::setDefaultCallbackToTopAgain()
{
    if(!m_DefaultCallBack) return;
    
    auto callback = m_DefaultCallBack;
    this->removeDefaultCallbackFromStack();
    this->setDefaultCallback(callback, m_DefaultCallbackCleanUp);
}

void CPopup::removeDefaultCallbackFromStack()
{
    if (m_DefaultCallBack){
        m_DefaultCallbackStack.remove_if([=](sDEFAULT_CALLBACK data){
            return data._sender == this;
        });
        m_DefaultCallBack = nullptr;
    }
}
