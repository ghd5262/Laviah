#pragma once
#include "Bullet.h"

class CPlayItem : public CBullet
{
public:
	/*create를 호출하면 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
	static CPlayItem* create(
		std::string textureName,	//item 이미지
		float boundingRadius,		//item 충돌 범위
		float angle,				//item 초기 각도 
		float speed,				//item 초기 속도
		int itemType,				//item 타입
		bool isFly);				//fly item 인지 ground item 인지

	virtual void Execute(float delta = 0.f) override;
	virtual void CollisionWithPlayer();
	virtual void CollisionWithPlanet();

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CPlayItem(
		std::string textureName,
		float boundingRadius,
		float angle,
		float speed,
		int itemType,
		bool isFly);

	virtual ~CPlayItem(){};

private:
	Vec2 m_TargetPos;		//player가 먹으면 플레이어 위치로 날아감
	eITEM_FLAG m_ItemType;
	bool m_bPlayerGet;
};

