#pragma once

#include "../Bullet.h"
class CGameObject;

class CNormalBullet : public CBullet {
public:
	/*create를 호출하면 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
	static CNormalBullet* create(
		std::string textureName,	//bullet 이미지
		float boundingRadius,		//bullet 충돌 범위
		float angle,				//bullet 초기 각도 
		float speed,				//bullet 초기 속도
		CGameObject* target);		//bullet 타겟

	virtual void Execute(float delta = 0.f) override;

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CNormalBullet(std::string textureName,
		float boundingRadius,
		float angle,
		float speed,
		CGameObject* target);
	virtual ~CNormalBullet(){};
};


class CTargetMark : public CBullet {
public:
	/*create를 호출하면 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
	static CTargetMark* create(
		std::string textureName,	//bullet 이미지
		float boundingRadius,		//bullet 충돌 범위
		float angle,				//bullet 초기 각도 
		float speed,				//bullet 초기 속도
		CGameObject* target,		//bullet 타겟
		CBullet* owner);			//소유 Bullet

	virtual void Execute(float delta = 0.f) override;

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CTargetMark(std::string textureName,
		float boundingRadius,
		float angle,
		float speed,
		CGameObject* target,
		CBullet* owner);
	virtual ~CTargetMark(){};

private:
	CBullet* m_OwnerBullet;
	Rect m_ScreenRect;
};


class CNormalMissile : public CBullet {
public:
	/*create를 호출하면 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
	static CNormalMissile* create(
		std::string textureName,	//bullet 이미지
		float boundingRadius,		//bullet 충돌 범위
		float angle,				//bullet 초기 각도 
		float speed,				//bullet 초기 속도
		CGameObject* target,		//bullet 타겟
		bool isAiming = false);		//조준미사일인지 여부 true = CrushShake 호출
	virtual void Execute(float delta = 0.f) override;

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CNormalMissile(std::string textureName,
		float boundingRadius,
		float angle,
		float speed,
		CGameObject* target,
		bool isAiming = false);				
	virtual ~CNormalMissile(){};

private:
	bool m_bIsAimingMissile;		// 조준미사일인지 여부 true = CrushShake 호출
};