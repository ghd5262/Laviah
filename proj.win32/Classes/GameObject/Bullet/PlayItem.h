#pragma once
#include "Bullet.h"

enum eITEM_TYPE;
class CPlayItem : public CBullet
{
public:
	/*create를 호출하면 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
	static CPlayItem* create(sBULLET_PARAM bulletParam, float angle);

	virtual void Execute(float delta = 0.f) override;
	virtual void CollisionWithPlayer() override;
	virtual void CollisionWithPlanet() override;

protected:
	virtual bool init() override;

private:
	CPlayItem(
		sBULLET_PARAM bulletParam,
		float angle);

	virtual ~CPlayItem(){};

private:
	cocos2d::Vec2 m_TargetPos;		//player가 먹으면 플레이어 위치로 날아감
};

