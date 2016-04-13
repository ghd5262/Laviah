#include "TargetMark.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../../Particle/Particles.h"

CTargetMark::CTargetMark(
	sBULLET_PARAM bulletParam,
	float angle,					//TargetMark �ʱ� ���� 
	Vec2 missilePos,				//Missile ���� ��ǥ
	float missileSpeed,				//Missile �ӷ�
	bool isMissileChangedToCoin/* = true*/,
	CBullet* owner/* = nullptr*/)	//owner missile (nullptr �� ������ �����ð����� �����Ѵ�.)
	: CBullet(
	bulletParam,
	angle, 
	0.0f)
	, m_fArriveTime(1.0f)
	, m_OwnerBullet(owner)
	, m_bIsMissileChangedToCoin(isMissileChangedToCoin)
	, m_pParticle(nullptr)
{
	float distance = m_pPlanet->getPosition().distance(missilePos);
	m_fArriveTime = (distance / missileSpeed);
}

CTargetMark* CTargetMark::create(
	sBULLET_PARAM bulletParam,
	float angle,					//TargetMark �ʱ� ���� 
	Vec2 missilePos,				//Missile ���� ��ǥ
	float missileSpeed,				//Missile �ӷ�
	bool isMissileChangedToCoin,
	CBullet* owner)					//owner missile
{
	CTargetMark* pRet = 
		(CTargetMark*)new(std::nothrow)CTargetMark(
		bulletParam, angle, missilePos, missileSpeed, isMissileChangedToCoin, owner);

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
		CAudioManager::Instance()->PlayEffectSound("sounds/missile_warning_1.mp3", false, 0.7f);

		m_ScreenRect = Rect(-1280, 0, 3840, 1280);
		setPositionX((cos(CC_DEGREES_TO_RADIANS(m_fAngle)) * (m_pPlanet->getBRadius() + 20)) + m_pPlanet->getPosition().x);
		setPositionY((sin(CC_DEGREES_TO_RADIANS(m_fAngle)) * (m_pPlanet->getBRadius() + 20)) + m_pPlanet->getPosition().y);
		setRotation(-m_fAngle);

		auto texture = Director::getInstance()->getTextureCache()->addImage(m_BulletParam._TextureName);
		
		

		if (m_bIsMissileChangedToCoin){

			m_pParticle = CParticle_Line::create(MakeString("particle_star%d.png", m_BulletParam._isAimingMissile + 1));
			if (m_pParticle != nullptr){
				m_pParticle->retain();
				m_pParticle->setAnchorPoint(Vec2::ZERO);
				m_pParticle->setPosition(Vec2(texture->getContentSize().width * 0.25f, 0));
				this->addChild(m_pParticle, 10);
			}
		}
		else
		{
			/* �� ������ �ڿ� ����
			 * m_bIsMissileChangedToCoin = true�� ���� 
			 * �Ϲ� �̻����� ������ �� ���� ����Ǵ� �Ŷ�
			 * targetmark�� ������ �Ϸ���Ŀ� �����ϱ� ���ؼ� */
			this->scheduleOnce([=](float delta){
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
			}, 0.05f, "targetMarkInit");
		}
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void CTargetMark::Rotation(float dir, float delta)
{
	// aimingMissile�� ��� ȭ��ȿ� ������ ������ ȸ���Ѵ�.
	if (true == m_BulletParam._isAimingMissile && !m_bIsMissileChangedToCoin){
		if (m_OwnerBullet->IsAlive() && !m_ScreenRect.containsPoint(m_OwnerBullet->getPosition()))
		{
			return;
		}
	}

	CBullet::Rotation(dir, delta);
}

void CTargetMark::Execute(float delta)
{
	// coin �̳� star�� ����Ǿ��� ���� m_OwnerBullet == nullptr�̴�.
	if ( true != m_bIsMissileChangedToCoin){
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
