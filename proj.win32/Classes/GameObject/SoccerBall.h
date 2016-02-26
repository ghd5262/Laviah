#pragma once

#include "MovingEntity.h"
#include "SoccerPitch.h"
#include <vector>

class CPlayerBase;

class CSoccerBall : public CMovingEntity
{
public:

	CSoccerBall(Vec2 pos, double ballSize, double mass, std::vector<sWALL>* vecPitchBoundary)
		: CMovingEntity(pos
		, ballSize
		, Vec2(0, 0)
		, -1.0f
		, Vec2(0, 1)
		, mass
		, 0
		, 0)
		, m_vecPitchBoundary(vecPitchBoundary)
		, m_pOwner(nullptr){};
	virtual ~CSoccerBall();

public:
	//* 생성함수 */
	static CSoccerBall* create(Vec2 pos, double ballSize, double mass, std::vector<sWALL>* vecPitchBoundary);

	//* 공에 노이즈를 추가한다. */
	Vec2 addNoiseToKick(Vec2 ballPos, Vec2 ballTarget);

	//* 공에 방향성 힘을 적용한다. */
	void kick(Vec2 direction, double force);

	//* 시작위치와 끝나는 위치 힘을 매개변수로 받아 도착하는 시간을 반환 */
	double timeToCoverDistance(Vec2 from, Vec2 to, double force) const;
	
	//* 주어진 시간에 공의 위치를 반환 */
	Vec2 FuturePosition(double time) const;
	
	//* 공을 원하는 위치에 놓으며 속도를 0으로 셋팅한다. */
	void PlaceAtPosition(Vec2 newPos);

protected:
	virtual bool init();

	///getter & setter
	CC_SYNTHESIZE(Point, m_oldPos, OldPos);

private:
	bool initVariable();

private:
	const std::vector<sWALL>* m_vecPitchBoundary;
	CPlayerBase* m_pOwner;

	Sprite* m_pImgSoccerBall;
};

inline CSoccerBall* CSoccerBall::create(Vec2 pos, double ballSize, double mass, std::vector<sWALL>* vecPitchBoundary)
{
	CSoccerBall *pRet = new(std::nothrow) CSoccerBall(pos, ballSize, mass, vecPitchBoundary);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}