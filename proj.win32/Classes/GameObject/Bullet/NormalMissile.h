#pragma once
#include "Bullet.h"

class CGameObject;

class CNormalMissile : public CBullet {
public:
	/*create를 호출하면 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
	static CNormalMissile* create(
		std::string textureName,	//bullet 이미지
		float boundingRadius,		//bullet 충돌 범위
		float angle,				//bullet 초기 각도 
		float speed,				//bullet 초기 속도
		bool isAiming = false);		//조준미사일인지 여부 true = CrushShake 호출

	virtual void Execute(float delta = 0.f) override;
	virtual void Rotation(int dir) override;
	virtual void CollisionWithPlayer();
	virtual void CollisionWithPlanet();
	virtual void ChangeToCoin();
	virtual void ChangeToStar();

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CNormalMissile(std::string textureName,
		float boundingRadius,
		float angle,
		float speed,
		bool isAiming = false);
	virtual ~CNormalMissile(){};

private:
	bool m_bIsAimingMissile;		// 조준미사일인지 여부 true = CrushShake 호출
	Rect m_ScreenRect;
};
