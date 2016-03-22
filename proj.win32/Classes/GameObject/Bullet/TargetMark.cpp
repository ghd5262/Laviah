#include "TargetMark.h"

CTargetMark::CTargetMark(
	std::string textureName,	    //bullet 이미지
	float boundingRadius,		    //bullet 충돌 범위
	float angle,				    //bullet 초기 각도 
	float speed,				    //bullet 초기 속도
	CBullet* owner,					//소유 bullet
	bool isAiming)					//조준 미사일 여부
	: CBullet(
	textureName, 
	boundingRadius,
	angle, 
	speed)
	, m_OwnerBullet(owner)
	, m_bIsAimingMissile(isAiming)
{}

CTargetMark* CTargetMark::create(
	std::string textureName,		//bullet 이미지
	float boundingRadius,			//bullet 충돌 범위
	float angle,					//bullet 초기 각도 
	float speed,					//bullet 초기 속도
	CBullet* owner,					//소유 bullet
	bool isAiming)					//조준 미사일 여부
{
	CTargetMark* pRet = 
		(CTargetMark*)new(std::nothrow)CTargetMark(
		textureName, boundingRadius, angle, speed, owner, isAiming);

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

bool CTargetMark::initVariable()
{
	try{
		AudioEngine::play2d("sounds/missile_warning_1.mp3", false, 0.7f);

		m_ScreenRect = Rect(0, 0, 720, 1280);
		setPositionX((cos(CC_DEGREES_TO_RADIANS(m_fAngle)) * (m_pPlanet->getBRadius() + 20)) + m_pPlanet->getPosition().x);
		setPositionY((sin(CC_DEGREES_TO_RADIANS(m_fAngle)) * (m_pPlanet->getBRadius() + 20)) + m_pPlanet->getPosition().y);
		setRotation(-m_fAngle);

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

void CTargetMark::Rotation(int dir)
{
	// aimingMissile일 경우 화면안에 들어왔을 때에만 회전한다.
	if (true == m_bIsAimingMissile){
		if (!m_ScreenRect.containsPoint(m_OwnerBullet->getPosition()))
		{
			return;
		}
	}

	CBullet::Rotation(dir);
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