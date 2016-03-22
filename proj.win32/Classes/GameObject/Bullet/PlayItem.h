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
	eITEM_FLAG_none = 0x0000,		//eITEM_TYPE에서 eITEM_FLAG으로 변환하면 아래와 같다.
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
	/*create를 호출하면 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
	static CPlayItem* create(
		std::string textureName,	//item 이미지
		float boundingRadius,		//item 충돌 범위
		float angle,				//item 초기 각도 
		float speed,				//item 초기 속도
		int itemType,				//item 타입
		bool isFly);				//fly item 인지 ground item 인지
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
	Vec2 m_TargetPos;		//player가 먹으면 플레이어 위치로 날아감
	CPlayer* m_Player;
	eITEM_FLAG m_ItemType;
	bool m_bIsFlyItem;		//fly item 인지 ground item 인지
	bool m_bPlayerGet;
};

