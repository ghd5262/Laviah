#include "CharacterSelectPopupDP.h"
#include "../MyButton.h"
#include "../../DataManager/UserDataManager.h"

CCharacterSelectPopupDP* CCharacterSelectPopupDP::create(const sCHARACTER_PARAM character)
{
    CCharacterSelectPopupDP *pRet = new(std::nothrow) CCharacterSelectPopupDP(character);
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

bool CCharacterSelectPopupDP::init()
{
    if (!initVariable())
        return false;
    return true;
}

bool CCharacterSelectPopupDP::initVariable()
{
    try{
        m_DPBack = LayerColor::create(Color4B(0, 0, 0, 0), 150.f, 15.f);
        if (m_DPBack != nullptr){
            this->setContentSize(m_DPBack->getContentSize());
            
            m_DPBack->setIgnoreAnchorPointForPosition(false);
            m_DPBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_DPBack->setPosition(Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.5f));
            this->addChild(m_DPBack);
        }
        
        m_CharacterImg = Sprite::create(m_Character._normalTextureName.c_str());
        if(m_CharacterImg != nullptr)
        {
            m_CharacterImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_CharacterImg->setPosition(Vec2(m_DPBack->getContentSize().width * 0.5f, m_DPBack->getContentSize().height * 0.5f));
            m_DPBack->addChild(m_CharacterImg);
        }
    }
    catch (...){
        throw StringUtils::format("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
        return false;
    }
    return true;
}

void CCharacterSelectPopupDP::Buy()
{
    if (CUserDataManager::Instance()->CoinUpdate(-m_Character._cost)){
    
        // USER Data Save
        CUserDataManager::Instance()->setUserData_ItemGet("USER_CHARACTER_LIST", m_Character._idx);
        CUserDataManager::Instance()->setUserData_Number("USER_CUR_CHARACTER", m_Character._idx);
    }
}

void CCharacterSelectPopupDP::Select()
{
    // USER Data Save
    CUserDataManager::Instance()->setUserData_Number("USER_CUR_CHARACTER", m_Character._idx);
}

void CCharacterSelectPopupDP::Center()
{
    m_CharacterImg->setScale(3.5f);
}

void CCharacterSelectPopupDP::DeSelect()
{
    m_CharacterImg->setScale(1.f);
}