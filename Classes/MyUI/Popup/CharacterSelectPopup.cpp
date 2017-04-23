#include "CharacterSelectPopup.h"
#include "CharacterSelectPopupDP.h"
#include "../MyButton.h"
#include "../UserCoinButton.h"
#include "../MenuLayer.hpp"
#include "../../Scene/GameScene.h"
#include "../../DataManager/CharacterDataManager.h"
#include "../../DataManager/UserDataManager.h"
#include "../../GameObject/ObjectManager.h"
#include "../../GameObject/Planet.h"
#include "../../GameObject/Player.h"

using namespace cocos2d;
using namespace cocos2d::ui;

CCharacterSelectPopup* CCharacterSelectPopup::create()
{
	CCharacterSelectPopup *pRet = new(std::nothrow) CCharacterSelectPopup();
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

bool CCharacterSelectPopup::init()
{
	if (!CPopup::init()) return false;

	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto bg = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080.f, 1920.f);
	if (bg != nullptr){
		bg->setIgnoreAnchorPointForPosition(false);
		bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bg->setPosition(this->getContentSize() / 2);
		this->addChild(bg);
	}

	auto scrollBack = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080.f, 2100.f);
	if (scrollBack != nullptr){
		scrollBack->setIgnoreAnchorPointForPosition(false);
		scrollBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		scrollBack->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 1.5f));
		this->addChild(scrollBack);
	}

	/* select label*/
	//        auto selectLabel = Label::createWithTTF(TRANSLATE("BUTTON_SELECT"), FONT::MALGUNBD, 80);
	//        if (selectLabel != nullptr)
	//        {
	//            selectLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	//            selectLabel->setPosition(Vec2(scrollBack->getContentSize().width * 0.5f, scrollBack->getContentSize().height * 0.9f));
	//            selectLabel->setColor(COLOR::DARKGRAY);
	//            scrollBack->addChild(selectLabel);
	//            selectLabel->setOpacity(0);
	//        }


	auto characterList = CCharacterDataManager::Instance()->getCharacterList();
	Size layerSize = scrollBack->getContentSize();
	size_t dpDistance = 50;
	unsigned currentCharacterIdx = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::CHARACTER);
    unsigned dpIdx = 0;
    unsigned currentCharacterDPIdx = 0;
    
	// Create the list view
	auto listView = ListView::create();
	if (listView != nullptr){
		listView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
		listView->setBounceEnabled(true);
		listView->setBackGroundImageScale9Enabled(true);
		listView->setContentSize(Size(layerSize.width, layerSize.height));
		listView->setScrollBarPositionFromCorner(Vec2(7, 7));
        listView->setScrollBarEnabled(false);
		listView->setItemsMargin(dpDistance);
		listView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		listView->setPosition(layerSize / 2);
		listView->setMagneticType(ListView::MagneticType::CENTER);
        listView->setTouchEnabled(false);
		listView->ScrollView::addEventListener((ui::ListView::ccScrollViewCallback)std::bind(&CCharacterSelectPopup::ScrollCallback, this, std::placeholders::_1, std::placeholders::_2));
		scrollBack->addChild(listView);



		for (auto iter : characterList)
		{
            auto character = iter.second;
			if (!character->_prepared) continue;

			auto characterDP = CCharacterSelectPopupDP::create(character);
			characterDP->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            characterDP->setDPIndex(dpIdx);
            characterDP->setCascadeOpacityEnabled(true);
            characterDP->setSelectDPListener([=](int dpIndex){
                if(!listView->isTouchEnabled()) return;
                if(m_CenterDP)
                {
                    if(dpIndex == m_CenterDP->getDPIndex())
                        this->Select(m_CenterDP);
                }
                listView->scrollToItem(dpIndex, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE, 0.5f);
            });
			listView->pushBackCustomItem(characterDP);

			if (character->_idx == currentCharacterIdx){
				currentCharacterDPIdx = dpIdx;
			}
            dpIdx++;
		}

		// Scrolling to current character
//		this->scheduleOnce([=](float delta){
//			listView->jumpToItem(currentCharacterDPIdx, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
//		}, 0.f, "ScrollToItem");
	}

	auto currentCharacterParam = CCharacterDataManager::Instance()->getCharacterByIndex(currentCharacterIdx);
    m_CenterCharacterNameLabel = Label::createWithSystemFont(TRANSLATE(currentCharacterParam->_name),
                                                             FONT::MALGUNBD, 80,
                                                             Size(layerSize.width * 0.8f, layerSize.height),
                                                             TextHAlignment::CENTER,
                                                             TextVAlignment::CENTER);
    if (m_CenterCharacterNameLabel != nullptr)
	{
		m_CenterCharacterNameLabel->setPosition(Vec2(bg->getContentSize().width * 0.5f,
                                                     bg->getContentSize().height * 0.8f));
        m_CenterCharacterNameLabel->setColor(Color3B::WHITE);
		bg->addChild(m_CenterCharacterNameLabel);
		m_CenterCharacterNameLabel->setOpacity(0);
	}

//	auto btnUserCoin = CUserCoinButton::create();
//	if (btnUserCoin != nullptr)
//	{
//		btnUserCoin->setPosition(Vec2(bg->getContentSize().width * 0.5f,
//			bg->getContentSize().height * 0.05f));
//		btnUserCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//		btnUserCoin->setCascadeOpacityEnabled(true);
//		btnUserCoin->setOpacity(0);
//		bg->addChild(btnUserCoin);
//	}

