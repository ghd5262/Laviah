#include "Popup.h"
#include "MyButton.h"

using namespace cocos2d;
using namespace cocos2d::ui;

CPopup::CPopup()
: m_PositiveButtonCallBack(nullptr)
, m_NegativeButtonCallBack(nullptr)
, m_OpenAnimationCallBack(nullptr)
, m_CloseAnimationCallBack(nullptr)
, m_Message("")
, m_PositiveButtonName("")
, m_NegativeButtonName("")
, m_MessageFontColor(Color3B::BLACK)
, m_ButtonFontColor(Color3B::WHITE)
, m_MessageFontSize(40)
, m_ButtonFontSize(40)
, m_AnchorPoint(Vec2::ZERO)
, m_Position(Vec2::ZERO)
, m_PopupOpenAnimation(ePOPUP_ANIMATION::NONE)
, m_PopupCloseAnimation(ePOPUP_ANIMATION::NONE)
{
	this->setContentSize(Director::getInstance()->getVisibleSize());
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

	backgroundTouchDisable();

	return true;
}

CPopup* CPopup::show(Node* parent, int zOrder/* = 0*/)
{
	if (m_PositiveButtonCallBack || m_NegativeButtonCallBack){
		auto defaultBG = LayerColor::create(Color4B(255, 255, 255, 255 * 0.8f), 1080.f, 570.f);
		defaultBG->setIgnoreAnchorPointForPosition(false);
		defaultBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		defaultBG->setPosition(this->getContentSize() / 2);
		this->addChild(defaultBG);

		auto createButton = [=](const NODE_CALLBACK &callback,
			Color4B btnColor, std::string name) -> CMyButton* {

			return CMyButton::create()
				->addEventListener(callback)
				->setLayer(LayerColor::create(btnColor, 430, 150))
				->setContents(name)
				->setFont(m_ButtonFontColor, m_ButtonFontSize)
				->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
				->setButtonPosition(Vec2(defaultBG->getContentSize().width * 0.5f, defaultBG->getContentSize().height * 0.25f))
				->show(defaultBG);
		};

		CMyButton* positiveButton = nullptr;
		CMyButton* negativeButton = nullptr;

		if (m_PositiveButtonCallBack != nullptr)
		{
			positiveButton = createButton([=](Node* sender){
				this->retain();
				m_PositiveButtonCallBack(this);
				this->removeFromParent();
				this->release();
			}, Color4B(255, 48, 48, 255 * 0.8f), m_PositiveButtonName);
		}

		if (m_NegativeButtonCallBack != nullptr)
		{
			negativeButton = createButton([=](Node* sender){
				this->retain();
				m_NegativeButtonCallBack(this);
				this->removeFromParent();
				this->release();
			}, Color4B(0, 0, 0, 255 * 0.8f), m_NegativeButtonName);
		}

		if (positiveButton && negativeButton)
		{
			positiveButton->setPosition(Vec2(defaultBG->getContentSize().width * 0.275f, defaultBG->getContentSize().height * 0.25f));
			negativeButton->setPosition(Vec2(defaultBG->getContentSize().width * 0.725f, defaultBG->getContentSize().height * 0.25f));
		}

		if (m_Message != ""){
			auto message = Label::createWithTTF(m_Message, "fonts/malgunbd.ttf", m_MessageFontSize);
			message->setColor(m_MessageFontColor);
			message->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			message->setPosition(Vec2(defaultBG->getContentSize().width * 0.5f, defaultBG->getContentSize().height * 0.6f));
			defaultBG->addChild(message);
		}
	}
	popupOpenAnimation();

    this->setAnchorPoint(m_AnchorPoint);
    this->setPosition(m_Position);
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
		action = EaseElasticOut::create(ScaleTo::create(0.5f, 1.0f));
	} break;
	}

	this->runAction(action);
}

