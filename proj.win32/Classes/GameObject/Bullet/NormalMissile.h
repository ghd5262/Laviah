#pragma once
#include "Bullet.h"

class CGameObject;

class CNormalMissile : public CBullet {
public:
	/*create�� ȣ���ϸ� CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�.*/
	static CNormalMissile* create(
		std::string textureName,	//bullet �̹���
		float boundingRadius,		//bullet �浹 ����
		float angle,				//bullet �ʱ� ���� 
		float speed,				//bullet �ʱ� �ӵ�
		bool isAiming = false);		//���ع̻������� ���� true = CrushShake ȣ��

	virtual void Execute(float delta = 0.f) override;
	virtual void Rotation(int dir) override;
	virtual void CollisionWithPlayer();
	virtual void CollisionWithPlanet();
	virtual void ChangeToCoin();
	virtual void ChangeToStar();

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CNormalMissile(std::string textureName,
		float boundingRadius,
		float angle,
		float speed,
		bool isAiming = false);
	virtual ~CNormalMissile(){};

private:
	bool m_bIsAimingMissile;		// ���ع̻������� ���� true = CrushShake ȣ��
	Rect m_ScreenRect;
};
