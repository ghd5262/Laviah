#include "FacebookRankPopupDP.hpp"
#include "../Popup.h"
#include "../MyButton.h"
#include "../UrlSprite.hpp"
#include "../../DataManager/UserDataManager.h"
#include "../../Common/StringUtility.h"
#include "../../Scene/GameScene.h"

CFacebookRankPopupDP* CFacebookRankPopupDP::create(const FBUSER_PARAM* user, int number, bool me/* = false*/)
{
    CFacebookRankPopupDP *pRet = new(std::nothrow) CFacebookRankPopupDP(user, number, me);
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

bool CFacebookRankPopupDP::init()
{
    if (!Widget::init()) return false;
    
    auto bg = LayerColor::create(COLOR::BRIGHTGRAY_ALPHA, 1080.f, 200.f);
    if (bg != nullptr){
        this->setContentSize(bg->getContentSize());
        bg->setIgnoreAnchorPointForPosition(false);
        bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        bg->setPosition(this->getContentSize() / 2);
        this->addChild(bg);
    }
    
    if(!m_IsMyData){
        CMyButton::create()
        ->addEventListener([=](Node* sender){
            CGameScene::getGameScene()->CreateAlertPopup()
            ->setPositiveButton([=](Node* sender){
                this->FBShare();
            }, TRANSLATE("BUTTON_YES"))
            ->setNegativeButton([=](Node* sender){
            }, TRANSLATE("BUTTON_NO"))
            ->setMessage("페이스북 친구들과 공유하시겠습니까?")
            ->show(CGameScene::getGameScene(), ZORDER::POPUP);
        })
        ->setButtonNormalImage("facebookShareIcon.png")
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->setButtonPosition(Vec2(bg->getContentSize().width * 0.91f, bg->getContentSize().height * 0.5f))
        ->show(bg);
        
        CMyButton::create()
        ->addEventListener([=](Node* sender){
            CGameScene::getGameScene()->CreateAlertPopup()
            ->setPositiveButton([=](Node* sender){
                this->TWShare();
            }, TRANSLATE("BUTTON_YES"))
            ->setNegativeButton([=](Node* sender){
            }, TRANSLATE("BUTTON_NO"))
            ->setMessage("트윗 하시겠습니까?")
            ->show(CGameScene::getGameScene(), ZORDER::POPUP);
        })
        ->setButtonNormalImage("twitterShareIcon.png")
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->setButtonPosition(Vec2(bg->getContentSize().width * 0.75f, bg->getContentSize().height * 0.5f))
        ->show(bg);
    }
    
    // create number
    {
        auto number = Label::createWithTTF(StringUtils::format("%d", m_Number), FONT::MALGUNBD, 80);
        number->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        number->setPosition(Vec2(this->getContentSize().width * 0.08f,
                                 this->getContentSize().height * 0.5f));
        this->addChild(number);
    }
    
    // create picture
    {
        auto pic = CUrlSprite::create()
        ->setUrl(m_User->_url)
        ->setSize(Size(150.f, 150.f))
        ->build(this);
        pic->setPosition(Vec2(this->getContentSize().width * 0.25f,
                              this->getContentSize().height * 0.5f));
    }
    
    // create name
    {
        auto name = Label::createWithSystemFont(m_User->_name, FONT::MALGUNBD, 50);
        name->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        name->setPosition(Vec2(this->getContentSize().width * 0.38f,
                               this->getContentSize().height * 0.75f));
        this->addChild(name);
    }
    
    // create score
    {
        auto score = Label::createWithTTF(StringUtility::toCommaString(m_User->_score), FONT::MALGUNBD, 70);
        score->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        score->setPosition(Vec2(this->getContentSize().width * 0.38f,
                                this->getContentSize().height * 0.35f));
        this->addChild(score);
    }
    
    // create level
    if(m_IsMyData)
    {
        auto userLevel = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::LEVEL);
        auto level     = Label::createWithTTF(StringUtils::format("%d", userLevel), FONT::MALGUNBD, 35);
        level->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        level->setPosition(Vec2(this->getContentSize().width * 0.6f,
                                this->getContentSize().height * 0.5f));
        level->enableOutline(COLOR::WHITEGRAY_ALPHA, 1);
        this->addChild(level);
    }
    
    // create share button
    {
        
    }
    
    
//    auto btn = CMyButton::create()
//    ->addEventListener([=](Node* sender){
//        this->retain();
//        if(m_SelectDPListener)
//        {
//            m_SelectDPListener(m_DPIndex);
//        }
//        this->release();
//    })
//    ->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, 150.f, 150.f))
//    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
//    ->show(this);
//    btn->setPosition(this->getContentSize() / 2);
//    btn->setSwallowTouches(false);
    
    
    return true;
}

void CFacebookRankPopupDP::FBShare()
{
}

void CFacebookRankPopupDP::TWShare()
{
}

void CFacebookRankPopupDP::Notice()
{
}

void CFacebookRankPopupDP::Invite()
{
}