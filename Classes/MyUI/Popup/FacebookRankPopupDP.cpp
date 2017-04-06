#include "FacebookRankPopupDP.hpp"
#include "../MyButton.h"
#include "../UrlSprite.hpp"
#include "../../DataManager/UserDataManager.h"
#include "../../Common/StringUtility.h"

CFacebookRankPopupDP* CFacebookRankPopupDP::create(const FBUSER_PARAM* user, int number)
{
    CFacebookRankPopupDP *pRet = new(std::nothrow) CFacebookRankPopupDP(user, number);
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
        CUrlSprite::create()
        ->setUrl(m_User->_url)
        ->setSize(Size(200.f, 200.f))
        ->build(this)
        ->setPosition(Vec2(this->getContentSize().width * 0.25f,
                           this->getContentSize().height * 0.5f));
    }
    
    // create name
    {
        auto name = Label::createWithSystemFont(m_User->_name, FONT::MALGUNBD, 50);
        name->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        name->setPosition(Vec2(this->getContentSize().width * 0.35f,
                               this->getContentSize().height * 0.75f));
        this->addChild(name);
    }
    
    // create score
    {
        auto score = Label::createWithTTF(StringUtility::toCommaString(m_User->_score), FONT::MALGUNBD, 70);
        score->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        score->setPosition(Vec2(this->getContentSize().width * 0.35f,
                                this->getContentSize().height * 0.35f));
        this->addChild(score);
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

void CFacebookRankPopupDP::Share()
{
}

void CFacebookRankPopupDP::Notice()
{
}

void CFacebookRankPopupDP::Invite()
{
}