#pragma once
#include "Bullet.h"

class CGameObject;

class CNormalBullet : public CBullet {
public:
	/*create�� ȣ���ϸ� CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�.*/
	static CNormalBullet* create(
		std::string textureName,	//bullet �̹���
		float boundingRadius,		//bullet �浹 ����
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
		std::string textureName,
		float boundingRadius,
		float angle,
		float speed);
	virtual ~CNormalBullet(){};
};