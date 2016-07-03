#include "WorkshopPopupDP.h"
#include "../MyButton.h"
#include "../../DataManager/CharacterDataManager.h"

CWorkshopPopupDP* CWorkshopPopupDP::create(int CharacterIdx, const std::function<void(cocos2d::Ref*)> &func)
{
    CWorkshopPopupDP *pRet = new(std::nothrow) CWorkshopPopupDP(CharacterIdx, func);
    if (pRet && pRet->init())
    {
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool CWorkshopPopupDP::init()
{
    if (!initVariable())
        return false;
    return true;
}

bool CWorkshopPopupDP::initVariable()
{
    try{
        auto characterInfo = CCharacterDataManager::Instance()->getCharacterInfoByIndex(m_CharacterIdx);
        
        auto dpBack = LayerColor::create(Color4B(0, 0, 0, 0), 1080.f, 200.f);
        if (dpBack != nullptr){
            dpBack->ignoreAnchorPointForPosition(false);
            dpBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            dpBack->setPosition(Vec2::ZERO);
            this->addChild(dpBack);
        }
        
        auto dpItemBack = LayerColor::create(Color4B(0, 0, 0, 255 * 0.4f), 805.f, 200.f);
        if (dpItemBack != nullptr){
            dpItemBack->ignoreAnchorPointForPosition(false);
            dpItemBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            dpItemBack->setPosition(Vec2(0 + dpItemBack->getContentSize().width * 0.5f, 0));
            dpBack->addChild(dpItemBack);
        }
        
        auto dpBuyBtn = CMyButton::createWithLayerColor(Size(260, 200), Color4B(0, 0, 0, 255 * 0.8f), "Buy", 40, g_labelColor2, END, std::bind(&CWorkshopPopupDP::Buy, this), EFFECT_SIZEDOWN);
        if (dpBuyBtn != nullptr)
        {
            dpBuyBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            dpBuyBtn->setPosition(Vec2(dpBack->getContentSize().width - (dpBuyBtn->getContentSize().width * 0.5f), 0));
            dpBack->addChild(dpBuyBtn);
        }
        
        auto characterName = Label::createWithTTF(characterInfo._name.c_str(), "fonts/malgunbd.ttf", 40);
        if (characterName != nullptr)
        {
            characterName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            characterName->setPosition(Vec2(dpItemBack->getContentSize().width * 0.5f, dpItemBack->getContentSize().height * 0.5f));
            characterName->setColor(g_labelColor2);
            dpItemBack->addChild(characterName);
        }
        
        auto characterImg = Sprite::create(characterInfo._textureName);
        if(characterImg != nullptr)
        {
            characterImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            characterImg->setPosition(Vec2(0 + characterImg->getContentSize().width * 0.5f, dpItemBack->getContentSize().height * 0.5f));
            dpItemBack->addChild(characterImg);
        }
    }
    catch (...){
        throw StringUtils::format("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
        return false;
    }
    return true;
}

void CWorkshopPopupDP::Buy()
{
    m_SelectFunc(this);
}

void CWorkshopPopupDP::DeSelect()
{
    auto characterInfo = CCharacterDataManager::Instance()->getCharacterInfoByIndex(m_CharacterIdx);
    CCLOG("%s", characterInfo._name.c_str());
}