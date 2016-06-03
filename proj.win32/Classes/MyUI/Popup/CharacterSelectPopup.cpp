#include "CharacterSelectPopup.h"
#include "../MyButton.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/CharacterDataManager.h"
#include "ui/UIScrollView.h"
#include "ui/UIImageView.h"
#include "ui/UIPageView.h"

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
        
        m_BG = Sprite::create("empty1920.png");
        if(m_BG != nullptr){
            m_BG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_BG->setPosition(Vec2::ZERO);
            m_Popup->addChild(m_BG);
        }
        
        m_ScrollBack = Sprite::create("empty1500_w.png");
        if(m_ScrollBack != nullptr){
            m_ScrollBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_ScrollBack->setPosition(Vec2(0, origin.x + visibleSize.height * 1.5f));
            m_ScrollBack->setOpacity(255 * 0.8f);
            m_Popup->addChild(m_ScrollBack);
        }
        
        auto characterScroll = ScrollView::create();
        if(characterScroll != nullptr){
            
            /* 캐릭터리스트 데이터 읽음 */
            auto characterList = CCharacterDataManager::Instance()->getCharacterList();
            size_t listCount = characterList->size();
            size_t dpDistance = 10;
            
            characterScroll->setDirection(ScrollView::Direction::HORIZONTAL);
            characterScroll->setBounceEnabled(true);
            characterScroll->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            characterScroll->setPosition(Vec2(m_ScrollBack->getContentSize().width * 0.5f, m_ScrollBack->getContentSize().height * 0.5f));
        
            for(int dpIdx = 0 ; dpIdx < listCount ;dpIdx++ )
            {
                auto characters = Sprite::create("planet.png");
                characters->setPosition(Vec2((characters->getContentSize().width + dpDistance) * dpIdx
                                             + (characters->getContentSize().width * 0.5f),
                                             characterScroll->getContentSize().height * 0.5f));
                characterScroll->addChild(characters);
            }
            
            characterScroll->setContentSize(Size(400, m_ScrollBack->getContentSize().height * 0.7f));
            characterScroll->setInnerContainerSize(Size(400 * listCount, 500));
            
            characterScroll->addEventListenerScrollView(<#cocos2d::Ref *target#>, <#SEL_ScrollViewEvent selector#>)
            
            m_ScrollBack->addChild(characterScroll);
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
        
        
        m_Popup->setPopupOpenEffectFunc([this](CPopup* pausePopup){
            auto winSize = Director::getInstance()->getWinSize();
            
            m_Popup->scheduleOnce([this](float delta){
            
            
            }, 0.1f, "CharacterSelectPopupOpen");
            
            m_ScrollBack->runAction(EaseExponentialOut::create(MoveTo::create(0.8f, Vec2(0, winSize.height * 0.12f))));
            m_btnEnd->runAction(FadeIn::create(0.5f));
        });
        
        m_Popup->setPopupCloseEffectFunc([this, visibleSize, origin](CPopup* pausePopup){
            
            
            m_btnEnd->runAction(FadeTo::create(0.5f, 0));
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