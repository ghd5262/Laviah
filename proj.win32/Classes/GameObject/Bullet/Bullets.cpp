#include "Bullets.h"
#include "../ObjectManager.h"
#include "../Planet.h"

CNormalBullet::CNormalBullet(
	std::string textureName,	    //bullet 이미지
	float boundingRadius,		    //bullet 충돌 범위
	float angle,				    //bullet 초기 각도 
	float speed,				    //bullet 초기 속도
	CGameObject* target)		    //bullet 타겟 위치
	: CBullet(textureName, boundingRadius, angle, speed, target)
{
	setPositionX((cos(CC_DEGREES_TO_RADIANS(angle)) * 1000.f) + target->getPosition().x);
	setPositionY((sin(CC_DEGREES_TO_RADIANS(angle)) * 1000.f) + target->getPosition().y);
	setRotation(-angle);
}

CNormalBullet* CNormalBullet::create(
	std::string textureName,		//bullet 이미지
	float boundingRadius,			//bullet 충돌 범위
	float angle,					//bullet 초기 각도 
	float speed,					//bullet 초기 속도
	CGameObject* target)			//bullet 타겟 위치
{
	CNormalBullet* pRet = (CNormalBullet*)new(std::nothrow)CNormalBullet(textureName, boundingRadius, angle, speed, target);
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

bool CNormalBullet::init()
{
	if (!initVariable())
		return false;
	return true;
}

// 이곳은 bullet을 오브젝트 풀에서 꺼낼때마다 호출하는 부분이니 addChild를 무작정해서는 안된다.
bool CNormalBullet::initVariable()
{
	try{
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

void CNormalBullet::Execute(float delta)
{
	Vec2 dir = m_Target->getPosition() - getPosition();
	dir.normalize();
	dir *= (m_fBulletSpeed * delta);

	setPosition(getPosition() + dir);

	if (IsHit(m_Target))
	{
		ReturnToMemoryBlock();
	}
}



CTargetMark::CTargetMark(
	std::string textureName,	    //bullet 이미지
	float boundingRadius,		    //bullet 충돌 범위
	float angle,				    //bullet 초기 각도 
	float speed,				    //bullet 초기 속도
	CGameObject* target,		    //bullet 타겟 위치
	CBullet* owner)					//소유 bullet
	: CBullet(textureName, boundingRadius, angle, speed, target)
	, m_OwnerBullet(owner)
{
	m_ScreenRect = Rect(0, 0, 720, 1280);
	setPositionX((cos(CC_DEGREES_TO_RADIANS(angle)) * (target->getBRadius() + 20)) + target->getPosition().x);
	setPositionY((sin(CC_DEGREES_TO_RADIANS(angle)) * (target->getBRadius() + 20)) + target->getPosition().y);
	setRotation(-angle);
}

CTargetMark* CTargetMark::create(
	std::string textureName,		//bullet 이미지
	float boundingRadius,			//bullet 충돌 범위
	float angle,					//bullet 초기 각도 
	float speed,					//bullet 초기 속도
	CGameObject* target,			//bullet 타겟 위치
	CBullet* owner)					//소유 bullet
{
	CTargetMark* pRet = (CTargetMark*)new(std::nothrow)CTargetMark(textureName, boundingRadius, angle, speed, target, owner);
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

bool CTargetMark::init()
{
	if (!initVariable())
		return false;
	return true;
}

// 이곳은 bullet을 오브젝트 풀에서 꺼낼때마다 호출하는 부분이니 addChild를 무작정해서는 안된다.
bool CTargetMark::initVariable()
{
	try{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(
			"sounds/missile_warning_1.wav");

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

void CTargetMark::Execute(float delta)
{
	if (!m_OwnerBullet->IsAlive())		// 이것 이외의 OwnerBullet을 사용하는 곳이 있으면 안된다.. 사실상 이 코드도 이미 메모리 블럭으로 되돌아간 bullet의 Alive이다.
		ReturnToMemoryBlock();			// OwnerBullet은 항상 Target보다 먼저 메모리 블럭으로 되돌아가기 때문이다.
	else{
		if (!m_ScreenRect.containsPoint(m_OwnerBullet->getPosition()))
			this->setVisible(true);
		else
			this->setVisible(false);
	}
}



CNormalMissile::CNormalMissile(
	std::string textureName,	    //bullet 이미지
	float boundingRadius,		    //bullet 충돌 범위
	float angle,				    //bullet 초기 각도 
	float speed,				    //bullet 초기 속도
	CGameObject* target,		    //bullet 타겟 위치
	bool isAiming/* = false*/)		//조준미사일인지 여부 true = CrushShake 호출					
	: CBullet(textureName, boundingRadius, angle, speed, target)
	, m_bIsAimingMissile(isAiming)
{
	setPositionX((cos(CC_DEGREES_TO_RADIANS(angle)) * 2500.f) + target->getPosition().x);
	setPositionY((sin(CC_DEGREES_TO_RADIANS(angle)) * 2500.f) + target->getPosition().y);
	setRotation(-angle);
}

CNormalMissile* CNormalMissile::create(
	std::string textureName,		//bullet 이미지
	float boundingRadius,			//bullet 충돌 범위
	float angle,					//bullet 초기 각도 
	float speed,					//bullet 초기 속도
	CGameObject* target,			//bullet 타겟 위치
	bool isAiming/* = false*/)		//조준미사일인지 여부 true = CrushShake 호출	
{
	CNormalMissile* pRet = (CNormalMissile*)new(std::nothrow)CNormalMissile(textureName, boundingRadius, angle, speed, target, isAiming);
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

bool CNormalMissile::init()
{
	if (!initVariable())
		return false;
	return true;
}

// 이곳은 bullet을 오브젝트 풀에서 꺼낼때마다 호출하는 부분이니 addChild를 무작정해서는 안된다.
bool CNormalMissile::initVariable()
{
	try{
		m_pTexture = Sprite::create(m_TextureName);
		m_pTexture->setAnchorPoint(Vec2(0.2f, 0.5f));
		addChild(m_pTexture);
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTIONW__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void CNormalMissile::Execute(float delta)
{
	Vec2 dir = m_Target->getPosition() - getPosition();
	dir.normalize();
	dir *= (m_fBulletSpeed * delta);

	setPosition(getPosition() + dir);

	if (IsHit(m_Target))
	{
		if (true == m_bIsAimingMissile){
			CObjectManager::Instance()->getM_Planet()->CrushShake();
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(
				"sounds/explosion_2.wav");
		}
		else {
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(
				"sounds/explosion_1.wav");
		}

		ReturnToMemoryBlock();
	}
}