#include "BonusLetter.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../../MyUI/BonusTimeUI.h"

using namespace cocos2d;

CBonusLetter::CBonusLetter(){}

CBonusLetter* CBonusLetter::create()
{
	CBonusLetter* pRet = (CBonusLetter*)new(std::nothrow)CBonusLetter();

	if (pRet && pRet->init())
	{
#if(!USE_MEMORY_POOLING)
        pRet->autorelease();
#endif
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool CBonusLetter::init()
{
    if (!CBullet::init()) return false;
    
    this->setItemEffect(eITEM_FLAG_magnet);
    
    m_TargetPos = CBonusTimeUI::Instance()->NonCollectedLetterWorldPos();
    m_LetterNum = CBonusTimeUI::Instance()->NonCollectedLetterNum();

    return true;
}

void CBonusLetter::CollisionWithPlanet()
{
	if (this->getIsFly())
	{
		ReturnToMemoryBlock();
	}
}

void CBonusLetter::CollisionWithPlayer()
{
	CAudioManager::Instance()->PlayEffectSound("sounds/Star_2.mp3", false);
    CBonusTimeUI::Instance()->CollectLetter(m_LetterNum);

	R_BezierWithScale(m_TargetPos, Vec2(100, 100), Vec2(80, 800), 1.0f, 4.0f);
}