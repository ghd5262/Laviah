#include "PlanetSelectPopupDP.hpp"
#include "../MyButton.h"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/PlanetDataManager.hpp"

CPlanetSelectPopupDP* CPlanetSelectPopupDP::create(const PLANET* planet)
{
    CPlanetSelectPopupDP *pRet = new(std::nothrow) CPlanetSelectPopupDP(planet);
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

bool CPlanetSelectPopupDP::init()
{
    if (!Widget::init()) return false;
    
    this->setContentSize(Size(1080, 1920));
    auto layerSize = this->getContentSize();
    
    auto layer     = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080, 1920);
    layer->setIgnoreAnchorPointForPosition(false);
    layer->setPosition(layerSize / 2);
    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(layer);
    
    
    m_PlanetImg = Sprite::createWithSpriteFrameName(m_Planet->_planetTexture);
    m_PlanetImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_PlanetImg->setPosition(Vec2(layerSize.width * 0.5f, layerSize.height * 0.57f));
    m_PlanetImg->setScale(0.7f);
    this->addChild(m_PlanetImg);
    
    if (!CPlanetDataManager::Instance()->IsPlanetOpened(m_Planet->_index)){
        m_PlanetImg->setColor(Color3B::BLACK);
    }
    
    return true;
}
