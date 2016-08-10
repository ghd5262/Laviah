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
        m_DPBack = LayerColor::create(Color4B(0, 0, 0, 0), 540.f, 915.f);
        if (m_DPBack != nullptr){
            this->setContentSize(m_DPBack->getContentSize());
            
            m_DPBack->setIgnoreAnchorPointForPosition(false);
            m_DPBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_DPBack->setPosition(Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.5f));
            this->addChild(m_DPBack);
        }
        
        m_ItemBack = LayerColor::create(Color4B(0, 0, 0, 255 * 0.4f), 540.f, 750.f);
        if (m_ItemBack != nullptr){
            m_ItemBack->setIgnoreAnchorPointForPosition(false);
            m_ItemBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_ItemBack->setPosition(Vec2(m_DPBack->getContentSize().width * 0.5f, m_DPBack->getContentSize().height - m_ItemBack->getContentSize().height * 0.5f));
            m_DPBack->addChild(m_ItemBack);
            
            // 선택된 캐릭터일 경우
            if(CUserDataManager::Instance()->getUserData_Number("USER_CUR_CHARACTER") == m_Character._idx)
            {
                m_ItemBack->setOpacity(255 * 0.8f);
            }
        }
        
        if (CUserDataManager::Instance()->getUserData_IsItemHave("USER_CHARACTER_LIST", m_Character._idx)){
            auto dpSelectBtn = CMyButton::createWithLayerColor(Size(540, 150), Color4B(0, 0, 0, 255 * 0.8f), "Select", 40, g_labelColor2, END, std::bind(&CCharacterSelectPopupDP::Select, this), EFFECT_SIZEDOWN);
            if (dpSelectBtn != nullptr)
            {
                dpSelectBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                dpSelectBtn->setPosition(Vec2(m_DPBack->getContentSize().width * 0.5f, 0 + (dpSelectBtn->getContentSize().height * 0.5f)));
                m_DPBack->addChild(dpSelectBtn);
            }
        }
        else
        {
            auto dpBuyBtn = CMyButton::createWithLayerColor(Size(540, 150), Color4B(0, 0, 0, 255 * 0.8f), "Buy", 40, g_labelColor2, END, std::bind(&CCharacterSelectPopupDP::Buy, this), EFFECT_SIZEDOWN);
            if (dpBuyBtn != nullptr)
            {
                dpBuyBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                dpBuyBtn->setPosition(Vec2(m_DPBack->getContentSize().width * 0.5f, 0 + (dpBuyBtn->getContentSize().height * 0.5f)));
                m_DPBack->addChild(dpBuyBtn);
            }
        }
        
        auto characterName = Label::createWithTTF(m_Character._name.c_str(), "fonts/malgunbd.ttf", 40);
        if (characterName != nullptr)
        {
            characterName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            characterName->setPosition(Vec2(m_ItemBack->getContentSize().width * 0.5f, m_ItemBack->getContentSize().height * 0.1f));
            characterName->setColor(g_labelColor2);
            m_ItemBack->addChild(characterName);
        }
        
        auto characterImg = Sprite::create(m_Character._normalTextureName.c_str());
        if(characterImg != nullptr)
        {
            characterImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            characterImg->setPosition(Vec2(m_ItemBack->getContentSize().width * 0.5f, m_ItemBack->getContentSize().height * 0.5f));
            m_ItemBack->addChild(characterImg);
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
    m_ItemBack->setOpacity(255 * 0.8f);
    
    // USER Data Save
    CUserDataManager::Instance()->setUserData_ItemGet("USER_CHARACTER_LIST", m_Character._idx);
    CUserDataManager::Instance()->setUserData_Number("USER_CUR_CHARACTER", m_Character._idx);
}

void CCharacterSelectPopupDP::Select()
{
    m_ItemBack->setOpacity(255 * 0.8f);
    
    // USER Data Save
    CUserDataManager::Instance()->setUserData_Number("USER_CUR_CHARACTER", m_Character._idx);
}

void CCharacterSelectPopupDP::Center()
{
    m_ItemBack->setOpacity(255 * 0.8f);
}

void CCharacterSelectPopupDP::DeSelect()
{
    m_ItemBack->setOpacity(255 * 0.4f);
}