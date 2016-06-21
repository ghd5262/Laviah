#include "CharacterSelectPopupDP.h"
#include "../MyButton.h"
#include "../../DataManager/CharacterDataManager.h"

CCharacterSelectPopupDP* CCharacterSelectPopupDP::create(int CharacterIdx, const std::function<void(cocos2d::Ref*)> &func)
{
	CCharacterSelectPopupDP *pRet = new(std::nothrow) CCharacterSelectPopupDP(CharacterIdx, func);
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

bool CCharacterSelectPopupDP::init()
{
    if (!initVariable())
        return false;
    return true;
}

bool CCharacterSelectPopupDP::initVariable()
{
    try{
        auto characterInfo = CCharacterDataManager::Instance()->getCharacterInfoByIndex(m_CharacterIdx);
        
        auto dpBack = LayerColor::create(Color4B(0, 0, 0, 0), 540.f, 915.f);
        if (dpBack != nullptr){
            dpBack->ignoreAnchorPointForPosition(false);
			dpBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			dpBack->setPosition(Vec2::ZERO);
			this->addChild(dpBack);
		}
        
        m_ItemBack = LayerColor::create(Color4B(0, 0, 0, 255 * 0.4f), 540.f, 750.f);
        if (m_ItemBack != nullptr){
            m_ItemBack->ignoreAnchorPointForPosition(false);
            m_ItemBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_ItemBack->setPosition(Vec2(dpBack->getContentSize().width * 0.5f, dpBack->getContentSize().height - m_ItemBack->getContentSize().height * 0.5f));
            dpBack->addChild(m_ItemBack);
        }
        
		auto dpSelectBtn = CMyButton::createWithString("empty150_b.png", "Select", 40, g_labelColor2, END, std::bind(&CCharacterSelectPopupDP::Select, this), EFFECT_GRAY);
		if (dpSelectBtn != nullptr)
		{
			dpSelectBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			dpSelectBtn->setPosition(Vec2(dpBack->getContentSize().width * 0.5f, 0 + (dpSelectBtn->getContentSize().height * 0.5f)));
			dpBack->addChild(dpSelectBtn);
		}

		auto characterName = Label::createWithTTF(characterInfo._name.c_str(), "fonts/malgunbd.ttf", 40);
		if (characterName != nullptr)
		{
			characterName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			characterName->setPosition(Vec2(m_ItemBack->getContentSize().width * 0.5f, m_ItemBack->getContentSize().height * 0.1f));
			characterName->setColor(g_labelColor2);
			m_ItemBack->addChild(characterName);
		}

        auto characterImg = Sprite::create(characterInfo._textureName);
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

void CCharacterSelectPopupDP::Select()
{
    m_ItemBack->setOpacity(255 * 0.8f);
	m_SelectFunc(this);
}

void CCharacterSelectPopupDP::DeSelect()
{
	auto characterInfo = CCharacterDataManager::Instance()->getCharacterInfoByIndex(m_CharacterIdx);
    m_ItemBack->setOpacity(255 * 0.4f);
	CCLOG("%s", characterInfo._name.c_str());
}