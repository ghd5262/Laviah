#include "CharacterSelectPopup.h"
#include "CharacterSelectPopupDP.h"
#include "../MyButton.h"
#include "../UserCoinButton.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/CharacterDataManager.h"
#include "../../DataManager/UserDataManager.h"
#include "../../GameObject/MenuSceneObjectManager.h"
#include "../../GameObject/Planet.h"
#include "ui/UIListView.h"

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
            m_BG->setIgnoreAnchorPointForPosition(false);
            m_BG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_BG->setPosition(Vec2::ZERO);
            m_Popup->addChild(m_BG);
        }
        
        m_ScrollBack = LayerColor::create(Color4B(255, 255, 255, 255 * 0.8f), 1080.f, 1500.f);
        if(m_ScrollBack != nullptr){
            m_ScrollBack->setIgnoreAnchorPointForPosition(false);
            m_ScrollBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_ScrollBack->setPosition(Vec2(0, origin.x + visibleSize.height * 1.5f));
            m_Popup->addChild(m_ScrollBack);
        }
        
        /* select label*/
//        auto selectLabel = Label::createWithTTF("Select", "fonts/malgunbd.ttf", 80);
//        if (selectLabel != nullptr)
//        {
//            selectLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//            selectLabel->setPosition(Vec2(m_ScrollBack->getContentSize().width * 0.5f, m_ScrollBack->getContentSize().height * 0.9f));
//            selectLabel->setColor(g_labelColor1);
//            m_ScrollBack->addChild(selectLabel);
//            selectLabel->setOpacity(0);
//        }
        
        
        auto characterList = CCharacterDataManager::Instance()->getCharacterList();
        Size layerSize = m_ScrollBack->getContentSize();
        size_t dpDistance = 15;
        unsigned currentCharacterIdx = CUserDataManager::Instance()->getUserData_Number("USER_CUR_CHARACTER");
        
        // Create the list view
        auto listView = ListView::create();
        if (listView != nullptr){
            listView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
            listView->setBounceEnabled(true);
            listView->setBackGroundImageScale9Enabled(true);
            listView->setContentSize(Size(layerSize.width, layerSize.height * 0.5f));
            listView->setScrollBarPositionFromCorner(Vec2(7, 7));
            listView->setItemsMargin(dpDistance);
            listView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            listView->setPosition(layerSize / 2);
            listView->setMagneticType(ListView::MagneticType::CENTER);
            listView->ScrollView::addEventListener((ui::ListView::ccScrollViewCallback)std::bind(&CCharacterSelectPopup::ScrollCallback, this, std::placeholders::_1, std::placeholders::_2));
            m_ScrollBack->addChild(listView);
            
            unsigned dpIdx = 0;
            unsigned currentCharacterDPIdx = 0;
            
            for (auto character : characterList)
            {
                auto characterDP = CCharacterSelectPopupDP::create(character);
                characterDP->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                listView->pushBackCustomItem(characterDP);
                
                if( character._idx == currentCharacterIdx ){
                    currentCharacterDPIdx = dpIdx;
                }
                dpIdx++;
            }
            
            // Scrolling to current character
            Director::getInstance()->getScheduler()->schedule([listView, currentCharacterDPIdx](float delta){
                listView->scrollToItem(currentCharacterDPIdx, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE, 1.5f);
            }, Director::getInstance(), 0.f, 0, 0.3f, false, "ScrollToItem");
        }
        
        auto currentCharacterParam = CCharacterDataManager::Instance()->getCharacterInfoByIndex(currentCharacterIdx);
        m_CenterCharacterNameLabel = Label::createWithTTF(currentCharacterParam._name, "fonts/malgunbd.ttf", 80);
        if(m_CenterCharacterNameLabel != nullptr)
        {
            m_CenterCharacterNameLabel->setPosition(Vec2(m_ScrollBack->getContentSize().width * 0.5f, m_ScrollBack->getContentSize().height * 0.7f));
            m_CenterCharacterNameLabel->setColor(g_labelColor1);
            m_ScrollBack->addChild(m_CenterCharacterNameLabel);
            m_CenterCharacterNameLabel->setOpacity(0);
        }
        
        
        m_btnSelect = CMyButton::createWithLayerColor(Size(250, 150),
                                                      Color4B(0, 0, 0, 255 * 0.8f),
                                                      "Select",
                                                      40,
                                                      Color3B::WHITE,
                                                      END,
													  [=](Node* sender){this->Select(sender); },
                                                      EFFECT_ALPHA);
        if (m_btnSelect != nullptr)
        {
            m_btnSelect->setPosition(Vec2(m_ScrollBack->getContentSize().width * 0.5f,
                                          m_ScrollBack->getContentSize().height * 0.1f));
            m_btnSelect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_btnSelect->setCascadeOpacityEnabled(true);
            m_btnSelect->setOpacity(0);
            m_ScrollBack->addChild(m_btnSelect);
        }
        
		m_btnUserCoin = CUserCoinButton::create();
		if (m_btnUserCoin != nullptr)
		{
			m_btnUserCoin->setPosition(Vec2(m_BG->getContentSize().width * 0.5f,
				m_BG->getContentSize().height * 0.05f));
			m_btnUserCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_btnUserCoin->setCascadeOpacityEnabled(true);
			m_btnUserCoin->setOpacity(0);
            m_BG->addChild(m_btnUserCoin);
		}
        
        m_btnEnd = CMyButton::create("endIcon.png",
                                     END,
									 [=](Node* sender){this->End(sender); },
                                     EFFECT_ALPHA);
        
        if (m_btnEnd != nullptr)
        {
            m_btnEnd->setPosition(Vec2(m_BG->getContentSize().width * 0.92f,
                                       m_BG->getContentSize().height * 0.05f));
            m_btnEnd->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_btnEnd->setCascadeOpacityEnabled(true);
            m_btnEnd->setOpacity(0);
            m_BG->addChild(m_btnEnd);
        }
        
        m_Popup->setPopupOpenEffectFunc([this](CPopup* pausePopup){
            auto winSize = Director::getInstance()->getWinSize();
       
            m_CenterCharacterNameLabel->runAction(FadeIn::create(0.5f));
            m_ScrollBack->runAction(EaseExponentialOut::create(MoveTo::create(0.8f, Vec2(0, winSize.height * 0.12f))));
            m_btnEnd->runAction(FadeIn::create(0.5f));
			m_btnUserCoin->runAction(FadeIn::create(0.5f));
            m_btnSelect->runAction(FadeIn::create(0.5f));
        });
        
        m_Popup->setPopupCloseEffectFunc([this, visibleSize, origin](CPopup* pausePopup){
            
            m_CenterCharacterNameLabel->runAction(FadeTo::create(0.5f, 0));
            m_btnEnd->runAction(FadeTo::create(0.5f, 0));
			m_btnUserCoin->runAction(FadeTo::create(0.5f, 0));
            m_btnSelect->runAction(FadeTo::create(0.5f, 0));
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

void CCharacterSelectPopup::End(Node* sender){
    CCLOG("format popup End");
    CSpecificPopupBase::PopupClose();
}

// When touch the select or buy button
void CCharacterSelectPopup::Select(Node* sender)
{
    if(m_CenterDP == nullptr) return;
    
    auto centerCharacterParam = m_CenterDP->getCharacterParam();

	auto planet = CMenuSceneObjectManager::Instance()->getPlanet();
	if (planet != nullptr)
		planet->setPlanetTexture(centerCharacterParam._planetTextureName);
    
	if(CUserDataManager::Instance()->getUserData_IsItemHave("USER_CHARACTER_LIST", centerCharacterParam._idx))
    {
        m_CenterDP->Select();
    }else{
        m_CenterDP->Buy();
        m_btnSelect->getBtnLabel()->setString("Select");
		m_btnUserCoin->UpdateUI();
    }
}

void CCharacterSelectPopup::ScrollCallback(cocos2d::Ref* ref, cocos2d::ui::ScrollView::EventType type)
{
    ListView* listView = dynamic_cast<ListView*>(ref);
    if (listView == nullptr || type != ScrollView::EventType::CONTAINER_MOVED)
    {
        return;
    }

    // Get center dp
    auto center = listView->getCenterItemInCurrentView();
    auto centerIdx = listView->getIndex(center);
    auto centerChild = listView->getChildren().at(centerIdx);
    m_CenterDP = dynamic_cast<CCharacterSelectPopupDP*>(centerChild);
    if (m_CenterDP == nullptr)
        return;
    
    // Center dp size up
    m_CenterDP->Center();
    
    // Get CenterDP's Character Param
    auto centerCharacterParam = m_CenterDP->getCharacterParam();
    
    // If already have the Center Character, Change the Button String to "Select"
    if(CUserDataManager::Instance()->getUserData_IsItemHave("USER_CHARACTER_LIST", centerCharacterParam._idx))
        m_btnSelect->getBtnLabel()->setString("Select");
    else// If do not have, Change the Button String to "buy cost"
        m_btnSelect->getBtnLabel()->setString(MakeString("$ %d", centerCharacterParam._cost));
    
    // Change name label
    m_CenterCharacterNameLabel->setString(centerCharacterParam._name);
    
    // Size down the other dp
    for(auto characterDP : listView->getChildren())
    {
        if(characterDP != nullptr && characterDP != center)
        {
            dynamic_cast<CCharacterSelectPopupDP*>(characterDP)->DeSelect();
        }
    }
}