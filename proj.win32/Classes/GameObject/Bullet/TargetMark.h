#pragma once
#include "Bullet.h"

class CGameObject;

class CTargetMark : public CBullet {
public:
	/*create�� ȣ���ϸ� CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�.*/
	static CTargetMark* create(
		sBULLET_PARAM bulletParam,
		float angle,				//TargetMark �ʱ� ���� 
		Vec2 missilePos,			//Missile ���� ��ǥ
		float missileSpeed,			//Missile �ӷ�
		bool isMissileChangedToCoin = true,
		CBullet* owner = nullptr);	//owner missile (nullptr �� ������ �����ð����� �����Ѵ�.)
	virtual void Execute(float delta = 0.f) override;
	virtual void Rotation(int dir) override;

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CTargetMark(
		sBULLET_PARAM bulletParam,
		float angle,
		Vec2 missilePos,
		float missileSpeed,
		bool isMissileChangedToCoin,
		CBullet* owner);

	virtual ~CTargetMark(){};

private:
	CBullet* m_OwnerBullet;			// owner missile
	float m_fArriveTime;			// �̻��� �����ð� =  target
	Rect m_ScreenRect;
	bool m_bIsMissileChangedToCoin;
};