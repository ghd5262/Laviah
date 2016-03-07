#include "Bullet.h"
#include "Planet.h"
#include "../Task/PoolingManager.h"

CBullet::CBullet(
	std::string textureName,	    //bullet 이미지
	float boundingRadius,		    //bullet 충돌 범위
	float angle,				    //bullet 초기 각도 
	float speed,				    //bullet 초기 속도
	CMover* target)		    //bullet 타겟 위치
	: CMover(boundingRadius)
	, m_TextureName(textureName)
	, m_fAngle(angle)
	, m_fBulletSpeed(speed)
	, m_Target(target)
	, m_pTexture(nullptr)
{
	setPositionX((cos(CC_DEGREES_TO_RADIANS(angle)) * 700.f) + target->getPosition().x);
	setPositionY((sin(CC_DEGREES_TO_RADIANS(angle)) * 700.f) + target->getPosition().y);
	setRotation(-angle);
}

CBullet* CBullet::create(
	std::string textureName,		//bullet 이미지
	float boundingRadius,			//bullet 충돌 범위
	float angle,					//bullet 초기 각도 
	float speed,					//bullet 초기 속도
	CMover* target)			//bullet 타겟 위치
{
	CBullet* pRet = (CBullet*)new(std::nothrow)CBullet(textureName, boundingRadius, angle, speed, target);
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

bool CBullet::init()
{
	if (!initVariable())
		return false;
	return true;
}

// 이곳은 bullet을 오브젝트 풀에서 꺼낼때마다 호출하는 부분이니 addChild를 무작정해서는 안된다.
bool CBullet::initVariable()
{
	try{
		if (m_pTexture == nullptr){
			m_pTexture = Sprite::create(m_TextureName);
			m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
			addChild(m_pTexture);
		}else{
			m_pTexture->setTexture(m_TextureName);
		}
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTIONW__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void* CBullet::operator new(size_t size, const std::nothrow_t)
{
	// ObjectManager에서 메모리를 할당 받는다.
	return CPoolingManager::Instance()->BulletNew();
}

void CBullet::Execute(float delta)
{
	Vec2 dir = m_Target->getPosition() - getPosition();
	dir.normalize();
	dir *= (m_fBulletSpeed * delta);

	setPosition(getPosition() + dir);

	if (IsHit(m_Target))
	{
		ReturnToMemoryBlock();
	}
	//m_fAngle += m_fAngleRate;
	//m_fBulletSpeed += m_fSpeedRate​​;
	//if (abs(getPositionX()) >= MaxX + Shape->Size || abs(getPositionY()) >= MaxY + Shape->Size) {
	//	setAlive(false);
	//}
}