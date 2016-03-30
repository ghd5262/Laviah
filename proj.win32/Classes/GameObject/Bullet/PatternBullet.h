#pragma once
#include "Bullet.h"

class CPatternBullet : public CBullet
{
public:
	/*create를 호출하면 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
	static CPatternBullet* create(
		float boundingRadius,		//coin 충돌 범위
		float angle,				//coin 초기 각도 
		float speed);				//coin 초기 속도

	virtual void Execute(float delta = 0.f) override;
	virtual void CollisionWithPlayer();
	virtual void CollisionWithPlanet();

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CPatternBullet(
		eCOIN_TYPE coinType,
		float boundingRadius,
		float angle,
		float speed,
		bool isFly,
		Vec2 createPosition);

	virtual ~CPatternBullet(){};

private:
	CScoreUI* m_pUIScore;
	Vec2 m_CreatePos;
	eCOIN_TYPE m_CoinType;	//coin 타입
	float m_fCoinValue;
};

