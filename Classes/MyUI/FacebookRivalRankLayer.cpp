#include "FacebookRivalRankLayer.hpp"
#include "MyButton.h"
#include "UrlSprite.hpp"
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
    
    auto userList   = CFacebookManager::Instance()->getFBUserList();
    
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
        
        auto createRivalsDP = [=](const FBUSER_PARAM* user, int sequence){
            auto dp = Widget::create();
            dp->setContentSize(dpSize);
            dp->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            
            // score
            auto score = Label::createWithTTF(StringUtility::toCommaString(user->_score), FONT::MALGUNBD, 50);
            score->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            score->setPosition(Vec2(dpSize.width * 0.96f, dpSize.height * 0.5f));
            dp->addChild(score);
            
            // picture
            auto picture = CUrlSprite::create()
            ->setUrl(user->_url)
            ->setSize(Size(58.f, 58.f))
            ->build(dp);
            picture->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            picture->setPosition(Vec2((dpSize.width * 0.96f) - (score->getContentSize().width + 40), dpSize.height * 0.5f));
            
            auto number = Label::createWithTTF(StringUtils::format("%d", sequence), FONT::MALGUNBD, 70);
            number->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            number->setPosition(Vec2(picture->getPosition().x - 40, dpSize.height * 0.5f));
            dp->addChild(number);
            
            return dp;
        };
        
        auto index = 1;
        // create dp
        for (auto user : userList)
            m_ListView->pushBackCustomItem(createRivalsDP(user.second, index++));
        
        
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
        auto nextTargetRank = currentRank - 1;
        if(currentRank >= 10)   return;
        if(nextTargetRank < 0) return;
        
        if(m_PrevRank != nextTargetRank)
        {
            m_PrevRank = nextTargetRank;
            m_ListView->scrollToItem(m_PrevRank, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
        }
    }
}
