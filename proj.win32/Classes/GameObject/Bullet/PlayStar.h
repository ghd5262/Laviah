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
		eSTAR_TYPE starType,		//star Ÿ�� (�����, �ǹ�, ��� ��)
		float boundingRadius,		//star �浹 ����
		float distance,				//star �ʱ� �Ÿ�
		float angle,				//star �ʱ� ���� 
		float speed,				//star �ʱ� �ӵ�
		bool isFly,					//fly star ���� ground star ����
		Vec2 createPosition = Vec2(0.0f, 0.0f));

	virtual void Execute(float delta = 0.f) override;
	virtual void CollisionWithPlayer();
	virtual void CollisionWithPlanet();

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CPlayStar(
		eSTAR_TYPE starType,
		float boundingRadius,
		float distance,
		float angle,
		float speed,
		bool isFly,
		Vec2 createPosition);
	virtual ~CPlayStar(){};

private:
	CScoreUI* m_pUIScore;
	Vec2 m_CreatePos;
	eSTAR_TYPE m_StarType;	//��ŸŸ��
	float m_fStarValue;
	float m_fDistance;
};


