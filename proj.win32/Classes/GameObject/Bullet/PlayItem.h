#pragma once
#include "../Bullet.h"

enum eITEM_TYPE{
	eITEM_TYPE_health = 0,
	eITEM_TYPE_shield = 1,
	eITEM_TYPE_magnet = 2,
	eITEM_TYPE_coin = 3,
	eITEM_TYPE_star = 4,
	eITEM_TYPE_giant = 5,
	eITEM_TYPE_bonustime = 6,

	eITEM_TYPE_MAX
};

enum eITEM_FLAG{
	eITEM_FLAG_none = 0x0000,		//eITEM_TYPE���� eITEM_FLAG���� ��ȯ�ϸ� �Ʒ��� ����.
	eITEM_FLAG_shield = 0x0002,		//== eITEM_TYPE_shield = 1,
	eITEM_FLAG_magnet = 0x0004,		//== eITEM_TYPE_magnet = 2,
	eITEM_FLAG_coin = 0x0008,		//== eITEM_TYPE_coin = 3,
	eITEM_FLAG_star = 0x00010,		//== eITEM_TYPE_star = 4,
	eITEM_FLAG_giant = 0x0020,		//== eITEM_TYPE_giant = 5,
	eITEM_FLAG_bonustime = 0x0040,	//== eITEM_TYPE_bonustime = 6,
};

class CPlayer;
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
	virtual void Rotation(int dir) override;
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
	CPlayer* m_Player;
	eITEM_FLAG m_ItemType;
	bool m_bIsFlyItem;		//fly item ���� ground item ����
	bool m_bPlayerGet;
};

