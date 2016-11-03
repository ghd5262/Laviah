#include "ExitPopup.h"
#include "../MyButton.h"

CExitPopup* CExitPopup::create()
{
	CExitPopup *pRet = new(std::nothrow) CExitPopup();
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

bool CExitPopup::initVariable()
{
	try{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		auto bg = LayerColor::create(Color4B(255, 255, 255, 255 * 0.8f), 1080.f, 570.f);
		if (bg != nullptr){
			bg->setIgnoreAnchorPointForPosition(false);
			bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			bg->setPosition(Vec2::ZERO);
			m_Popup->addChild(bg);
		}

		auto mentLabel = Label::createWithTTF("Are you sure you want to exit StarStarStar?", "fonts/malgunbd.ttf", 40);
		if (mentLabel != nullptr)
		{
			mentLabel->setColor(g_labelColor1);
			mentLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			mentLabel->setPosition(Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.6f));
			bg->addChild(mentLabel);
		}

		auto btnExit = CMyButton::createWithString("defaultBtn_1.png",
			"Watch",
			40,
			Color3B::WHITE,
			END,
			[=](Node* sender){this->Exit(sender); },
			EFFECT_ALPHA);
		if (btnExit != nullptr)
		{
			btnExit->setPosition(Vec2(bg->getContentSize().width * 0.275f, bg->getContentSize().height * 0.25f));
			btnExit->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			bg->addChild(btnExit);
		}

		auto btnNo = CMyButton::createWithString("defaultBtn_2.png",
			"No",
			40,
			Color3B::WHITE,
			END,
			[this](Node* sender){
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
		});

		m_Popup->setPopupCloseEffectFunc([this](CPopup* pausePopup){
			m_Popup->scheduleOnce([this](float delta){
				CSpecificPopupBase::PopupRelease();
			}, 0.1f, "ExitPopupClose");
		});
	}
	catch (...){
		throw StringUtils::format("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	return true;
}

void CExitPopup::Exit(Node* sender){
	CCLOG("format popup Video");
}