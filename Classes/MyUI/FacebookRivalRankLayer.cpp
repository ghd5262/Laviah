#include "FacebookRivalRankLayer.hpp"
#include "MyButton.h"
#include "UrlSprite.hpp"
#include "ScoreUI.h"
#include "../Scene/GameScene.h"
#include "../DataManager/UserDataManager.h"
#include "../GameObject/ObjectManager.h"
#include "../Facebook/FacebookManager.hpp"
#include "../Common/StringUtility.h"

using namespace cocos2d;
using namespace cocos2d::ui;

CFacebookRivalRankLayer* CFacebookRivalRankLayer::create()
{
    CFacebookRivalRankLayer *pRet = new(std::nothrow) CFacebookRivalRankLayer();
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

bool CFacebookRivalRankLayer::init()
{
    if (!CPopup::init()) return false;
    
    this->setContentSize(Size(1080.f, 60.f));
    this->InitListView();
    this->scheduleUpdate();
    
    return true;
}

void CFacebookRivalRankLayer::InitListView()
{
    if(m_ListView) {
        m_ListView->removeFromParent();
        m_ListView = nullptr;
    }
    
    if(!sdkbox::PluginFacebook::isLoggedIn()) return;
    
    auto userList   = CFacebookManager::Instance()->getFBUserList();
    auto myData     = CFacebookManager::Instance()->getMyFacebookData();
    
    // Cut to the top 10.
    if(userList.size() > 10)
        userList.resize(10);
    
    Size layerSize  = this->getContentSize();
    Size dpSize     = Size(1080.f, 58.f);
    size_t dpDistance = 2;
    float spawnCount = 1;
    
    // Create the list view
    m_ListView = ListView::create();
    if (m_ListView != nullptr){
        m_ListView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
        m_ListView->setBackGroundImageScale9Enabled(true);
        m_ListView->setContentSize(Size(dpSize.width, (dpSize.height + dpDistance) * spawnCount));
        m_ListView->setItemsMargin(dpDistance);
        m_ListView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_ListView->setPosition(layerSize / 2);
        m_ListView->setMagneticType(ListView::MagneticType::BOTH_END);
        m_ListView->setScrollBarEnabled(false);
        m_ListView->setTouchEnabled(false);
        this->addChild(m_ListView);
        
        // My score will be recorded after I pass the first record.
        m_ListView->pushBackCustomItem(createRankDP(GLOBAL->STAR_SCORE, myData->_url, 1));
        
        auto rank = 1;
        // create dp
        for (auto user : userList){
            auto data = const_cast<FBUSER_PARAM*>(user.second);
            m_ListView->pushBackCustomItem(createRankDP(data->_score, data->_url, rank++));
        }
        
        m_PrevRank = (int)m_ListView->getChildrenCount();
        
        // Scrolling to bottom
        this->scheduleOnce([=](float delta){
            m_ListView->jumpToBottom();
        }, 0.f, "ScrollToItem");
    }
}

void CFacebookRivalRankLayer::update(float delta)
{
    if(CObjectManager::Instance()->getIsGamePause()) return;
    
    if(m_PrevScore != GLOBAL->STAR_SCORE)
    {
        m_PrevScore = GLOBAL->STAR_SCORE;
        auto currentRank = CFacebookManager::Instance()->getRankByScore(m_PrevScore);
        if(currentRank >= 10) return;
        if(m_PrevRank != currentRank)
        {
            m_PrevRank = currentRank;
            m_ListView->scrollToItem(m_PrevRank, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
        }
    }
}

cocos2d::ui::Widget* CFacebookRivalRankLayer::createRankDP(int &scoreRef, std::string url, int rank)
{
    auto dp = Widget::create();
    dp->setContentSize(this->getContentSize());
    dp->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    auto score = CScoreUI::create(scoreRef)
    ->setFont(FONT::MALGUNBD, 45)
    ->setScoreAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT)
    ->show(dp);
    
    score->setPosition(Vec2(this->getContentSize().width * 0.96f,
                            this->getContentSize().height * 0.5f));
    
    auto pic = CUrlSprite::create()
    ->setUrl(url)
    ->setSize(Size(55.f, 55.f))
    ->build(score);
    
    score->setIcon(pic);
    
    auto number = Label::createWithTTF(StringUtils::format("%d", rank), FONT::MALGUNBD, 55);
    number->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    number->setPosition(Vec2(-15, 55 * 0.5f));
    pic->addChild(number);
    
    return dp;
}
