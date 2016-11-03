#include "Popup.h"
#include "MyButton.h"

CPopup::CPopup()
	: m_PositiveButtonFunc(nullptr)
	, m_NegativeButtonFunc(nullptr)
	, m_WidgetInitFunc(nullptr)
	, m_OpenEffectFunc(nullptr)
	, m_CloseEffectFunc(nullptr)
	, m_Message("")
	, m_PositiveButtonName("")
	, m_NegativeButtonName("")
	, m_MessageFontColor(Color3B::BLACK)
	, m_ButtonFontColor(Color3B::WHITE)
	, m_MessageFontSize(40)
	, m_ButtonFontSize(40)
{}

CPopup::CPopup(std::string popupNotice,
	CMyButton* button,
	size_t fontSize,
	Color3B fontColor,
	ePOPUP_EFFECT openEffect,
	const std::function<void(CPopup*)> &openEffectFunc)
	: m_Btn1(button)
	, m_Btn2(nullptr)
	, m_PopupNotice(popupNotice)
	, m_PopupAble(nullptr)
	, m_FontSize(fontSize)
	, m_PopupInfo(ONEBTN)
	, m_FontColor(fontColor)
	, m_PopupOpenEffectFunc(openEffectFunc)
	, m_PopupCloseEffectFunc(nullptr)
	, m_PopupOpenEffect(openEffect)
	, m_PopupCloseEffect(POPUPEFFECT_none)
{}


CPopup::CPopup(std::string popupNotice,
	CMyButton* button1,
	CMyButton* button2,
	size_t fontSize,
	Color3B fontColor,
	ePOPUP_EFFECT openEffect,
	const std::function<void(CPopup*)> &openEffectFunc)
	: m_Btn1(button1)
	, m_Btn2(button2)
	, m_PopupNotice(popupNotice)
	, m_PopupAble(nullptr)
	, m_FontSize(fontSize)
	, m_PopupInfo(TWOBTN)
	, m_FontColor(fontColor)
	, m_PopupOpenEffectFunc(openEffectFunc)
	, m_PopupCloseEffectFunc(nullptr)
	, m_PopupOpenEffect(openEffect)
	, m_PopupCloseEffect(POPUPEFFECT_none)
{}


CPopup::CPopup(CSpecificPopupBase* format,
	ePOPUP_EFFECT openEffect,
	const std::function<void(CPopup*)> &openEffectFunc)
	: m_Btn1(nullptr)
	, m_Btn2(nullptr)
	, m_PopupNotice("")
	, m_PopupAble(std::shared_ptr<CSpecificPopupBase>(format))
	, m_FontSize(20)
	, m_PopupInfo(FORMAT)
	, m_FontColor(Color3B::BLACK)
	, m_PopupOpenEffectFunc(openEffectFunc)
	, m_PopupCloseEffectFunc(nullptr)
	, m_PopupOpenEffect(openEffect)
	, m_PopupCloseEffect(POPUPEFFECT_none)
{}


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

