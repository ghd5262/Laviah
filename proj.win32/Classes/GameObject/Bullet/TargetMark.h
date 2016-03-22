#pragma once
#include "../Bullet.h"

class CGameObject;

class CTargetMark : public CBullet {
public:
	/*create를 호출하면 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
	static CTargetMark* create(
		std::string textureName,	//bullet 이미지
		float boundingRadius,		//bullet 충돌 범위
		float angle,				//bullet 초기 각도 
		float speed,				//bullet 초기 속도
		CBullet* owner, 			//소유 Bullet
		bool isAiming = false);		//조준미사일인지 여부

	virtual void Execute(float delta = 0.f) override;
	virtual void Rotation(int dir) override;

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CTargetMark(std::string textureName,
		float boundingRadius,
		float angle,
		float speed,
		CBullet* owner,
		bool isAiming = false);

	virtual ~CTargetMark(){};

private:
	bool m_bIsAimingMissile;		// 조준미사일인지 여부
	CBullet* m_OwnerBullet;
	Rect m_ScreenRect;
};