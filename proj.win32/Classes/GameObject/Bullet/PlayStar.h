#pragma once
#include "Bullet.h"

class CScoreUI;
enum eSTAR_TYPE;
class CPlayStar : public CBullet
{
public:
	/*create를 호출하면 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
	static CPlayStar* create(
		eSTAR_TYPE starType,		//star 타입 (브론즈, 실버, 골드 등)
		float boundingRadius,		//star 충돌 범위
		float distance,				//star 초기 거리
		float angle,				//star 초기 각도 
		float speed,				//star 초기 속도
		bool isFly,					//fly star 인지 ground star 인지
		Vec2 createPosition = Vec2(0.0f, 0.0f));

	virtual void Execute(float delta = 0.f) override;
	virtual void CollisionWithPlayer();
	virtual void CollisionWithPlanet();

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CPlayStar(
		eSTAR_TYPE starType,
		float boundingRadius,
		float distance,
		float angle,
		float speed,
		bool isFly,
		Vec2 createPosition);
	virtual ~CPlayStar(){};

private:
	CScoreUI* m_pUIScore;
	Vec2 m_CreatePos;
	eSTAR_TYPE m_StarType;	//스타타입
	float m_fStarValue;
	float m_fDistance;
};


