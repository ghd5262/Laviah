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
		Vec2 missilePos,			//Missile 현재 좌표
		float missileSpeed,			//Missile 속력
		bool isMissileChangedToCoin = true,
		CBullet* owner = nullptr);	//owner missile (nullptr 일 때에는 도착시간으로 삭제한다.)
	virtual void Execute(float delta = 0.f) override;
	virtual void Rotation(int dir) override;

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CTargetMark(
		sBULLET_PARAM bulletParam,
		float angle,
		Vec2 missilePos,
		float missileSpeed,
		bool isMissileChangedToCoin,
		CBullet* owner);

	virtual ~CTargetMark(){};

private:
	CBullet* m_OwnerBullet;			// owner missile
	float m_fArriveTime;			// 미사일 도착시간 =  target
	Rect m_ScreenRect;
	bool m_bIsMissileChangedToCoin;
};