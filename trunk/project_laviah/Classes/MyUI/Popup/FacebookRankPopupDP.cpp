#include "FacebookRankPopupDP.hpp"
#include "../Popup.h"
#include "../MyButton.h"
#include "../UrlSprite.hpp"
#include "../../DataManager/UserDataManager.h"
#include "../../Common/StringUtility.h"
#include "../../Scene/GameScene.h"
#include "../../SDKBOX/SDKBoxHeaders.h"

USING_NS_CC;
using namespace cocos2d::ui;

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
    
    // create reward
    {
        if(m_Number <= 3){
            auto button = CMyButton::create()
            ->addEventListener([=](Node* sender){
                CGameScene::getGameScene()->CreateAlertPopup()
                ->setPositiveButton([=](Node* sender){}, TRANSLATE("BUTTON_OK"))
                ->setMessage(TRANSLATE("FACEBOOK_RANK_ALERT_POPUP"))
                ->show(CGameScene::getPopupLayer(), ZORDER::POPUP);

            })
            ->setButtonNormalImage(StringUtils::format("rankReward_%d.png", m_Number))
            ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
            ->setButtonPosition(Vec2(this->getContentSize().width * 0.83f,
                                     this->getContentSize().height * 0.5f))
            ->show(this);
        }
    }
    
    // create number
    {
        if(m_Number <= 3){
            auto number = Sprite::create(StringUtils::format("rankNumber_%d.png", m_Number));
            number->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            number->setPosition(Vec2(this->getContentSize().width,
                                     this->getContentSize().height));
            this->addChild(number);
        }
        else{
            auto number = Label::createWithTTF(StringUtils::format("%d", m_Number), FONT::MALGUNBD, 80);
            number->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            number->setPosition(Vec2(this->getContentSize().width * 0.95f,
                                     this->getContentSize().height * 0.55f));
            this->addChild(number);
        }
    }
    
    // create picture
    {
        auto pic = CUrlSprite::create()
        ->setUrl(m_User->_url, m_User->_url)
        ->setSaveToFileEnable(true)
        ->setSize(Size(200.f, 200.f))
        ->build(this);
        pic->setPosition(Vec2(this->getContentSize().width * 0.09f,
                              this->getContentSize().height * 0.5f));
    }
    
    // create name
    {
        auto name = Label::createWithSystemFont(m_User->_name, FONT::MALGUNBD, 50);
        name->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        name->setPosition(Vec2(this->getContentSize().width * 0.22f,
                               this->getContentSize().height * 0.75f));
        this->addChild(name);
    }
    
    // create score
    {
        auto score = Label::createWithTTF(StringUtility::toCommaString(m_User->_score), FONT::MALGUNBD, 70);
        score->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        score->setPosition(Vec2(this->getContentSize().width * 0.22f,
                                this->getContentSize().height * 0.35f));
        this->addChild(score);
    }
    
    // create level
//    if(m_IsMyData)
//    {
//        auto userLevel = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::LEVEL);
//        auto level     = Label::createWithTTF(StringUtils::format("%d", userLevel), FONT::MALGUNBD, 35);
//        level->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
//        level->setPosition(Vec2(this->getContentSize().width * 0.6f,
//                                this->getContentSize().height * 0.5f));
//        level->enableOutline(COLOR::WHITEGRAY_ALPHA, 1);
//        this->addChild(level);
//    }
    
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
