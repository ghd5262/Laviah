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
	//* �����Լ� */
	static CSoccerBall* create(Vec2 pos, double ballSize, double mass, std::vector<sWALL>* vecPitchBoundary);

	//* ���� ����� �߰��Ѵ�. */
	Vec2 addNoiseToKick(Vec2 ballPos, Vec2 ballTarget);

	//* ���� ���⼺ ���� �����Ѵ�. */
	void kick(Vec2 direction, double force);

	//* ������ġ�� ������ ��ġ ���� �Ű������� �޾� �����ϴ� �ð��� ��ȯ */
	double timeToCoverDistance(Vec2 from, Vec2 to, double force) const;
	
	//* �־��� �ð��� ���� ��ġ�� ��ȯ */
	Vec2 FuturePosition(double time) const;
	
	//* ���� ���ϴ� ��ġ�� ������ �ӵ��� 0���� �����Ѵ�. */
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