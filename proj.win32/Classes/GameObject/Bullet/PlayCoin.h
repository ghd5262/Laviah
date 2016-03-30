#pragma once
#include "Bullet.h"

class CScoreUI;
enum eCOIN_TYPE;
class CPlayCoin : public CBullet
{
public:
	/*create�� ȣ���ϸ� CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�.*/
	static CPlayCoin* create(
		eCOIN_TYPE coinType,		//coin Ÿ�� (�����, �ǹ�, ��� ��)
		float boundingRadius,		//coin �浹 ����
		float distance,				//coin �ʱ� �Ÿ�
		float angle,				//coin �ʱ� ���� 
		float speed,				//coin �ʱ� �ӵ�
		bool isFly,					//fly coin ���� ground coin ����
		Vec2 createPosition = Vec2(0.0f, 0.0f));

	virtual void Execute(float delta = 0.f) override;
	virtual void CollisionWithPlayer();
	virtual void CollisionWithPlanet();

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CPlayCoin(
		eCOIN_TYPE coinType,
		float boundingRadius,
		float distance,				
		float angle,
		float speed,
		bool isFly,
		Vec2 createPosition );

	virtual ~CPlayCoin(){};

private:
	CScoreUI* m_pUIScore;
	Vec2 m_CreatePos;
	eCOIN_TYPE m_CoinType;	//coin Ÿ��
	float m_fCoinValue;
	float m_fDistance;
};

