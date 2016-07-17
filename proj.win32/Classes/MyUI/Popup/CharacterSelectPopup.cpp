#include "CharacterSelectPopup.h"
#include "CharacterSelectPopupDP.h"
#include "../MyButton.h"
#include "../UserCoinButton.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/CharacterDataManager.h"
#include "../../DataManager/UserDataManager.h"

CCharacterSelectPopup* CCharacterSelectPopup::create()
{
    CCharacterSelectPopup *pRet = new(std::nothrow) CCharacterSelectPopup();
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

bool CCharacterSelectPopup::initVariable()
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
        
        m_ScrollBack = LayerColor::create(Color4B(255, 255, 255, 255 * 0.8f), 1080.f, 1500.f);
        if(m_ScrollBack != nullptr){
            m_ScrollBack->ignoreAnchorPointForPosition(false);
            m_ScrollBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_ScrollBack->setPosition(Vec2(0, origin.x + visibleSize.height * 1.5f));
            m_Popup->addChild(m_ScrollBack);
        }
        
        /* select label*/
        auto selectLabel = Label::createWithTTF("Select", "fonts/malgunbd.ttf", 80);
        if (selectLabel != nullptr)
        {
            selectLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            selectLabel->setPosition(Vec2(m_ScrollBack->getContentSize().width * 0.5f, m_ScrollBack->getContentSize().height * 0.9f));
            selectLabel->setColor(g_labelColor1);
            m_ScrollBack->addChild(selectLabel);
            selectLabel->setOpacity(0);
        }
        
        m_ScrollView = ScrollView::create();
        if(m_ScrollView != nullptr){
            
            /* 캐릭터리스트 데이터 읽음 */
            auto characterList = CCharacterDataManager::Instance()->getCharacterList();
            size_t listCount = characterList.size();
            size_t dpDistance = 15;
			Size dpSize = Size(540, 915);

            m_ScrollView->setDirection(ScrollView::Direction::HORIZONTAL);
            m_ScrollView->setBounceEnabled(true);
            m_ScrollView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_ScrollView->setPosition(Vec2(m_ScrollBack->getContentSize().width * 0.5f, m_ScrollBack->getContentSize().height * 0.5f));
        
            for(int dpIdx = 0 ; dpIdx < listCount ;dpIdx++ )
            {
				auto characters = CCharacterSelectPopupDP::create(dpIdx, std::bind(&CCharacterSelectPopup::Select, this, std::placeholders::_1/*= 호출하는 곳의 인자를 사용한다.*/));
				characters->setPosition(Vec2((dpSize.width + dpDistance) * dpIdx
					+ (dpSize.width * 0.5f),
					dpSize.height * 0.5f));
				characters->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);

				// 현재 선택된 캐릭터는 DP Select로 변경
				if (dpIdx == CUserDataManager::Instance()->getUserDataCurrentCharacter())
					characters->Select();

                m_ScrollView->addChild(characters);
            }
            
			m_ScrollView->setContentSize(Size(m_ScrollBack->getContentSize().width, dpSize.height));
			m_ScrollView->setInnerContainerSize(Size((dpSize.width + dpDistance)* listCount, dpSize.height));
            
            m_ScrollBack->addChild(m_ScrollView);
        }
        
        m_btnEnd = CMyButton::create("endIcon.png",
                                     END,
                                     std::bind(&CCharacterSelectPopup::End, this),
                                     EFFECT_ALPHA);
        
        if (m_btnEnd != nullptr)
        {
            m_btnEnd->setPosition(Vec2(m_BG->getContentSize().width * 0.92f,
                                       m_BG->getContentSize().height * 0.05f));
            m_btnEnd->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_BG->addChild(m_btnEnd);
            m_btnEnd->setCascadeOpacityEnabled(true);
            m_btnEnd->setOpacity(0);
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
        
        m_Popup->setPopupOpenEffectFunc([this, selectLabel](CPopup* pausePopup){
            auto winSize = Director::getInstance()->getWinSize();
       
            selectLabel->runAction(FadeIn::create(0.5f));
            m_ScrollBack->runAction(EaseExponentialOut::create(MoveTo::create(0.8f, Vec2(0, winSize.height * 0.12f))));
            m_btnEnd->runAction(FadeIn::create(0.5f));
			m_btnUserCoin->runAction(FadeIn::create(0.5f));
        });
        
        m_Popup->setPopupCloseEffectFunc([this, visibleSize, origin, selectLabel](CPopup* pausePopup){
            
            selectLabel->runAction(FadeTo::create(0.5f, 0));
            m_btnEnd->runAction(FadeTo::create(0.5f, 0));
			m_btnUserCoin->runAction(FadeTo::create(0.5f, 0));
            m_Popup->scheduleOnce([this, visibleSize, origin](float delta){
                m_ScrollBack->runAction(Sequence::create(EaseSineIn::create(MoveTo::create(0.4f, Vec2(0, origin.x + visibleSize.height * 1.5f))),
                                                 CallFunc::create([this](){
                    CSpecificPopupBase::PopupRelease();
                }), nullptr));
            }, 0.1f, "CharacterSelectPopupClose");
        });
    }
    catch (...){
        throw StringUtils::format("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
        return false;
    }
    return true;
}

void CCharacterSelectPopup::End(){
    CCLOG("format popup End");
    CSpecificPopupBase::PopupClose();
}

void CCharacterSelectPopup::Select(cocos2d::Ref* dp)
{
	auto selectDP = dynamic_cast<CCharacterSelectPopupDP*>(dp);
	if (selectDP != nullptr)
	{
        for(auto child : m_ScrollView->getChildren())
        {
            if(child != nullptr && child != selectDP)
            {
                dynamic_cast<CCharacterSelectPopupDP*>(child)->DeSelect();
            }
        }
	}
}