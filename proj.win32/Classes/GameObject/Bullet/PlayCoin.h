#pragma once
#include "Bullet.h"

enum eCOIN_TYPE;
class CPlayCoin : public CBullet
{
public:
	/*create�� ȣ���ϸ� CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�.*/
	static CPlayCoin* create(
		sBULLET_PARAM bulletParam,
		float angle,				//coin �ʱ� ���� 
		float speed,				//coin �ʱ� �ӵ�
		Vec2 createPosition = Vec2(0.0f, 0.0f));

	virtual void Execute(float delta = 0.f) override;
	virtual void CollisionWithPlayer();
	virtual void CollisionWithPlanet();

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CPlayCoin(
		sBULLET_PARAM bulletParam,
		float angle,
		float speed,
		Vec2 createPosition );

	virtual ~CPlayCoin(){};

private:
	Vec2 m_CreatePos;
	float m_fCoinValue;
};

