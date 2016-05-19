#include "TargetMark.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../../Particle/Particles.h"

CTargetMark::CTargetMark(
	sBULLET_PARAM bulletParam,
	float angle,					//TargetMark 초기 각도 
	Vec2 missilePos,				//Missile 현재 좌표
	float missileSpeed,				//Missile 속력
	bool isMissileChangedToCoin/* = true*/,
	CBullet* owner/* = nullptr*/)	//owner missile (nullptr 일 때에는 도착시간으로 삭제한다.)
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
	float angle,					//TargetMark 초기 각도 
	Vec2 missilePos,				//Missile 현재 좌표
	float missileSpeed,				//Missile 속력
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
        Size visibleSize = Director::getInstance()->getVisibleSize();
        m_ScreenRect = Rect(-visibleSize.width, 0, visibleSize.width * 3, visibleSize.height);
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
            CAudioManager::Instance()->PlayEffectSound("sounds/missile_warning_1.mp3", false, 0.7f);
            
			/* ∏Ó «¡∑π¿” µ⁄ø° ª˝º∫
			 * m_bIsMissileChangedToCoin = true¿œ ∂ß¥¬ 
			 * ¿œπ› πÃªÁ¿œ¿Ã ª˝º∫µ» »ƒ ∫∞∑Œ ∫Ø∞Êµ«¥¬ ∞≈∂Û
			 * targetmark∏¶ ∫Ø∞Ê¿Ã øœ∑·µ»»ƒø° ª˝º∫«œ±‚ ¿ß«ÿº≠ */
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
	// aimingMissile일 경우 화면안에 들어왔을 때에만 회전한다.
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
	// coin 이나 star로 변경되었을 때는 m_OwnerBullet == nullptr이다.
	if ( true != m_bIsMissileChangedToCoin){
		if (!m_OwnerBullet->IsAlive())		// 이것 이외의 OwnerBullet을 사용하는 곳이 있으면 안된다.. 사실상 이 코드도 이미 메모리 블럭으로 되돌아간 bullet의 Alive이다.
			ReturnToMemoryBlock();			// OwnerBullet은 항상 Target보다 먼저 메모리 블럭으로 되돌아가기 때문이다.
	}
	else // coin 이나 star로 변경되었을 때에는 시간으로 삭제한다.
	{
		m_fTime += delta;
		if (m_fTime > m_fArriveTime)
			ReturnToMemoryBlock();
	}
}
