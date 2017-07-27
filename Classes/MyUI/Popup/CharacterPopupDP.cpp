#include "CharacterPopupDP.hpp"
#include "../MyButton.h"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/CostumeDataManager.hpp"

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
    
    
    m_CharacterImg = Sprite::createWithSpriteFrameName(m_Character->_texture_600.c_str());
    m_CharacterImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_CharacterImg->setPosition(layerSize / 2);
    m_CharacterImg->setScale(1.5f);
    this->addChild(m_CharacterImg);
    
    auto data = CCostumeDataManager::Instance()->getCurCostumeByCharacter(m_Character->_index);
    m_CostumeOriginIndex = data->_index;
    m_CostumeImg = Sprite::createWithSpriteFrameName(data->_texture_600.c_str());
    m_CostumeImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_CostumeImg->setPosition(layerSize / 2);
    m_CostumeImg->setScale(1.5f);
    this->addChild(m_CostumeImg);
    
    if (CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::LEVEL) < m_Character->_level){
        m_CharacterImg->setColor(COLOR::DARKGRAY);
    }
    
    return true;
}

void CCharacterPopupDP::ChangeCostume()
{
    if(!m_CostumeImg) return;
    
    m_CostumeImg->setVisible(true);
    auto data = CCostumeDataManager::Instance()->getCurCostumeByCharacter(m_Character->_index);
    if(m_CostumeOriginIndex == data->_index) return;
    
    m_CostumeOriginIndex = data->_index;
    m_CostumeImg->setSpriteFrame(data->_texture_600);
}

void CCharacterPopupDP::CostumeOff()
{
    if(!m_CostumeImg) return;
    m_CostumeImg->setVisible(false);
}
