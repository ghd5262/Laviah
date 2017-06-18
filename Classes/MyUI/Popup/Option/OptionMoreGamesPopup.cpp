#include "OptionMoreGamesPopup.hpp"
#include "../../MyButton.h"
#include "../../UrlSprite.hpp"
#include "../../../Scene/GameScene.h"
#include "../../../DataManager/MoreGamesDataManager.hpp"
#include "../../../DataManager/NetworkManager.hpp"

using namespace cocos2d;
using namespace cocos2d::ui;

COptionMoreGamesPopup* COptionMoreGamesPopup::create()
{
    COptionMoreGamesPopup *pRet = new(std::nothrow) COptionMoreGamesPopup();
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

bool COptionMoreGamesPopup::init()
{
    if (!CPopup::init()) return false;
    
    this->setContentSize(Size(1080.f, 1500.f));
    auto layerSize = getContentSize();
    auto gamesScroll = createListView(Size(layerSize.width * 0.9f, layerSize.height * 0.5f), 5,
                                      this->getContentSize() / 2);
    this->addChild(gamesScroll);
    
    auto gameList = CMoreGamesDataManager::Instance()->getMoreGamesList();
    for(auto game : gameList)
    {
        auto layer = Widget::create();
        layer->setContentSize(Size(layerSize.width * 0.9f, layerSize.height * 0.15f));
        layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        gamesScroll->pushBackCustomItem(layer);
        
        auto btn = this->createGameDP(game.second->_url);
        btn->setSwallowTouches(false);
        btn->setButtonPosition(Vec2((layer->getContentSize().width * 0.5f) - 7.f,
                                    layer->getContentSize().height * 0.5f));
        btn->show(layer);
        
        CUrlSprite::create()
        ->setUrl(NETWORK_DEFINE::URL_DOWNLOAD + game.second->_textureName, game.second->_textureName)
        ->setSize(btn->getContentSize())
        ->build(btn)
        ->setPosition(btn->getContentSize() / 2);
    }
    
    return true;
}

CMyButton* COptionMoreGamesPopup::createGameDP(std::string url)
{
    return CMyButton::create()
    ->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA,
                                  this->getContentSize().width * 0.9f,
                                  this->getContentSize().height * 0.15f))
    ->addEventListener([=](Node* sender){
        Application::getInstance()->openURL(url);
    })
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE);
}

cocos2d::ui::ListView* COptionMoreGamesPopup::createListView(Size size, size_t distance, Vec2 pos)
{
    auto listView = ListView::create();
    listView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(true);
    listView->setBackGroundImageScale9Enabled(true);
    listView->setScrollBarPositionFromCorner(Vec2(7, 7));
    listView->setContentSize(size);
    listView->setItemsMargin(distance);
    listView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    listView->setPosition(pos);
    listView->setMagneticType(ListView::MagneticType::BOTH_END);
    return listView;
}
