#pragma once
#include "Bullet.h"

class CGameObject;

class CTargetMark : public CBullet {
public:
	/*create를 호출하면 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
	static CTargetMark* create(
		sBULLET_PARAM bulletParam,
		float angle,				//TargetMark 초기 각도 
		cocos2d::Vec2 missilePos,			//Missile 현재 좌표
		float missileSpeed,			//Missile 속력
		bool isMissileChangedToCoin = true,
		CBullet* owner = nullptr);	//owner missile (nullptr 일 때에는 도착시간으로 삭제한다.)
	virtual void Execute(float delta = 0.f) override;
	virtual void Rotation(float dir, float delta) override;

protected:
	virtual bool init() override;

private:
	CTargetMark(
		sBULLET_PARAM bulletParam,
		float angle,
		cocos2d::Vec2 missilePos,
		float missileSpeed,
		bool isMissileChangedToCoin,
		CBullet* owner);

	virtual ~CTargetMark();

private:
	cocos2d::Vector<cocos2d::SpriteFrame*> m_AnimationFrames;
	cocos2d::Sprite* m_SpriteFrame;
	CBullet* m_OwnerBullet;			// owner missile
	float m_fArriveTime;			// 미사일 도착시간 =  target
	cocos2d::Rect m_ScreenRect;
	bool m_bIsMissileChangedToCoin;
	
	//파티클
	cocos2d::ParticleSystemQuad* m_pParticle;
};