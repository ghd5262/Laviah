#include "TargetMark.h"

CTargetMark::CTargetMark(
	std::string textureName,	    //bullet �̹���
	float boundingRadius,		    //bullet �浹 ����
	float angle,				    //bullet �ʱ� ���� 
	float speed,				    //bullet �ʱ� �ӵ�
	CBullet* owner,					//���� bullet
	bool isAiming)					//���� �̻��� ����
	: CBullet(
	textureName, 
	boundingRadius,
	angle, 
	speed)
	, m_OwnerBullet(owner)
	, m_bIsAimingMissile(isAiming)
{}

CTargetMark* CTargetMark::create(
	std::string textureName,		//bullet �̹���
	float boundingRadius,			//bullet �浹 ����
	float angle,					//bullet �ʱ� ���� 
	float speed,					//bullet �ʱ� �ӵ�
	CBullet* owner,					//���� bullet
	bool isAiming)					//���� �̻��� ����
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
	// aimingMissile�� ��� ȭ��ȿ� ������ ������ ȸ���Ѵ�.
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
	if (!m_OwnerBullet->IsAlive())		// �̰� �̿��� OwnerBullet�� ����ϴ� ���� ������ �ȵȴ�.. ��ǻ� �� �ڵ嵵 �̹� �޸� ������ �ǵ��ư� bullet�� Alive�̴�.
		ReturnToMemoryBlock();			// OwnerBullet�� �׻� Target���� ���� �޸� ������ �ǵ��ư��� �����̴�.
	else{
		if (!m_ScreenRect.containsPoint(m_OwnerBullet->getPosition()))
			this->setVisible(true);
		else
			this->setVisible(false);
	}
}