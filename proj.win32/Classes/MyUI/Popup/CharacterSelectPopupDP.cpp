#include "CharacterSelectPopupDP.h"
#include "../MyButton.h"
#include "../../DataManager/UserDataManager.h"

CCharacterSelectPopupDP* CCharacterSelectPopupDP::create(const sCHARACTER_PARAM* character)
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
    if (!Widget::init()) return false;
    
    auto bg = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 150.f, 150.f);
    if (bg != nullptr){
        this->setContentSize(bg->getContentSize());
        
        bg->setIgnoreAnchorPointForPosition(false);
        bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        bg->setPosition(this->getContentSize() / 2);
        this->addChild(bg);
    }
    
    m_CharacterImg = Sprite::createWithSpriteFrameName(m_Character->_normalTextureName.c_str());
    if (m_CharacterImg != nullptr)
    {
        m_CharacterImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_CharacterImg->setPosition(this->getContentSize() / 2);
        bg->addChild(m_CharacterImg);
    }
    
    if (!CUserDataManager::Instance()->getUserData_IsItemHave(USERDATA_KEY::CHARACTER_LIST, m_Character->_idx)){
        if(m_Character->_random)
            m_CharacterImg->setColor(COLOR::DARKGRAY);
        else
            m_CharacterImg->setColor(Color3B::BLACK);
    }

    auto btn = CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->retain();
        if(m_SelectDPListener)
        {
            m_SelectDPListener(m_DPIndex);
        }
        this->release();
    })
    ->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, 150.f, 150.f))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(this);
    
    btn->setPosition(this->getContentSize() / 2);
    btn->setSwallowTouches(false);
    
    auto content = TRANSLATE("CURRENCY_UNIT");
    if(!m_Character->_random) content = TRANSLATE("CHARACTER_CANNOT_RANDOM");
    m_CostLabel = Label::createWithSystemFont(content, FONT::MALGUNBD, 23,
                                              Size(this->getContentSize().width * 2.5f,
                                                   this->getContentSize().height),
                                              TextHAlignment::CENTER,
                                              TextVAlignment::CENTER);
    m_CostLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_CostLabel->setPosition(Vec2(this->getContentSize().width * 0.5f,
                                  this->getContentSize().height * 1.5f));
    m_CostLabel->setVisible(false);
    this->addChild(m_CostLabel);
    
    return true;
}

void CCharacterSelectPopupDP::Buy()
{
    if (CUserDataManager::Instance()->CoinUpdate(-0)){
    
        // USER Data Save
        CUserDataManager::Instance()->setUserData_ItemGet(USERDATA_KEY::CHARACTER_LIST, m_Character->_idx);
        CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::CHARACTER, m_Character->_idx);

		// change color to white
		m_CharacterImg->setColor(Color3B::WHITE);
        m_CostLabel->setVisible(false);
    }
}

void CCharacterSelectPopupDP::Select()
{
    // USER Data Save
    CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::CHARACTER, m_Character->_idx);
}

void CCharacterSelectPopupDP::Center()
{
    m_CharacterImg->setScale(3.f);
    auto alreadyHave = CUserDataManager::Instance()->getUserData_IsItemHave(USERDATA_KEY::CHARACTER_LIST, m_Character->_idx);
    m_CostLabel->setVisible(!alreadyHave);
}

void CCharacterSelectPopupDP::DeSelect()
{
    m_CharacterImg->setScale(1.5f);
    m_CostLabel->setVisible(false);
}