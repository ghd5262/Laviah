#include "HelpPopup.h"
#include "../MyButton.h"
#include "../../Scene/GameScene.h"
#include "ui/UIScrollView.h"
#include "ui/UIImageView.h"
#include "ui/UIPageView.h"

CHelpPopup* CHelpPopup::create()
{
	CHelpPopup *pRet = new(std::nothrow) CHelpPopup();
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

bool CHelpPopup::initVariable()
{
	try{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		size_t helpImgCount = 2;

        m_BG = LayerColor::create(Color4B(255, 255, 255, 0), 1080.f, 1920.f);
        if(m_BG != nullptr){
            m_BG->setIgnoreAnchorPointForPosition(false);
            m_BG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_BG->setPosition(Vec2(0, visibleSize.height));
            m_Popup->addChild(m_BG);
        }

		// PageView 생성
		auto pageview = PageView::create();
		pageview->setTouchEnabled(true);
		pageview->setContentSize(visibleSize);
		pageview->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f));
		pageview->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		for (int i = 0; i < helpImgCount; i++) {
			// Layout 생성
			auto layout = Layout::create();
			layout->setSize(visibleSize);

			// ImageView를 생성하고 Layout에 add함
			auto imageView = ImageView::create(StringUtils::format("helpImg_%d.png", i + 1));
			imageView->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
			layout->addChild(imageView);

			// Layout을 PageView에 add함
			pageview->addPage(layout);
		}

		m_BG->addChild(pageview);


		m_btnEnd = CMyButton::create("endIcon.png",
			END,
			std::bind(&CHelpPopup::End, this),
			EFFECT_ALPHA);

		if (m_btnEnd != nullptr)
		{
			m_btnEnd->setPosition(Vec2(m_BG->getContentSize().width * 0.92f,
				m_BG->getContentSize().height * 0.05f));
			m_btnEnd->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_BG->addChild(m_btnEnd, 10);
			m_btnEnd->setCascadeOpacityEnabled(true);
			m_btnEnd->setOpacity(0);
		}


		m_Popup->setPopupOpenEffectFunc([this](CPopup* pausePopup){
			auto winSize = Director::getInstance()->getWinSize();
			m_BG->runAction(EaseExponentialOut::create(MoveTo::create(0.8f, Vec2(0, 0))));
			m_btnEnd->runAction(FadeIn::create(0.5f));
		});

		m_Popup->setPopupCloseEffectFunc([this, visibleSize, origin](CPopup* pausePopup){


			m_btnEnd->runAction(FadeTo::create(0.5f, 0));
			m_Popup->scheduleOnce([this, visibleSize, origin](float delta){
				m_BG->runAction(Sequence::create(EaseSineIn::create(MoveTo::create(0.4f, Vec2(0, origin.x + visibleSize.height))),
					CallFunc::create([this](){
					CSpecificPopupBase::PopupRelease();
				}), nullptr));
			}, 0.1f, "HelpPopupClose");
		});
	}
	catch (...){
		throw StringUtils::format("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	return true;
}

void CHelpPopup::End(){
	CCLOG("format popup End");
	CSpecificPopupBase::PopupClose();
}