#include "BonusLetter.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../../MyUI/BonusTimeUI.h"
#include "../../MyUI/UIManager.h"

CBonusLetter::CBonusLetter(
	sBULLET_PARAM bulletParam,
	float angle,				    //bullet 초기 각도 
	float speed)				    //bullet 초기 속도
	: CBullet(
	bulletParam,
	angle, 
	speed)
	, m_pUIBonusTime(nullptr)
{}

CBonusLetter* CBonusLetter::create(
	sBULLET_PARAM bulletParam,
	float angle,					//bullet 초기 각도 
	float speed)					//bullet 초기 속도
{
	CBonusLetter* pRet = 
		(CBonusLetter*)new(std::nothrow)CBonusLetter(
		bulletParam, angle, speed);

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
	if (!initVariable())
		return false;
	return true;
}

bool CBonusLetter::initVariable()
{
	setItemEffect(eITEM_FLAG_magnet);

	if (!m_BulletParam._isFly){
		m_BulletParam._fDistance = m_pPlanet->getBRadius() + 20;

		this->scheduleOnce([this](float delta)
		{
			this->R_FadeOutWithCount(5, 3.f);
		}, 5.f, MakeString("AutoRemove_%d", random<int>(1, 100)));
	}
	setPositionX((cos(CC_DEGREES_TO_RADIANS(m_fAngle)) *  m_BulletParam._fDistance) + m_pPlanet->getPosition().x);
	setPositionY((sin(CC_DEGREES_TO_RADIANS(m_fAngle)) *  m_BulletParam._fDistance) + m_pPlanet->getPosition().y);
	setRotation(-m_fAngle + 90);

	m_pUIBonusTime = static_cast<CBonusTimeUI*>(CUIManager::Instance()->FindUIWithName("BonusTime"));
	m_TargetPos = m_pUIBonusTime->NonCollectedLetterWorldPos();
	m_LetterNum = m_pUIBonusTime->NonCollectedLetterNum();

	m_Player = CObjectManager::Instance()->getPlayer();

	m_pTexture = Sprite::create(MakeString("bonusLetter_%d.png", static_cast<int>(m_LetterNum)));
	m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
	addChild(m_pTexture);

	return true;
}

void CBonusLetter::Execute(float delta)
{
	getFSM()->Execute(delta);
}

void CBonusLetter::CollisionWithPlanet()
{
	if (true == m_BulletParam._isFly)
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