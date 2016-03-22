#pragma once
#include "../Bullet.h"

class CGameObject;

class CTargetMark : public CBullet {
public:
	/*create�� ȣ���ϸ� CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�.*/
	static CTargetMark* create(
		std::string textureName,	//bullet �̹���
		float boundingRadius,		//bullet �浹 ����
		float angle,				//bullet �ʱ� ���� 
		float speed,				//bullet �ʱ� �ӵ�
		CBullet* owner, 			//���� Bullet
		bool isAiming = false);		//���ع̻������� ����

	virtual void Execute(float delta = 0.f) override;
	virtual void Rotation(int dir) override;

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CTargetMark(std::string textureName,
		float boundingRadius,
		float angle,
		float speed,
		CBullet* owner,
		bool isAiming = false);

	virtual ~CTargetMark(){};

private:
	bool m_bIsAimingMissile;		// ���ع̻������� ����
	CBullet* m_OwnerBullet;
	Rect m_ScreenRect;
};