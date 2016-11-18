#include "BonusLetter.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../../MyUI/BonusTimeUI.h"
#include "../../MyUI/UIManager.h"

using namespace cocos2d;

CBonusLetter::CBonusLetter()
: m_pUIBonusTime(nullptr){}

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

    m_pUIBonusTime = static_cast<CBonusTimeUI*>(CUIManager::Instance()->FindUIWithName("BonusTime"));
    
    this->setItemEffect(eITEM_FLAG_magnet);
    
    m_TargetPos = m_pUIBonusTime->NonCollectedLetterWorldPos();
    m_LetterNum = m_pUIBonusTime->NonCollectedLetterNum();

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
	m_pUIBonusTime->CollectLetter(m_LetterNum);

	R_BezierWithScale(m_TargetPos, Vec2(100, 100), Vec2(80, 800), 1.0f, 4.0f);
}