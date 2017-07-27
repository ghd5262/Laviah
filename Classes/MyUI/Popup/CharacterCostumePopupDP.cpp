#include "CharacterCostumePopupDP.hpp"
#include "../MyButton.h"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/CostumeDataManager.hpp"

CCharacterCostumePopupDP* CCharacterCostumePopupDP::create(const COSTUME* costume)
{
    CCharacterCostumePopupDP *pRet = new(std::nothrow) CCharacterCostumePopupDP(costume);
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

bool CCharacterCostumePopupDP::init()
{
    if (!Widget::init()) return false;
    
    this->setContentSize(Size(1080, 1920));
    auto layerSize = this->getContentSize();
    
    auto layer     = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080, 1920);
    layer->setIgnoreAnchorPointForPosition(false);
    layer->setPosition(layerSize / 2);
    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(layer);
    
    
    m_CostumeImg = Sprite::createWithSpriteFrameName(m_Costume->_texture_600.c_str());
    m_CostumeImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_CostumeImg->setPosition(layerSize / 2);
    m_CostumeImg->setScale(1.5f);
    this->addChild(m_CostumeImg);
    
    if (!CUserDataManager::Instance()->getUserData_IsItemHave(USERDATA_KEY::COSTUME_LIST, m_Costume->_index)){
        m_CostumeImg->setColor(COLOR::DARKGRAY);
    }
    
    return true;
}

void CCharacterCostumePopupDP::Buy()
{
    if (CUserDataManager::Instance()->CoinUpdate(-0)){
        
        // USER Data Save
        CUserDataManager::Instance()->setUserData_ItemGet(USERDATA_KEY::CHARACTER_LIST, m_Costume->_index);
        CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::CHARACTER, m_Costume->_index);
        
        // change color to white
        m_CostumeImg->setColor(Color3B::WHITE);
    }
}
