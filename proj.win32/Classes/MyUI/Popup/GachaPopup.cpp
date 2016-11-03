#include "GachaPopup.h"
#include "../MyButton.h"
#include "../UserCoinButton.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/CharacterDataManager.h"
#include "../../DataManager/UserDataManager.h"
#include <vector>

const int g_coinToGacha = 1500;

CGachaPopup* CGachaPopup::create()
{
	CGachaPopup *pRet = new(std::nothrow) CGachaPopup();
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

bool CGachaPopup::initVariable()
{
	try{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		m_BG = LayerColor::create(Color4B(255, 255, 255, 0), 1080.f, 1920.f);
		if (m_BG != nullptr){
			m_BG->setIgnoreAnchorPointForPosition(false);
			m_BG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_BG->setPosition(Vec2::ZERO);
			m_Popup->addChild(m_BG);
		}

		m_GachaBG = LayerColor::create(Color4B(255, 255, 255, 255 * 0.8f), 1080.f, 1500.f);
		if (m_GachaBG != nullptr){
			m_GachaBG->setIgnoreAnchorPointForPosition(false);
			m_GachaBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_GachaBG->setPosition(Vec2(0, origin.x + visibleSize.height * 1.5f));
			m_Popup->addChild(m_GachaBG);
		}

		m_btnExit = CMyButton::create("endIcon.png",
			END,
			[=](Node* sender){this->End(sender); },
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

		auto btnGacha = CMyButton::createWithLayerColor(Size(250, 150),
			Color4B(0, 0, 0, 255 * 0.8f),
			MakeString("Get -%d", g_coinToGacha),
			40,
			Color3B::WHITE,
			END,
			[=](Node* sender){this->PlayGacha(sender); },
			EFFECT_ALPHA);
		if (btnGacha != nullptr)
		{
			btnGacha->setPosition(Vec2(m_GachaBG->getContentSize().width * 0.5f, m_GachaBG->getContentSize().height * 0.15f));
			btnGacha->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_GachaBG->addChild(btnGacha);
		}

		m_btnUserCoin = CUserCoinButton::create();
		if (m_btnUserCoin != nullptr)
		{
			m_btnUserCoin->setPosition(Vec2(m_BG->getContentSize().width * 0.5f,
				m_BG->getContentSize().height * 0.05f));
			m_btnUserCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_BG->addChild(m_btnUserCoin);
			m_btnUserCoin->setCascadeOpacityEnabled(true);
			m_btnUserCoin->setOpacity(0);
		}

		m_Popup->setPopupOpenEffectFunc([this](CPopup* pausePopup){
			auto winSize = Director::getInstance()->getWinSize();
			m_GachaBG->runAction(EaseExponentialOut::create(MoveTo::create(0.8f, Vec2(0, winSize.height * 0.12f))));
			m_btnExit->runAction(FadeIn::create(0.5f));
			m_btnUserCoin->runAction(FadeIn::create(0.5f));
		});

		m_Popup->setPopupCloseEffectFunc([this, visibleSize, origin](CPopup* pausePopup){
			m_btnExit->runAction(FadeTo::create(0.5f, 0));
			m_btnUserCoin->runAction(FadeTo::create(0.5f, 0));
			m_Popup->scheduleOnce([this, visibleSize, origin](float delta){
				m_GachaBG->runAction(Sequence::create(EaseSineIn::create(MoveTo::create(0.4f, Vec2(0, origin.x + visibleSize.height * 1.5f))),
					CallFunc::create([this](){
					CSpecificPopupBase::PopupRelease();
				}), nullptr));
			}, 0.1f, "GachaPopupClose");
		});
	}
	catch (...){
		throw StringUtils::format("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	return true;
}

void CGachaPopup::End(Node* sender){
	CCLOG("format popup End");
	CSpecificPopupBase::PopupClose();
}

void CGachaPopup::PlayGacha(Node* sender){
    
    if (CUserDataManager::Instance()->CoinUpdate(-g_coinToGacha)){
        CCLOG("format popup PlayGacha");
        
        int allCharacterCount = static_cast<int>(CCharacterDataManager::Instance()->getCharacterList().size());
        auto haveCharacterList = CUserDataManager::Instance()->getUserData_List("USER_CHARACTER_LIST");
        int currentCharacterCount = static_cast<int>(haveCharacterList->size());
        
        if (allCharacterCount < haveCharacterList->size())
            CCASSERT(false, "Error : Character count should be less than haveCharacterList count.");
        
        
        int randomIdx = 0;
        // 뽑을 캐릭터가 더이상 없을 때
        if (allCharacterCount <= currentCharacterCount){
            CCLOG("No more Character");
            return;
        }
        else
        {
            do{
                randomIdx = random<int>(0, allCharacterCount - 1);
            } while (CUserDataManager::Instance()->getUserData_IsItemHave("USER_CHARACTER_LIST", randomIdx));
        }
        
        CCLOG("GET %d", randomIdx);
        CUserDataManager::Instance()->setUserData_ItemGet("USER_CHARACTER_LIST", randomIdx);
        CUserDataManager::Instance()->setUserData_Number("USER_CUR_CHARACTER", randomIdx);
    }
}