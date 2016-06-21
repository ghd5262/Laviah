#include "PausePopup.h"
#include "../MyButton.h"
#include "../../Scene/GameScene.h"


CPausePopup* CPausePopup::create()
{
	CPausePopup *pRet = new(std::nothrow) CPausePopup();
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

bool CPausePopup::initVariable()
{
	try{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

        m_BG = LayerColor::create(Color4B(255, 255, 255, 0), 1080.f, 1920.f);
        if(m_BG != nullptr){
            m_BG->ignoreAnchorPointForPosition(false);
            m_BG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_BG->setPosition(Vec2::ZERO);
            m_Popup->addChild(m_BG);
        }
        
        m_PauseBG = LayerColor::create(Color4B(255, 255, 255, 255 * 0.8f), 1080.f, 570.f);
        if(m_PauseBG != nullptr){
            m_PauseBG->ignoreAnchorPointForPosition(false);
            m_PauseBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_PauseBG->setPosition(Vec2(0, origin.x + visibleSize.height * 0.75f));
            m_Popup->addChild(m_PauseBG);
        }
        
        auto noticeLabel = Label::createWithTTF("", "fonts/malgunbd.ttf", 25);
		noticeLabel->setColor(Color3B::BLACK);
		noticeLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		noticeLabel->setPosition(Vec2(m_PauseBG->getContentSize().width * 0.5f, m_PauseBG->getContentSize().height * 0.5f));
		m_PauseBG->addChild(noticeLabel);

		m_btnHome = CMyButton::create("homeButton.png",
			END,
			std::bind(&CPausePopup::GoHome, this),
			EFFECT_ALPHA);

		if (m_btnHome != nullptr)
		{
			m_btnHome->setPosition(Vec2(m_PauseBG->getContentSize().width * -1.1f,
				m_PauseBG->getContentSize().height * 0.85f));
			m_btnHome->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_PauseBG->addChild(m_btnHome);
		}

		m_btnReset = CMyButton::create("restartButton.png",
			END,
			std::bind(&CPausePopup::Reset, this),
			EFFECT_ALPHA);

		if (m_btnReset != nullptr)
		{
			m_btnReset->setPosition(Vec2(m_PauseBG->getContentSize().width * -1.1f,
				m_PauseBG->getContentSize().height * 0.7f));
			m_btnReset->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_PauseBG->addChild(m_btnReset);
		}

		m_btnPlay = CMyButton::create("playButton.png",
			END,
			std::bind(&CPausePopup::Play, this),
			EFFECT_ALPHA);

		if (m_btnPlay != nullptr)
		{
			m_btnPlay->setPosition(Vec2(m_PauseBG->getContentSize().width * -1.1f,
				m_PauseBG->getContentSize().height * 0.55f));
			m_btnPlay->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_PauseBG->addChild(m_btnPlay);
		}

        m_btnExit = CMyButton::create("exitButton.png",
                                     END,
                                     std::bind(&CPausePopup::GameExit, this),
                                     EFFECT_ALPHA);
        
        if (m_btnExit != nullptr)
        {
            m_btnExit->setPosition(Vec2(m_BG->getContentSize().width * 0.92f,
                                       m_BG->getContentSize().height * 0.05f));
            m_btnExit->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_BG->addChild(m_btnExit);
            m_btnExit->setCascadeOpacityEnabled(true);
            m_btnExit->setOpacity(0);
        }
        
        
        m_btnHelp = CMyButton::create("helpButton.png",
                                      END,
                                      std::bind(&CPausePopup::Help, this),
                                      EFFECT_ALPHA);
        
        if (m_btnHelp != nullptr)
        {
            m_btnHelp->setPosition(Vec2(m_BG->getContentSize().width * 0.08f,
                                        m_BG->getContentSize().height * 0.05f));
            m_btnHelp->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_BG->addChild(m_btnHelp);
            m_btnHelp->setCascadeOpacityEnabled(true);
            m_btnHelp->setOpacity(0);
        }
        
        m_Popup->setPopupOpenEffectFunc([this](CPopup* pausePopup){
			auto winSize = Director::getInstance()->getWinSize();

			m_Popup->scheduleOnce([this](float delta){
				m_btnHome->runAction(EaseExponentialOut::create(MoveTo::create(1.0f, Vec2(m_PauseBG->getContentSize().width * 0.15f,
					m_PauseBG->getContentSize().height * 0.85f))));

				m_btnReset->runAction(EaseExponentialOut::create(MoveTo::create(1.0f, Vec2(m_PauseBG->getContentSize().width * 0.15f,
					m_PauseBG->getContentSize().height * 0.7f))));

				m_btnPlay->runAction(EaseExponentialOut::create(MoveTo::create(1.0f, Vec2(m_PauseBG->getContentSize().width * 0.15f,
					m_PauseBG->getContentSize().height * 0.55f))));
			}, 0.1f, "PausePopupOpen");
	
			m_PauseBG->runAction(EaseExponentialOut::create(MoveTo::create(0.5f, Vec2(0, winSize.height * 0.36f))));
            m_btnExit->runAction(FadeIn::create(0.5f));
            m_btnHelp->runAction(FadeIn::create(0.5f));
        });

		m_Popup->setPopupCloseEffectFunc([this, visibleSize, origin](CPopup* pausePopup){

			m_btnHome->runAction(EaseSineIn::create(MoveTo::create(0.4f, Vec2(m_PauseBG->getContentSize().width * -1.1f,
				m_PauseBG->getContentSize().height * 0.85f))));

			m_btnReset->runAction(EaseSineIn::create(MoveTo::create(0.4f, Vec2(m_PauseBG->getContentSize().width * -1.1f,
				m_PauseBG->getContentSize().height * 0.7f))));

			m_btnPlay->runAction(EaseSineIn::create(MoveTo::create(0.4f, Vec2(m_PauseBG->getContentSize().width * -1.1f,
				m_PauseBG->getContentSize().height * 0.55f))));

            m_btnExit->runAction(FadeTo::create(0.5f, 0));
            m_btnHelp->runAction(FadeTo::create(0.5f, 0));
            
			m_Popup->scheduleOnce([this, visibleSize, origin](float delta){
				m_PauseBG->runAction(Sequence::create(EaseSineIn::create(MoveTo::create(0.3f, Vec2(0, origin.x + visibleSize.height * 0.75f))),
					CallFunc::create([this](){
					CSpecificPopupBase::PopupRelease();
				}), nullptr));
			}, 0.1f, "PausePopupClose");
		});
	}
	catch (...){
		throw StringUtils::format("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	return true;
}

void CPausePopup::Play(){
	CCLOG("format popup Play");
	CGameScene::getGameScene()->GameResume();
	CSpecificPopupBase::PopupClose();
}

void CPausePopup::Reset(){
	CCLOG("format popup Replay");
	CGameScene::getGameScene()->resetGameScene();
}

void CPausePopup::GoHome(){
	CCLOG("format popup GoHome");
    CGameScene::getGameScene()->backToMenuScene();
	CSpecificPopupBase::PopupClose();
}

void CPausePopup::GameExit(){
    CCLOG("format popup GameExit");
    CGameScene::getGameScene()->GameExit();
    CSpecificPopupBase::PopupClose();
}

void CPausePopup::Help()
{
    CGameScene::getGameScene()->GameHelp();
}

