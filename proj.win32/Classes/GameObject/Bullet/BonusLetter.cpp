#include "BonusLetter.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../../MyUI/BonusTimeUI.h"
#include "../../MyUI/UIManager.h"

CBonusLetter::CBonusLetter(
	std::string textureName,	    //bullet 이미지
	float boundingRadius,		    //bullet 충돌 범위
	float angle,				    //bullet 초기 각도 
	float speed)				    //bullet 초기 속도
	: CBullet(textureName,
	boundingRadius, 
	angle, 
	speed)
	, m_pUIManager(nullptr)
	, m_bPlayerGet(false)
{}

CBonusLetter* CBonusLetter::create(
	std::string textureName,		//bullet 이미지
	float boundingRadius,			//bullet 충돌 범위
	float angle,					//bullet 초기 각도 
	float speed)					//bullet 초기 속도
{
	CBonusLetter* pRet = 
		(CBonusLetter*)new(std::nothrow)CBonusLetter(
		textureName, boundingRadius, angle, speed);

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

bool CBonusLetter::init()
{
	if (!initVariable())
		return false;
	return true;
}

bool CBonusLetter::initVariable()
{
	try{
		setItemEffect(eITEM_FLAG_magnet);

		setPositionX((cos(CC_DEGREES_TO_RADIANS(m_fAngle)) * 1000.f) + m_pPlanet->getPosition().x);
		setPositionY((sin(CC_DEGREES_TO_RADIANS(m_fAngle)) * 1000.f) + m_pPlanet->getPosition().y);
		setRotation(-m_fAngle + 90);

		m_pUIManager = static_cast<CBonusTimeUI*>(CUIManager::Instance()->FindUIWithName("BonusTime"));
		m_TargetPos = m_pUIManager->NonCollectedLetterWorldPos();
		m_LetterNum = m_pUIManager->NonCollectedLetterNum();

		m_Player = CObjectManager::Instance()->getM_Player();

		m_pTexture = Sprite::create(m_TextureName);
		m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
		addChild(m_pTexture);
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTIONW__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void CBonusLetter::Rotation(int dir)
{
	// aimingMissile일 경우 화면안에 들어왔을 때에만 회전한다.
	if (true == m_bPlayerGet){
		return;
	}

	CBullet::Rotation(dir);
}

void CBonusLetter::Execute(float delta)
{
	getFSM()->Execute(delta);
}

void CBonusLetter::CollisionWithPlanet()
{
	ReturnToMemoryBlock();
}

void CBonusLetter::CollisionWithPlayer()
{
	AudioEngine::play2d("sounds/Star_2.mp3", false);
	m_bPlayerGet = true;
	m_pUIManager->CollectLetter(m_LetterNum);

	R_BezierWithScale(m_TargetPos, Vec2(100, 100), Vec2(80, 800), 1.0f, 4.0f);
}