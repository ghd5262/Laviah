#pragma once
#include "Bullet.h"

class CGameObject;

class CNormalBullet : public CBullet {
public:
	/*create�� ȣ���ϸ� CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�.*/
	static CNormalBullet* create(
		sBULLET_PARAM bulletParam,
		float angle,				//bullet �ʱ� ���� 
		float speed);				//bullet �ʱ� �ӵ�

	virtual void Execute(float delta = 0.f) override;
	virtual void CollisionWithPlayer();
	virtual void CollisionWithPlanet();
	virtual void ChangeToCoin();
	virtual void ChangeToStar();

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CNormalBullet(
		sBULLET_PARAM bulletParam,
		float angle,
		float speed);
	virtual ~CNormalBullet(){};
};