#include "WorkshopPopup.h"
#include "WorkshopPopupDP.h"
#include "../MyButton.h"
#include "../UserCoinButton.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/WorkshopItemDataManager.h"
#include "ui/UIScrollView.h"
#include "ui/UIImageView.h"
#include "ui/UIPageView.h"

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
        
        auto itemScroll = ScrollView::create();
        if(itemScroll != nullptr){
            
            /* 아이템리스트 데이터 읽음 */
			auto itemList = CWorkshopItemDataManager::Instance()->getWorkshopItemList();
			size_t listCount = itemList.size();
            size_t dpDistance = 15;
            Size dpSize = Size(1080, 200);
            
            itemScroll->setDirection(ScrollView::Direction::VERTICAL);
            itemScroll->setBounceEnabled(true);
            itemScroll->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            itemScroll->setPosition(Vec2(m_ScrollBack->getContentSize().width * 0.5f, m_ScrollBack->getContentSize().height * 0.5f));
            
			int dpIdx = 0;
            for(auto item : itemList)
            {
				if (item._isSelling){
					auto items = CWorkshopPopupDP::create(item, std::bind(&CWorkshopPopup::Select, this, std::placeholders::_1/*= 호출하는 곳의 인자를 사용한다.*/));
					items->setPosition(Vec2(dpSize.width * 0.5f,
						(dpSize.height + dpDistance) * dpIdx + (dpSize.height + dpDistance)));
					items->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
					itemScroll->addChild(items);
					dpIdx++;
				}
            }
            
            itemScroll->setContentSize(Size(m_ScrollBack->getContentSize().width, (dpSize.height + dpDistance) * 4));
            itemScroll->setInnerContainerSize(Size(dpSize.width, (dpSize.height + dpDistance) * listCount));
            
            m_ScrollBack->addChild(itemScroll);
        }
        
        m_btnEnd = CMyButton::create("endIcon.png",
                                     END,
                                     std::bind(&CWorkshopPopup::End, this),
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

void CWorkshopPopup::End(){
    CCLOG("format popup End");
    CSpecificPopupBase::PopupClose();
}

void CWorkshopPopup::Select(cocos2d::Ref* dp)
{
    auto selectDP = dynamic_cast<CWorkshopPopupDP*>(dp);
    if (selectDP != nullptr)
    {
        selectDP->DeSelect();
    }
}