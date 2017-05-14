#include "AchievementPopupDP.hpp"
#include "../MyButton.h"
#include "../UrlSprite.hpp"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/AchievementDataManager.hpp"
#include "../../Common/StringUtility.h"

CAchievementPopupDP* CAchievementPopupDP::create(const sACHIEVEMENT_PARAM* achievement, int level, int maxLevel)
{
    CAchievementPopupDP *pRet = new(std::nothrow) CAchievementPopupDP(achievement, level, maxLevel);
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
    
    // create title
    {
        auto name = Label::createWithSystemFont(TRANSLATE(m_Achievement->_title), FONT::MALGUNBD, 50);
        name->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        name->setPosition(Vec2(this->getContentSize().width * 0.25f,
                               this->getContentSize().height * 0.75f));
        this->addChild(name);
    }
    
    // create content
    {
        auto contents = StringUtils::format(TRANSLATE(m_Achievement->_contents).c_str(), m_Achievement->_materialValueList.at(0));
        auto content = Label::createWithSystemFont(contents, FONT::MALGUNBD, 35);
        content->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        content->setPosition(Vec2(this->getContentSize().width * 0.25f,
                                  this->getContentSize().height * 0.5f));
        this->addChild(content);
    }
    
    // create star
    {
        for(int count = 0; count < m_AchievementMaxLevel; count++)
        {
            auto star = Sprite::create("starIcon_s.png");
            star->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            star->setPosition(Vec2((this->getContentSize().width * 0.25f) + (count * 50),
                                    this->getContentSize().height * 0.25f));
            this->addChild(star);
            
            if(count < m_AchievementLevel)
                star->setColor(COLOR::GOLD);
            else
                star->setOpacity(255 * 0.4f);
        }
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