//    m_btnSelect = CMyButton::create()
//    ->addEventListener([=](Node* sender){
//        this->Select(sender);
//    })
//    ->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, 200, 200))
////    ->setContents(TRANSLATE("BUTTON_SELECT"))
//    ->setButtonPosition(Vec2(scrollBack->getContentSize().width * 0.5f,
//                             scrollBack->getContentSize().height * 0.5f))
//    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
//    ->show(scrollBack, ZORDER::POPUP);
//
//	m_btnSelect->setOpacity(0);


    auto btnEnd = CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->End(sender);
    })
    ->setButtonSingleUse(true)
    ->setButtonNormalImage("endIcon.png")
    ->setButtonPosition(Vec2(bg->getContentSize().width * 0.92f,
                             bg->getContentSize().height * 0.05f))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(bg);
    
	btnEnd->setOpacity(0);
    btnEnd->setTouchEnabled(false);

	this->setOpenAnimation([=](Node* sender){
        auto action = [=](Node* owner){
            auto delay = DelayTime::create(1.f);
            auto fade  = FadeIn::create(0.5f);
            auto sequence = Sequence::createWithTwoActions(delay, fade);
            owner->runAction(sequence);
        };
		action(m_CenterCharacterNameLabel);
		action(btnEnd);
//        action(m_btnSelect);
        
//		btnUserCoin->runAction(FadeIn::create(0.5f));
		scrollBack->runAction(EaseExponentialOut::create(MoveTo::create(0.8f, Vec2(visibleSize.width * 0.5f,
                                                                                   visibleSize.height * 0.62f))));
        listView->jumpToItem(currentCharacterDPIdx, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
        m_CenterDP->setVisible(false);
        m_CenterDP->runAction(Sequence::createWithTwoActions(DelayTime::create(1.f), CallFunc::create([=](){
            m_CenterDP->setVisible(true);
            CObjectManager::Instance()->getPlayer()->setVisible(false);
            btnEnd->setTouchEnabled(true);
            listView->setTouchEnabled(true);
            this->changeDefaultCallback([=](Node* sender){
                this->End(sender);
            });
            this->setDefaultCallbackCleanUp(true);
        })));
	});

	this->setCloseAnimation([=](Node* sender){
		m_CenterCharacterNameLabel->runAction(FadeTo::create(0.5f, 0));
		btnEnd->runAction(FadeTo::create(0.5f, 0));
//		btnUserCoin->runAction(FadeTo::create(0.5f, 0));
//		m_btnSelect->runAction(FadeTo::create(0.5f, 0));
		scrollBack->runAction(EaseSineIn::create(MoveTo::create(0.4f, Vec2(visibleSize.width * 0.5f,
                                                                           visibleSize.height * 1.5f))));
        
        auto curIndex = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::CHARACTER);
        if(m_CenterDP->getCharacterParam()->_idx == curIndex)
            m_CenterDP->setVisible(false);
	});
    
    this->setDefaultCallback([=](Node* sender){}, false);
    
	return true;
}

void CCharacterSelectPopup::End(Node* sender){
	CCLOG("format popup End");
    CObjectManager::Instance()->ZoomIn();
    CGameScene::getGameScene()->MenuFadeIn();
    CMenuLayer::Instance()->setVisible(true);
	this->popupClose(1.3f);
}

// When touch the select or buy button
void CCharacterSelectPopup::Select(Node* sender)
{
	if (m_CenterDP == nullptr) return;

	auto centerCharacterParam = m_CenterDP->getCharacterParam();

	if (CUserDataManager::Instance()->getUserData_IsItemHave(USERDATA_KEY::CHARACTER_LIST,
                                                             centerCharacterParam->_idx))
	{
		m_CenterDP->Select();
		CObjectManager::Instance()->ChangeCharacter();
		this->End(nullptr);
	}
	else{
//        if(!centerCharacterParam->_random) return;
        
        auto characterName = TRANSLATE(centerCharacterParam->_name);
        CGameScene::getGameScene()->CreateAlertPopup()
        ->setPositiveButton([=](Node* sender){
            m_CenterDP->Buy();
            //			m_btnSelect->changeContents(TRANSLATE("BUTTON_SELECT"));
            m_CenterCharacterNameLabel->setString(characterName);
            CObjectManager::Instance()->ChangeCharacter();
        }, TRANSLATE("BUTTON_YES"))
        ->setNegativeButton([=](Node* sender){
        }, TRANSLATE("BUTTON_NO"))
        ->setMessage(StringUtils::format(TRANSLATE("CHARACTER_BUY_CHECK").c_str(), characterName.c_str()))
        ->show(CGameScene::getGameScene(), ZORDER::POPUP);
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
    auto centerDP = dynamic_cast<CCharacterSelectPopupDP*>(centerChild);
    if (!centerDP) return;
    if (centerDP == m_CenterDP) return;
    m_CenterDP = centerDP;

	// Center dp size up
	m_CenterDP->Center();

	// Get CenterDP's Character Param
	auto centerCharacterParam = m_CenterDP->getCharacterParam();

    // Change name label
    m_CenterCharacterNameLabel->setString(TRANSLATE(centerCharacterParam->_name));
    
	// If do not have and no random item, Change the name string to ???
    if (!CUserDataManager::Instance()->getUserData_IsItemHave(USERDATA_KEY::CHARACTER_LIST, centerCharacterParam->_idx)){
        if(!centerCharacterParam->_random)
            m_CenterCharacterNameLabel->setString("???");
    }
	
	// Size down the other dp
	for (auto characterDP : listView->getChildren())
	{
		if (characterDP != nullptr && characterDP != center)
		{
			dynamic_cast<CCharacterSelectPopupDP*>(characterDP)->DeSelect();
		}
	}
}