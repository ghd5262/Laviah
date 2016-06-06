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
        
		auto dpBack = Sprite::create("empty750_b.png");
		if (dpBack != nullptr)
		{
			dpBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			dpBack->setPosition(Vec2::ZERO);
			dpBack->setOpacity(255 * 0.4f);
			this->addChild(dpBack);
		}
        
		auto dpSelectBtn = CMyButton::createWithString("empty150_b.png", "Select", 40, g_labelColor2, END, std::bind(&CCharacterSelectPopupDP::Select, this), EFFECT_GRAY);
		if (dpSelectBtn != nullptr)
		{
			dpSelectBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			dpSelectBtn->setPosition(Vec2(dpBack->getContentSize().width * 0.5f, 0 + (dpSelectBtn->getContentSize().height * 0.5f)));
			dpBack->addChild(dpSelectBtn);
		}

		auto characterName = Label::create(characterInfo._name.c_str(), "fonts/malgunbd.ttf", 40);
		if (characterName != nullptr)
		{
			characterName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			characterName->setPosition(Vec2(dpBack->getContentSize().width * 0.5f, dpBack->getContentSize().height * 0.9f));
			characterName->setColor(g_labelColor2);
			dpBack->addChild(characterName);
		}

        auto characterImg = Sprite::create(characterInfo._textureName);
        if(characterImg != nullptr)
        {
			characterImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			characterImg->setPosition(Vec2(dpBack->getContentSize().width * 0.5f, dpBack->getContentSize().height * 0.5f));
			dpBack->addChild(characterImg);
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
	m_SelectFunc(this);
}

void CCharacterSelectPopupDP::DeSelect()
{
	auto characterInfo = CCharacterDataManager::Instance()->getCharacterInfoByIndex(m_CharacterIdx);
	CCLOG("%s", characterInfo._name.c_str());
}