void CPopup::popupClose()
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

	/* custom open animation일 경우 
	 * 애니메이션이 완료될 때까지 기다림
	 * 대략 0.5초
	 */
	float delayTime = 0.0f;
	if (m_CloseAnimationCallBack != nullptr)
		delayTime = 0.5f;

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

	}

	this->runAction(Sequence::create(action, DelayTime::create(delayTime), CallFunc::create([=](){
		this->removeFromParent();
	}), NULL));
	
}

void CPopup::backgroundTouchDisable()
{
	auto touchDisable = Widget::create();
	touchDisable->setTouchEnabled(true);
	touchDisable->addTouchEventListener([](Ref* ref, Widget::TouchEventType type){
		CCLOG("touch!!");
	});
	touchDisable->setContentSize(Director::getInstance()->getVisibleSize());
	touchDisable->setColor(Color3B::WHITE);
	touchDisable->setOpacity(255 * 0.4f);
	touchDisable->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	touchDisable->setPosition(this->getContentSize() / 2);
	addChild(touchDisable);
	//auto emptyBtnBG = CMyButton::create()
	//	->addEventListener([](Node* sender){})
	//	->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
	//	->setLayer(LayerColor::create(Color4B(255, 255, 255, 255 * 0.4f), 1080, 1920))
	//	->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
	//	->setButtonPosition(this->getContentSize() / 2)
	//	->show(this);
}

//void CPopup::playEffect(const CallBackTION_CALLBACK &CallBack)
//{
//	if (CallBack != nullptr){
//		this->retain();
//		CallBack(this);
//		this->release();
//	}
//}
//
//void CPopup::popupOpenEffect()
//{
//	if (m_PopupOpenEffect == POPUPEFFECT_none)
//		return;
//
//	switch (m_PopupOpenEffect) {
//	case POPUPEFFECT_default_move:
//	{
//		popupOpenEffect_move(m_PopupOpenEffectCallBack);
//	} break;
//	case POPUPEFFECT_default_fade:
//	{
//		popupOpenEffect_fadeIn(m_PopupOpenEffectCallBack);
//	} break;
//	}
//	if (m_PopupOpenEffectCallBack != nullptr)
//		this->m_PopupOpenEffectCallBack(this);
//}
//
//void CPopup::popupCloseEffect()
//{
//	if (m_PopupCloseEffect == POPUPEFFECT_none)
//		return;
//
//	switch (m_PopupCloseEffect) {
//	case POPUPEFFECT_default_move:
//	{
//		popupCloseEffect_move(m_PopupCloseEffectCallBack);
//	} break;
//	case POPUPEFFECT_default_fade:
//	{
//		popupCloseEffect_fadeOut(m_PopupCloseEffectCallBack);
//	} break;
//	}
//	if (m_PopupCloseEffectCallBack != nullptr)
//		this->m_PopupCloseEffectCallBack(this);
//}
//
//
//#pragma mark - DefaultPopupEffect
//
//void CPopup::popupOpenEffect_move(std::CallBacktion<void(CPopup*)>& popup)
//{
//	popup = [](CPopup* p){
//		Size visibleSize = Director::getInstance()->getVisibleSize();
//		Vec2 origin = Director::getInstance()->getVisibleOrigin();
//		p->runAction(MoveTo::create(0.5f, Vec2(origin.x + visibleSize.width * 0.5f, origin.x + visibleSize.height * 0.5f)));
//	};
//}
//
//void CPopup::popupCloseEffect_move(std::CallBacktion<void(CPopup*)>& popup)
//{
//	popup = [this](CPopup* p){
//		p->runAction(Sequence::create(MoveTo::create(0.5f, Vec2(m_OriginPos.x, m_OriginPos.y)),
//			CallCallBack::create([p](){
//			p->PopupRelease();
//		}), nullptr));
//	};
//}
//
//
//void CPopup::popupOpenEffect_fadeIn(std::CallBacktion<void(CPopup*)>& popup)
//{
//
//}
//
//
//
//void CPopup::popupCloseEffect_fadeOut(std::CallBacktion<void(CPopup*)>& popup)
//{
//
//}