CPopup* CPopup::createWithOneButton(std::string popupNotice,
	CMyButton* button,
	size_t fontSize/* = 20*/,
	Color3B fontColor/* = Color3B::BLACK*/,
	ePOPUP_EFFECT openEffect/* = POPUPEFFECT_none*/,
	const std::function<void(CPopup*)> &openEffectFunc/* = nullptr*/)
{
	CPopup *pRet = new(std::nothrow) CPopup(popupNotice,
		button,
		fontSize,
		fontColor,
		openEffect,
		openEffectFunc);
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

CPopup* CPopup::createWithTwoButton(std::string popupNotice,
	CMyButton* button1,
	CMyButton* button2,
	size_t fontSize/* = 20*/,
	Color3B fontColor/* = Color3B::BLACK*/,
	ePOPUP_EFFECT openEffect/* = POPUPEFFECT_none*/,
	const std::function<void(CPopup*)> &openEffectFunc/* = nullptr*/)
{
	CPopup *pRet = new(std::nothrow) CPopup(popupNotice,
		button1,
		button2,
		fontSize,
		fontColor,
		openEffect,
		openEffectFunc);
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

CPopup* CPopup::createWithSpecificFormat(CSpecificPopupBase* format,
	ePOPUP_EFFECT openEffect/* = POPUPEFFECT_none*/,
	const std::function<void(CPopup*)> &openEffectFunc/* = nullptr*/)
{
	CPopup *pRet = new(std::nothrow) CPopup(format,
		openEffect,
		openEffectFunc);
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
	if (!initVariable())
		return false;
	return true;
}

bool CPopup::initVariable()
{
	try{
		auto emptyBtnBG = CMyButton::createWithLayerColor(Size(1080, 1920),
			Color4B(255, 255, 255, 255 * 0.4f),
			"",
			40,
			Color3B::WHITE,
			END,
			[](Node* sender){});

        if(emptyBtnBG != nullptr){
            emptyBtnBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            emptyBtnBG->setPosition(Vec2::ZERO);
            this->addChild(emptyBtnBG);
        }
        
		switch (m_PopupInfo) {
		case ONEBTN:
		{
            auto defaultBG = LayerColor::create(Color4B(255, 255, 255, 255 * 0.8f), 1080.f, 570.f);
            if(defaultBG != nullptr){
                defaultBG->setIgnoreAnchorPointForPosition(false);
                defaultBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                defaultBG->setPosition(Vec2::ZERO);
                this->addChild(defaultBG);
            }
            
			m_NoticeLabel = Label::createWithTTF(m_PopupNotice, "fonts/malgunbd.ttf", m_FontSize);
			if (m_NoticeLabel != nullptr){
				m_NoticeLabel->setColor(m_FontColor);
				m_NoticeLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				m_NoticeLabel->setPosition(Vec2(defaultBG->getContentSize().width * 0.5f, defaultBG->getContentSize().height * 0.6f));
				defaultBG->addChild(m_NoticeLabel);
			}

			if (m_Btn1 != nullptr)
			{
				m_Btn1->setPosition(Vec2(defaultBG->getContentSize().width * 0.5f, defaultBG->getContentSize().height * 0.25f));
				m_Btn1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				defaultBG->addChild(m_Btn1);
				m_Btn1->AddState(END, std::bind(&CPopup::PopupClose, this));
				m_Btn1->AddState(END, std::bind(&CPopup::PopupRelease, this));
			}
		} break;
		case TWOBTN:
		{
            auto defaultBG = LayerColor::create(Color4B(255, 255, 255, 255 * 0.8f), 1080.f, 570.f);
            if(defaultBG != nullptr){
                defaultBG->setIgnoreAnchorPointForPosition(false);
                defaultBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                defaultBG->setPosition(Vec2::ZERO);
                this->addChild(defaultBG);
            }
            
			m_NoticeLabel = Label::createWithTTF(m_PopupNotice, "fonts/malgunbd.ttf", m_FontSize);
			if (m_NoticeLabel != nullptr){
				m_NoticeLabel->setColor(m_FontColor);
				m_NoticeLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				m_NoticeLabel->setPosition(Vec2(defaultBG->getContentSize().width * 0.5f, defaultBG->getContentSize().height * 0.6f));
				defaultBG->addChild(m_NoticeLabel);
			}

			if (m_Btn1 != nullptr)
			{
				m_Btn1->setPosition(Vec2(defaultBG->getContentSize().width * 0.275f, defaultBG->getContentSize().height * 0.25f));
				m_Btn1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				defaultBG->addChild(m_Btn1);
				m_Btn1->AddState(END, std::bind(&CPopup::PopupClose, this));
				m_Btn1->AddState(END, std::bind(&CPopup::PopupRelease, this));
			}
			if (m_Btn2 != nullptr)
			{
				m_Btn2->setPosition(Vec2(defaultBG->getContentSize().width * 0.725f, defaultBG->getContentSize().height * 0.25f));
                m_Btn2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				defaultBG->addChild(m_Btn2);
				m_Btn2->AddState(END, std::bind(&CPopup::PopupClose, this));
				m_Btn2->AddState(END, std::bind(&CPopup::PopupRelease, this));
			}
		} break;
		case FORMAT:
		{
			if (m_PopupAble != nullptr)
			{
				m_PopupAble->init(this);
			}
		} break;
		}
		this->scheduleOnce([this](float delta){popupOpenEffect(); }, 0.f, "popupOpenEffect");

	}
	catch (std::string errorLog){
		CCLOG("%s", errorLog.c_str());
		assert(false);
		return false;
	}
	return true;
}

CPopup* CPopup::show(Node* parent, int zOrder/* = 0*/)
{
	this->setContentSize(Director::getInstance()->getVisibleSize());

	if (m_WidgetInitFunc != nullptr){
		this->retain();
		m_WidgetInitFunc(this);
		this->release();
	}
	else{
		auto defaultBG = LayerColor::create(Color4B(255, 255, 255, 255 * 0.8f), 1080.f, 570.f);
		defaultBG->setIgnoreAnchorPointForPosition(false);
		defaultBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		defaultBG->setPosition(this->getContentSize() / 2);
		this->addChild(defaultBG);
		
		auto message = Label::createWithTTF(m_Message, "fonts/malgunbd.ttf", m_MessageFontSize);
		message->setColor(m_MessageFontColor);
		message->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		message->setPosition(Vec2(defaultBG->getContentSize().width * 0.5f, defaultBG->getContentSize().height * 0.6f));
		defaultBG->addChild(message);
			
		auto createLayerButton = [=](const FUNCTION_CALLBACK &func, Color4B btnColor, std::string name){
			return CMyButton::createWithLayerColor(Size(430, 150), btnColor, name, m_ButtonFontSize, m_ButtonFontColor, END, func);
		};

		CMyButton* positiveButton = nullptr;
		CMyButton* negativeButton = nullptr;

		if (m_PositiveButtonFunc != nullptr)
		{
			positiveButton = createLayerButton([=](Node* sender){
				this->retain();
				m_PositiveButtonFunc(this);
				this->removeFromParent();
				this->release();
			}, Color4B(255, 48, 48, 255 * 0.8f), m_PositiveButtonName);
			positiveButton->setPosition(Vec2(defaultBG->getContentSize().width * 0.5f, defaultBG->getContentSize().height * 0.25f));
			positiveButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			defaultBG->addChild(positiveButton);
		}

		if (m_NegativeButtonFunc != nullptr)
		{
			negativeButton = createLayerButton([=](Node* sender){
				this->retain();
				m_NegativeButtonFunc(this);
				this->removeFromParent();
				this->release();
			}, Color4B(0, 0, 0, 255 * 0.8f), m_NegativeButtonName);
			negativeButton->setPosition(Vec2(defaultBG->getContentSize().width * 0.5f, defaultBG->getContentSize().height * 0.25f));
			negativeButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			defaultBG->addChild(negativeButton);
		}

		if (positiveButton && negativeButton)
		{
			positiveButton->setPosition(Vec2(defaultBG->getContentSize().width * 0.275f, defaultBG->getContentSize().height * 0.25f));
			negativeButton->setPosition(Vec2(defaultBG->getContentSize().width * 0.725f, defaultBG->getContentSize().height * 0.25f));
		}
	}

	if (m_OpenEffectFunc != nullptr){
		this->retain();
		m_OpenEffectFunc(this);
		this->release();
	}

	parent->addChild(this, zOrder);
	
	return this;
}

CPopup* CPopup::setPositiveButton(const FUNCTION_CALLBACK &func, std::string btnName)
{
	m_PositiveButtonFunc = func;
	m_PositiveButtonName = btnName;

	return this;
}

CPopup* CPopup::setNegativeButton(const FUNCTION_CALLBACK &func, std::string btnName)
{
	m_NegativeButtonFunc = func;
	m_NegativeButtonName = btnName;

	return this;
}

CPopup* CPopup::setWidget(const FUNCTION_CALLBACK &func)
{
	m_WidgetInitFunc = func;

	return this;
}

CPopup* CPopup::setOpenEffect(const FUNCTION_CALLBACK &func)
{
	m_OpenEffectFunc = func;

	return this;
}

CPopup* CPopup::setCloseEffect(const FUNCTION_CALLBACK &func)
{
	m_CloseEffectFunc = func;

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

void CPopup::playEffect(const FUNCTION_CALLBACK &func)
{
	if (func != nullptr){
		this->retain();
		func(this);
		this->release();
	}
}





void CPopup::setPosition(const Vec2& position)
{
	Node::setPosition(position);
	m_OriginPos = position;
}

void CPopup::popupOpenEffect()
{
	if (m_PopupOpenEffect == POPUPEFFECT_none)
		return;

	switch (m_PopupOpenEffect) {
	case POPUPEFFECT_default_move:
	{
		popupOpenEffect_move(m_PopupOpenEffectFunc);
	} break;
	case POPUPEFFECT_default_fade:
	{
		popupOpenEffect_fadeIn(m_PopupOpenEffectFunc);
	} break;
	}
	if (m_PopupOpenEffectFunc != nullptr)
		this->m_PopupOpenEffectFunc(this);
}

void CPopup::popupCloseEffect()
{
	if (m_PopupCloseEffect == POPUPEFFECT_none)
		return;

	switch (m_PopupCloseEffect) {
	case POPUPEFFECT_default_move:
	{
		popupCloseEffect_move(m_PopupCloseEffectFunc);
	} break;
	case POPUPEFFECT_default_fade:
	{
		popupCloseEffect_fadeOut(m_PopupCloseEffectFunc);
	} break;
	}
	if (m_PopupCloseEffectFunc != nullptr)
		this->m_PopupCloseEffectFunc(this);
}


#pragma mark - DefaultPopupEffect

void CPopup::popupOpenEffect_move(std::function<void(CPopup*)>& popup)
{
	popup = [](CPopup* p){
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();
		p->runAction(MoveTo::create(0.5f, Vec2(origin.x + visibleSize.width * 0.5f, origin.x + visibleSize.height * 0.5f)));
	};
}

void CPopup::popupCloseEffect_move(std::function<void(CPopup*)>& popup)
{
	popup = [this](CPopup* p){
		p->runAction(Sequence::create(MoveTo::create(0.5f, Vec2(m_OriginPos.x, m_OriginPos.y)),
			CallFunc::create([p](){
			p->PopupRelease();
		}), nullptr));
	};
}


void CPopup::popupOpenEffect_fadeIn(std::function<void(CPopup*)>& popup)
{

}



void CPopup::popupCloseEffect_fadeOut(std::function<void(CPopup*)>& popup)
{

}
#pragma mark - SpecificPopupBase

bool CSpecificPopupBase::init(CPopup *popup)
{
	if (popup != nullptr)
		m_Popup = popup;

	if (!initVariable())
		return false;
	return true;
};