#include "CharacterPopupDP.hpp"
#include "../MyButton.h"
#include "../../DataManager/UserDataManager.h"

CCharacterPopupDP* CCharacterPopupDP::create(const CHARACTER* character)
{
    CCharacterPopupDP *pRet = new(std::nothrow) CCharacterPopupDP(character);
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

bool CCharacterPopupDP::init()
{
    if (!Widget::init()) return false;
    
    this->setContentSize(Size(1080, 1920));
    auto layerSize = this->getContentSize();
    
    auto layer     = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080, 1920);
    layer->setIgnoreAnchorPointForPosition(false);
    layer->setPosition(layerSize / 2);
    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(layer);
    
    
    m_CharacterImg = Sprite::createWithSpriteFrameName(m_Character->_texture.c_str());
    m_CharacterImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_CharacterImg->setPosition(layerSize / 2);
    m_CharacterImg->setScale(4.f);
    this->addChild(m_CharacterImg);
    
    if (CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::LEVEL) < m_Character->_level){
        m_CharacterImg->setColor(COLOR::DARKGRAY);
    }
    
    return true;
}

void CCharacterPopupDP::Buy()
{
    if (CUserDataManager::Instance()->CoinUpdate(-0)){
        
        // USER Data Save
        CUserDataManager::Instance()->setUserData_ItemGet(USERDATA_KEY::CHARACTER_LIST, m_Character->_index);
        CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::CHARACTER, m_Character->_index);
        
        // change color to white
        m_CharacterImg->setColor(Color3B::WHITE);
    }
}
