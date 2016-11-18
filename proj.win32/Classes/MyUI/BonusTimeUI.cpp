#include "BonusTimeUI.h"
#include "../GameObject/ItemManager.h"

using namespace cocos2d;

CLetter* CLetter::create(
	std::string normalTextureName,
	std::string bonusTextureName,
	eLETTER letterNum,
	Vec2 position)
{
	CLetter *pRet = new(std::nothrow) CLetter(normalTextureName, bonusTextureName, letterNum, position);
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

bool CLetter::init()
{
    if (!Node::init()) return false;

    this->setPosition(m_OriginPos);
    
    m_Texture = cocos2d::Sprite::create(m_NormalTextureName);
    if (m_Texture != nullptr)
    {
        m_Texture->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        this->addChild(m_Texture);
    }
    
    return true;
}

CBonusTimeUI* CBonusTimeUI::create()
{
	CBonusTimeUI *pRet = new(std::nothrow) CBonusTimeUI();
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

bool CBonusTimeUI::init()
{
    if (!Node::init()) return false;
    
    for (int letterNum = 0; letterNum < eLETTER_MAX; letterNum++){
        m_LetterList[letterNum]
        = CLetter::create(MakeString("grayLetter_%d.png", letterNum),
                          MakeString("bonusLetter_%d.png", letterNum),
                          static_cast<eLETTER>(letterNum),
                          Vec2(static_cast<float>(letterNum)* 29.f, 0.f));
        
        this->addChild(m_LetterList[letterNum]);
    }
    
    return true;
}

void CBonusTimeUI::LetsBonusTime()
{
	for (int letterNum = 0; letterNum < eLETTER_MAX; letterNum++)
	{
		CollectLetter(static_cast<eLETTER>(letterNum));
	}
}

void CBonusTimeUI::BonusTimeIsFinish() 
{
	for (int letterNum = 0; letterNum < eLETTER_MAX; letterNum++)
	{
		m_LetterList[letterNum]->setIsCollected(false);
		m_LetterList[letterNum]->setPositionY(0.f);
	}
}

void CBonusTimeUI::CollectLetter(eLETTER letter)
{
	if (letter < eLETTER_MAX)
		m_LetterList[letter]->setIsCollected(true);

	if (letter == eLETTER_MAX - 1){
		CItemManager::Instance()->StartItemTimer(eITEM_TYPE_bonustime);
	}
}

eLETTER CBonusTimeUI::NonCollectedLetterNum() const
{
	int letterNum = 0;
	for (; letterNum < eLETTER_MAX; letterNum++)
	{
		if (!m_LetterList[letterNum]->getIsCollected())
			return static_cast<eLETTER>(letterNum);
	}
	return static_cast<eLETTER>(eLETTER_B);
}

Vec2 CBonusTimeUI::NonCollectedLetterWorldPos() const
{
	Vec2 worldPos(0, 0);
	for (int letterNum = 0; letterNum < eLETTER_MAX; letterNum++)
	{
		if (!m_LetterList[letterNum]->getIsCollected())
			return m_LetterList[letterNum]->getLetterWorldPosition();
	}
	return worldPos;
}

void CBonusTimeUI::Execute(float delta)
{
	m_Time += delta;
	for (int letterNum = 0; letterNum < eLETTER_MAX; letterNum++){
		if (m_LetterList[letterNum]->getIsCollected())  
		{
			auto oldPosition = m_LetterList[letterNum]->getPosition();
			m_LetterList[letterNum]->setPosition(Vec2(oldPosition.x, sinf(m_Time * 15 + letterNum * 1.5f) * 5));
		}
	}
}