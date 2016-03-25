#pragma once
#include "Bullet.h"

class CPlayItem : public CBullet
{
public:
	/*create�� ȣ���ϸ� CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�.*/
	static CPlayItem* create(
		std::string textureName,	//item �̹���
		float boundingRadius,		//item �浹 ����
		float angle,				//item �ʱ� ���� 
		float speed,				//item �ʱ� �ӵ�
		int itemType,				//item Ÿ��
		bool isFly);				//fly item ���� ground item ����

	virtual void Execute(float delta = 0.f) override;
	virtual void CollisionWithPlayer();
	virtual void CollisionWithPlanet();

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CPlayItem(
		std::string textureName,
		float boundingRadius,
		float angle,
		float speed,
		int itemType,
		bool isFly);

	virtual ~CPlayItem(){};

private:
	Vec2 m_TargetPos;		//player�� ������ �÷��̾� ��ġ�� ���ư�
	eITEM_FLAG m_ItemType;
	bool m_bPlayerGet;
};

