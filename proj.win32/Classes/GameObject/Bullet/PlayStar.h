#pragma once
#include "Bullet.h"

class CScoreUI;
enum eSTAR_TYPE;
class CPlayStar : public CBullet
{
public:
	/*create를 호출하면 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
    static CPlayStar* create(sBULLET_PARAM bulletParam, float angle, cocos2d::Vec2 createPosition = cocos2d::Vec2::ZERO);

	virtual void Execute(float delta = 0.f) override;
	virtual void CollisionWithPlayer() override;
	virtual void CollisionWithPlanet() override;

protected:
	virtual bool init() override;

private:
	CPlayStar(
		sBULLET_PARAM bulletParam,
		float angle,
		cocos2d::Vec2 createPosition);
	virtual ~CPlayStar(){};

private:
	cocos2d::Vec2 m_CreatePos;
	float m_fStarValue;
	//충돌 파티클
	cocos2d::ParticleSystemQuad* m_pParticleCrash;
};


