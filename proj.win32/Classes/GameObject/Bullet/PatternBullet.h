#pragma once
#include "Bullet.h"

class CPatternBullet : public CBullet
{
public:
	/*create�� ȣ���ϸ� CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�.*/
	static CPatternBullet* create(
		float boundingRadius,		//coin �浹 ����
		float angle,				//coin �ʱ� ���� 
		float speed);				//coin �ʱ� �ӵ�

	virtual void Execute(float delta = 0.f) override;
	virtual void CollisionWithPlayer();
	virtual void CollisionWithPlanet();

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CPatternBullet(
		eCOIN_TYPE coinType,
		float boundingRadius,
		float angle,
		float speed,
		bool isFly,
		Vec2 createPosition);

	virtual ~CPatternBullet(){};

private:
	CScoreUI* m_pUIScore;
	Vec2 m_CreatePos;
	eCOIN_TYPE m_CoinType;	//coin Ÿ��
	float m_fCoinValue;
};

