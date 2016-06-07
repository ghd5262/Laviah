#include "PlayItem.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"

CPlayItem::CPlayItem(
	sBULLET_PARAM bulletParam,
	float angle,				    //item 초기 각도 
	float speed)				    //item 초기 속도)
	: CBullet(
	bulletParam,
	angle,
	speed)
{}

CPlayItem* CPlayItem::create(
	sBULLET_PARAM bulletParam,
	float angle,					//item 초기 각도 
	float speed)					//item 초기 속도

{
	CPlayItem* pRet = 
		(CPlayItem*)new(std::nothrow)CPlayItem(
		bulletParam, angle, speed);

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
//		setItemEffect(eITEM_FLAG_magnet);
//		Vec2 controlPoint_1;
//		Vec2 controlPoint_2;
//		Vec2 targetPos;
//
//		if (!m_BulletParam._isFly){
//			m_BulletParam._fDistance = m_pPlanet->getBRadius() + 20;
//
//			this->scheduleOnce([this, controlPoint_1, controlPoint_2, targetPos](float delta)
//			{
//				
//				float time = 1.0f;
//
//				// 베지어 곡선 생성
//				ccBezierConfig bezier;
//				bezier.controlPoint_1 = Vec2(controlPoint_1);
//				bezier.controlPoint_2 = Vec2(controlPoint_2);
//				bezier.endPosition = Vec2(targetPos);
//
//				// 베지어 액션 및 다른 액션 순서대로 실행
//				this->runAction(Sequence::create(BezierTo::create(time, bezier), CallFunc::create([this](){ m_BulletParam._isFly = true; }), nullptr));
//			}, 0.f, MakeString("AutoRemove_%d", random<int>(1, 100)));
//
//			/*this->scheduleOnce([this](float delta)
//			{
//				this->R_FadeOutWithCount(5, 3.f);
//			}, 5.f, MakeString("AutoRemove_%d", random<int>(1, 100)));*/
//		}
//		setPositionX((cos(CC_DEGREES_TO_RADIANS(m_fAngle)) *  m_BulletParam._fDistance) + m_pPlanet->getPosition().x);
//		setPositionY((sin(CC_DEGREES_TO_RADIANS(m_fAngle)) *  m_BulletParam._fDistance) + m_pPlanet->getPosition().y);
//		setRotation(-m_fAngle);
//
//		controlPoint_1 = getPosition() - m_pPlanet->getPosition();//Vec2((cos(CC_DEGREES_TO_RADIANS(m_fAngle)) *  m_pPlanet->getBRadius() + 100) + m_pPlanet->getPosition().x,
//			//(sin(CC_DEGREES_TO_RADIANS(m_fAngle)) *  m_pPlanet->getBRadius() + 100) + m_pPlanet->getPosition().y);
//		controlPoint_1 = controlPoint_1.getNormalized();
//
//		controlPoint_2 = getPosition() + (controlPoint_1 * 300);//Vec2((cos(CC_DEGREES_TO_RADIANS(m_fAngle)) *  m_pPlanet->getBRadius() + 130) + m_pPlanet->getPosition().x,
//			//(sin(CC_DEGREES_TO_RADIANS(m_fAngle)) *  m_pPlanet->getBRadius() + 130) + m_pPlanet->getPosition().y);
//
//		targetPos = getPosition() + (controlPoint_1 * 330);// Vec2((cos(CC_DEGREES_TO_RADIANS(m_fAngle)) *  m_pPlanet->getBRadius() + 130) + m_pPlanet->getPosition().x,
//			//(sin(CC_DEGREES_TO_RADIANS(m_fAngle)) *  m_pPlanet->getBRadius() + 130) + m_pPlanet->getPosition().y);
//
//
//		m_pTexture = Sprite::create(m_BulletParam._TextureName);
//		m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
//		addChild(m_pTexture);
        
        
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
        setRotation(-m_fAngle);
        
        m_pTexture = Sprite::create(m_BulletParam._TextureName);
        m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
        addChild(m_pTexture);
    }
    catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void CPlayItem::Execute(float delta)
{
	getFSM()->Execute(delta);
}

void CPlayItem::CollisionWithPlanet()
{
	if (true == m_BulletParam._isFly)
	{
		ReturnToMemoryBlock();
	}
}

void CPlayItem::CollisionWithPlayer()
{
	CAudioManager::Instance()->PlayEffectSound("sounds/Star_2.mp3", false);
	R_ScaleWithFadeOut(2.f, 0.5f, 0.5f);
	CItemManager::Instance()->StartItemTimer(m_BulletParam._itemType);
}
