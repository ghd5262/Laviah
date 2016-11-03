#include "WorkshopPopup.h"
#include "WorkshopPopupDP.h"
#include "../MyButton.h"
#include "../UserCoinButton.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/WorkshopItemDataManager.h"
#include "../../DataManager/UserDataManager.h"
#include "ui/UIListView.h"

CWorkshopPopup* CWorkshopPopup::create()
{
    CWorkshopPopup *pRet = new(std::nothrow) CWorkshopPopup();
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

bool CWorkshopPopup::initVariable()
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
        
        /* workShop label*/
        auto workShopLabel = Label::createWithTTF("Workshop", "fonts/malgunbd.ttf", 80);
        if (workShopLabel != nullptr)
        {
            workShopLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            workShopLabel->setPosition(Vec2(m_ScrollBack->getContentSize().width * 0.5f, m_ScrollBack->getContentSize().height * 0.9f));
            workShopLabel->setColor(g_labelColor1);
            m_ScrollBack->addChild(workShopLabel);
            workShopLabel->setOpacity(0);
        }
        
		auto itemList = CWorkshopItemDataManager::Instance()->getSellingWorkshopItemList();
		Size layerSize = m_ScrollBack->getContentSize();
		Size dpSize = Size(1080, 200);
		size_t dpDistance = 15;
		float spawnCount = 4;
        
        unsigned currentItemIdx = CUserDataManager::Instance()->getUserData_Number("USER_CUR_SELECT_ITEM");
        
		// Create the list view
		auto listView = ListView::create();
		if (listView != nullptr){
			listView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
			listView->setBounceEnabled(true);
			listView->setBackGroundImageScale9Enabled(true);
			listView->setContentSize(Size(m_ScrollBack->getContentSize().width, (dpSize.height + dpDistance) * spawnCount));
			listView->setScrollBarPositionFromCorner(Vec2(7, 7));
			listView->setItemsMargin(dpDistance);
			listView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			listView->setPosition(layerSize / 2);
			listView->setMagneticType(ListView::MagneticType::CENTER);
			listView->ScrollView::addEventListener((ui::ListView::ccScrollViewCallback)std::bind(&CWorkshopPopup::ScrollCallback, this, std::placeholders::_1, std::placeholders::_2));
			m_ScrollBack->addChild(listView);

            unsigned dpIdx = 0;
            unsigned currentItemDPIdx = 0;
            
			for (auto item : itemList)
			{
				auto itemDP = CWorkshopPopupDP::create(item);
				itemDP->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				listView->pushBackCustomItem(itemDP);
                
                if( item._idx == currentItemIdx ){
                    currentItemDPIdx = dpIdx;
                }
                dpIdx++;
			}
            
            // Scrolling to current character
            Director::getInstance()->getScheduler()->schedule([listView, currentItemDPIdx](float delta){
                listView->scrollToItem(currentItemDPIdx, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE, 0.f);
            }, Director::getInstance(), 0.f, 0, 0.f, false, "ScrollToItem");
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

        m_Popup->setPopupOpenEffectFunc([this, workShopLabel](CPopup* pausePopup){
            auto winSize = Director::getInstance()->getWinSize();

            workShopLabel->runAction(FadeIn::create(0.5f));
            m_ScrollBack->runAction(EaseExponentialOut::create(MoveTo::create(0.8f, Vec2(0, winSize.height * 0.12f))));
            m_btnEnd->runAction(FadeIn::create(0.5f));
			m_btnUserCoin->runAction(FadeIn::create(0.5f));
        });
        
        m_Popup->setPopupCloseEffectFunc([this, visibleSize, origin, workShopLabel](CPopup* pausePopup){
            
            workShopLabel->runAction(FadeTo::create(0.5f, 0));
            m_btnEnd->runAction(FadeTo::create(0.5f, 0));
			m_btnUserCoin->runAction(FadeTo::create(0.5f, 0));
            m_Popup->scheduleOnce([this, visibleSize, origin](float delta){
                m_ScrollBack->runAction(Sequence::create(EaseSineIn::create(MoveTo::create(0.4f, Vec2(0, origin.x + visibleSize.height * 1.5f))),
                                                         CallFunc::create([this](){
                    CSpecificPopupBase::PopupRelease();
                }), nullptr));
            }, 0.1f, "WorkshopPopupClose");
        });
    }
    catch (...){
        throw StringUtils::format("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
        return false;
    }
    return true;
}

void CWorkshopPopup::End(Node* sender){
    CCLOG("format popup End");
    CSpecificPopupBase::PopupClose();
}

void CWorkshopPopup::ScrollCallback(cocos2d::Ref* ref, cocos2d::ui::ScrollView::EventType type)
{
	ListView* listView = dynamic_cast<ListView*>(ref);
	if (listView == nullptr || type != ScrollView::EventType::CONTAINER_MOVED)
	{
		return;
	}
}