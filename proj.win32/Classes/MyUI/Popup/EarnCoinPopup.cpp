#include "EarnCoinPopup.h"
#include "../MyButton.h"

CEarnCoinPopup* CEarnCoinPopup::create()
{
	CEarnCoinPopup *pRet = new(std::nothrow) CEarnCoinPopup();
	if (pRet)
	{
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool CEarnCoinPopup::initVariable()
{
	try{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		auto bg = LayerColor::create(Color4B(255, 255, 255, 255 * 0.8f), 1080.f, 570.f);
		if (bg != nullptr){
			bg->ignoreAnchorPointForPosition(false);
			bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			bg->setPosition(Vec2::ZERO);
			m_Popup->addChild(bg);
		}

		auto mentLabel = Label::createWithTTF(MakeString("Would you like to watch a video to earn %d more?", 500).c_str(), "fonts/malgunbd.ttf", 40);
		if (mentLabel != nullptr)
		{
			mentLabel->setColor(g_labelColor1);
			mentLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			mentLabel->setPosition(Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.6f));
			bg->addChild(mentLabel);
		}

		auto btnWatch = CMyButton::createWithString("defaultBtn_1.png",
			"Watch",
			40,
			Color3B::WHITE,
			END,
			std::bind(&CEarnCoinPopup::Video, this), 
			EFFECT_ALPHA);
		if (btnWatch != nullptr)
		{
			btnWatch->setPosition(Vec2(bg->getContentSize().width * 0.275f, bg->getContentSize().height * 0.25f));
			btnWatch->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			bg->addChild(btnWatch);
		}

		auto btnNo = CMyButton::createWithString("defaultBtn_2.png",
			"No",
			40,
			Color3B::WHITE,
			END,
			[this](){
			CSpecificPopupBase::PopupClose();
		}, EFFECT_ALPHA);
		if (btnNo != nullptr)
		{
			btnNo->setPosition(Vec2(bg->getContentSize().width * 0.725f, bg->getContentSize().height * 0.25f));
			btnNo->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			bg->addChild(btnNo);
		}

		m_Popup->setPopupOpenEffectFunc([this](CPopup* pausePopup){
			auto winSize = Director::getInstance()->getWinSize();
			m_Popup->scheduleOnce([this](float delta){
			}, 0.1f, "EarnCoinPopupOpen");
		});

		m_Popup->setPopupCloseEffectFunc([this](CPopup* pausePopup){
			m_Popup->scheduleOnce([this](float delta){
				CSpecificPopupBase::PopupRelease();
			}, 0.1f, "EarnCoinPopupClose");
		});
	}
	catch (...){
		throw StringUtils::format("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	return true;
}

void CEarnCoinPopup::Video(){
	CCLOG("format popup Video");
}