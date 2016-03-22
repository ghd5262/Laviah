#include "PlayItem.h"
#include "../../AI/States/ObjectStates.h"

CPlayItem::CPlayItem(
	std::string textureName,	    //item 이미지
	float boundingRadius,		    //item 충돌 범위
	float angle,				    //item 초기 각도 
	float speed,				    //item 초기 속도
	int itemType,					//item 타입
	bool isFly)						//fly item 인지 ground item 인지
	: CBullet(
	textureName,
	boundingRadius,
	angle,
	speed)
	, m_ItemType(static_cast<eITEM_FLAG>(1 << itemType))	// eITEM_TYPE에서 eITEM_FLAG 로 변환
	, m_bIsFlyItem(isFly)
	, m_bPlayerGet(false)
{}

CPlayItem* CPlayItem::create(
	std::string textureName,		//item 이미지
	float boundingRadius,			//item 충돌 범위
	float angle,					//item 초기 각도 
	float speed,					//item 초기 속도
	int itemType,					//item 타입
	bool isFly)						//fly item 인지 ground item 인지
{
	CPlayItem* pRet = 
		(CPlayItem*)new(std::nothrow)CPlayItem(
		textureName, boundingRadius, angle, speed, itemType, isFly);

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

bool CPlayItem::init()
{
	if (!initVariable())
		return false;
	return true;
}

bool CPlayItem::initVariable()
{
	try{
		setItemEffect(eITEM_FLAG_magnet);

		if (m_bIsFlyItem){
			setPositionX((cos(CC_DEGREES_TO_RADIANS(m_fAngle)) * 1000.f) + m_pPlanet->getPosition().x);
			setPositionY((sin(CC_DEGREES_TO_RADIANS(m_fAngle)) * 1000.f) + m_pPlanet->getPosition().y);
			setRotation(-m_fAngle);
		}
		else
		{
			setPositionX((cos(CC_DEGREES_TO_RADIANS(m_fAngle)) * (m_pPlanet->getBRadius() + 20)) + m_pPlanet->getPosition().x);
			setPositionY((sin(CC_DEGREES_TO_RADIANS(m_fAngle)) * (m_pPlanet->getBRadius() + 20)) + m_pPlanet->getPosition().y);
			setRotation(-m_fAngle);
		}

		m_pTexture = Sprite::create(m_TextureName);
		m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
		addChild(m_pTexture);

		m_Player = CObjectManager::Instance()->getM_Player();
		m_TargetPos = m_Player->getPosition();
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTIONW__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void CPlayItem::Rotation(int dir)
{
	// aimingMissile일 경우 화면안에 들어왔을 때에만 회전한다.
	if (true == m_bPlayerGet){
		return;
	}

	CBullet::Rotation(dir);
}

void CPlayItem::Execute(float delta)
{
	getFSM()->Execute(delta);
}

void CPlayItem::CollisionWithPlanet()
{
	if (true == m_bIsFlyItem && IsHit(m_pPlanet))
	{
		ReturnToMemoryBlock();
	}
}

void CPlayItem::CollisionWithPlayer()
{
	AudioEngine::play2d("sounds/Star_2.mp3", false);
	setAlive(false);
	m_bPlayerGet = true;
	BezierWithScale(
		m_TargetPos,
		Vec2(m_TargetPos.x - 20, m_TargetPos.y + 25),
		Vec2(m_TargetPos.x + 20, m_TargetPos.y + 25),
		0.5f,
		2.0f);
	
	CObjectManager::Instance()->PlayerGetItem(m_ItemType);
}
