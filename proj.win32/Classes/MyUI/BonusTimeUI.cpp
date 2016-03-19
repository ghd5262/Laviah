#include "BonusTimeUI.h"

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
	if (!initVariable())
		return false;
	return true;
}

bool CLetter::initVariable()
{
	try{
		this->setPosition(m_OriginPos);

		m_Texture = cocos2d::Sprite::create(m_NormalTextureName);
		if (m_Texture != nullptr)
		{
			m_Texture->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			this->addChild(m_Texture);
		}
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTIONW__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void CLetter::Execute(float delta)
{
	
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
	if (!initVariable())
		return false;
	return true;
}

bool CBonusTimeUI::initVariable()
{
	try{
		for (int letterNum = 0; letterNum < eLETTER_MAX; letterNum++){
			m_LetterList[letterNum] = CLetter::create(
				MakeString("grayLetter_%d.png", letterNum),
				MakeString("bonusLetter_%d.png", letterNum),
				static_cast<eLETTER>(letterNum),
				Vec2(static_cast<float>(letterNum)* 38.f, 0.f));

			this->addChild(m_LetterList[letterNum]);
		}
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTIONW__, __LINE__);
		assert(false);
		return false;
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
	}
}

void CBonusTimeUI::CollectLetter(eLETTER letter)
{
	if (letter < eLETTER_MAX)
		m_LetterList[letter]->setIsCollected(true);
}

eLETTER CBonusTimeUI::NonCollectedLetterNum() const
{
	int letterNum = 0;
	for (; letterNum < eLETTER_MAX; letterNum++)
	{
		if (!m_LetterList[letterNum]->getIsCollected())
			return static_cast<eLETTER>(letterNum);
	}
	return static_cast<eLETTER>(letterNum);
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
	m_fTime += delta;
	for (int letterNum = 0; letterNum < eLETTER_MAX; letterNum++){
		if (m_LetterList[letterNum]->getIsCollected())  
		{
			auto oldPosition = m_LetterList[letterNum]->getPosition();
			m_LetterList[letterNum]->setPosition(Vec2(oldPosition.x, sinf(m_fTime * 15 + letterNum * 1.5f) * 5));
		}
	}
}