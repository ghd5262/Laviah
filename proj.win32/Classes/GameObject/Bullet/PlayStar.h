#pragma once
#include "Bullet.h"

class CScoreUI;
enum eSTAR_TYPE;
class CPlayStar : public CBullet
{
public:
	/*create�� ȣ���ϸ� CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�.*/
	static CPlayStar* create(
		sBULLET_PARAM bulletParam,
		float angle,				//star �ʱ� ���� 
		float speed,				//star �ʱ� �ӵ�
		Vec2 createPosition = Vec2(0.0f, 0.0f));

	virtual void Execute(float delta = 0.f) override;
	virtual void CollisionWithPlayer();
	virtual void CollisionWithPlanet();

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CPlayStar(
		sBULLET_PARAM bulletParam,
		float angle,
		float speed,
		Vec2 createPosition);
	virtual ~CPlayStar(){};

private:
	Vec2 m_CreatePos;
	float m_fStarValue;
};


