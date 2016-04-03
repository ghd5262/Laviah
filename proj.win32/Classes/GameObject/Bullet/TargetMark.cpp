#include "TargetMark.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"

CTargetMark::CTargetMark(
	std::string textureName,		//TargetMark �̹���
	float angle,					//TargetMark �ʱ� ���� 
	Vec2 missilePos,				//Missile ���� ��ǥ
	float missileSpeed,				//Missile �ӷ�
	bool isAiming,					//���ع̻������� ����
	CBullet* owner/* = nullptr*/)	//owner missile (nullptr �� ������ �����ð����� �����Ѵ�.)
	: CBullet(
	textureName, 
	0.0f,
	angle, 
	0.0f)
	, m_bIsAimingMissile(isAiming)
	, m_fArriveTime(1.0f)
	, m_OwnerBullet(owner)
	
{
	float distance = m_pPlanet->getPosition().distance(missilePos);
	m_fArriveTime = (distance / missileSpeed);
}

CTargetMark* CTargetMark::create(
	std::string textureName,		//TargetMark �̹���
	float angle,					//TargetMark �ʱ� ���� 
	Vec2 missilePos,				//Missile ���� ��ǥ
	float missileSpeed,				//Missile �ӷ�
	bool isAiming,					//���ع̻������� ����
	CBullet* owner)					//owner missile
{
	CTargetMark* pRet = 
		(CTargetMark*)new(std::nothrow)CTargetMark(
		textureName, angle, missilePos, missileSpeed, isAiming, owner);

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

		auto texture = Director::getInstance()->getTextureCache()->addImage(m_TextureName);
		const int FrameCount_MAX = 3;
		SpriteFrame* frame[FrameCount_MAX];
		Vector<SpriteFrame*> animFrames(FrameCount_MAX);

		for (int frameCount = 0; frameCount < FrameCount_MAX; frameCount++)
		{
			frame[frameCount] = SpriteFrame::createWithTexture(texture,
				Rect(texture->getContentSize().width * 0, 
				(texture->getContentSize().height / FrameCount_MAX) * frameCount,
				texture->getContentSize().width,
				(texture->getContentSize().height / FrameCount_MAX)));
			animFrames.pushBack(frame[frameCount]);
		}
		auto sprite = Sprite::createWithSpriteFrame(frame[0]);
		sprite->setAnchorPoint(Vec2(0.04f, 0.5f));
		addChild(sprite);

		auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
		auto animate = Animate::create(animation);
		sprite->runAction(RepeatForever::create(animate));
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
	// m_OwnerBullet != nullptr �� ���� coin �̳� star�� ����Ǿ��� �� �̴�.
	if (m_OwnerBullet != nullptr){			
		if (!m_OwnerBullet->IsAlive())		// �̰� �̿��� OwnerBullet�� ����ϴ� ���� ������ �ȵȴ�.. ��ǻ� �� �ڵ嵵 �̹� �޸� ������ �ǵ��ư� bullet�� Alive�̴�.
			ReturnToMemoryBlock();			// OwnerBullet�� �׻� Target���� ���� �޸� ������ �ǵ��ư��� �����̴�.
	}
	else // coin �̳� star�� ����Ǿ��� ������ �ð����� �����Ѵ�.
	{
		m_fTime += delta;
		if (m_fTime > m_fArriveTime)
			ReturnToMemoryBlock();
	}
}
