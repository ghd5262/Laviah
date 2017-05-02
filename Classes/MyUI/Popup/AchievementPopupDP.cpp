#include "AchievementPopupDP.hpp"
#include "../MyButton.h"
#include "../UrlSprite.hpp"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/AchievementDataManager.hpp"
#include "../../Common/StringUtility.h"

CAchievementPopupDP* CAchievementPopupDP::create(const sACHIEVEMENT_PARAM* achievement)
{
    CAchievementPopupDP *pRet = new(std::nothrow) CAchievementPopupDP(achievement);
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

bool CAchievementPopupDP::init()
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
    
    // create picture
    {
//        auto pic = CUrlSprite::create()
//        ->setUrl(m_User->_url)
//        ->setSize(Size(200.f, 200.f))
//        ->build(this);
//        pic->setPosition(Vec2(this->getContentSize().width * 0.25f,
//                              this->getContentSize().height * 0.5f));
    }
    
    // create name
    {
        auto name = Label::createWithSystemFont(TRANSLATE(m_Achievement->_contents), FONT::MALGUNBD, 50);
        name->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        name->setPosition(Vec2(this->getContentSize().width * 0.38f,
                               this->getContentSize().height * 0.75f));
        this->addChild(name);
    }
    
    // create score
    {
        auto score = Label::createWithTTF("", FONT::MALGUNBD, 70);
        score->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        score->setPosition(Vec2(this->getContentSize().width * 0.38f,
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

void CAchievementPopupDP::Share()
{
}

void CAchievementPopupDP::Notice()
{
}

void CAchievementPopupDP::Invite()
{
}