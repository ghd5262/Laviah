#pragma once
#include "Bullet.h"

enum eITEM_TYPE;
class CPlayItem : public CBullet
{
public:
	/*create�� ȣ���ϸ� CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�.*/
	static CPlayItem* create(
		sBULLET_PARAM bulletParam,
		float angle,				//item �ʱ� ���� 
		float speed);				//item �ʱ� �ӵ�

	virtual void Execute(float delta = 0.f) override;
	virtual void CollisionWithPlayer();
	virtual void CollisionWithPlanet();

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CPlayItem(
		sBULLET_PARAM bulletParam,
		float angle,
		float speed);

	virtual ~CPlayItem(){};

private:
	Vec2 m_TargetPos;		//player�� ������ �÷��̾� ��ġ�� ���ư�
};

