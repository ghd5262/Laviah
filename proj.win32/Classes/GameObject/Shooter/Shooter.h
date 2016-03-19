#pragma once
#include "../Enemy.h"

namespace Shooter{
	enum SHOOTER_OPTION{
		RIGHT = 0,
		LEFT = 1
	};


	//--------------------------Random Shooter--------------------------
	/* RandomShooter : 360도 사방에서 랜덤으로 총알을 쏘는 패턴*/
	class CRandomShooter : public CEnemy {
	public:
		/* create를 호출하면 operator new가 호출되면서 CObjectManager에서 메모리를 받는다.
		받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다. */
		static CRandomShooter* create(float speed, float interval, int maxBulletCount);						// interval = Bullet 생성 간격
		virtual void Execute(float delta) override;

	private:
		CRandomShooter(float speed, float interval, int maxBulletCount);
		virtual ~CRandomShooter(){}

	private:
		float m_fRandomInterval;
		int m_nBulletColor;
		int m_nMax;			// 한번에 쏘는 총알의 수 max값
		int m_nBulletCount;	// 한번에 쏘는 총알의 수
	};

	void RandomShoot(float speed = 250.0f, float interval = 0.1f, int maxBulletCount = 1);
	//------------------------------------------------------------------


	//--------------------------Screw Shooter--------------------------
	/* ScrewShooter : 시계방향, 반시계방향으로 돌면서 쏘는 패턴*/
	class CScrewShooter : public CEnemy {

	public:
		/* create를 호출하면 operator new가 호출되면서 CObjectManager에서 메모리를 받는다.
		받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다. */
		static CScrewShooter* create(float speed, float interval, int bulletCount, SHOOTER_OPTION direction);				// direction = Bullet 방향 : false = 시계방향
		virtual void Execute(float delta) override;

	private:
		CScrewShooter(float speed, float interval, int bulletCount, SHOOTER_OPTION direction);
		virtual ~CScrewShooter(){}

	private:
		int m_nBulletColor;
		int m_nBulletCount;	// 한번에 쏘는 총알의 수
		SHOOTER_OPTION m_Direction;
	};

	void ScrewShoot(float speed = 250.0f, float interval = 0.1f, int bulletCount = 1, SHOOTER_OPTION direction = RIGHT);
	//------------------------------------------------------------------


	//--------------------------DoubleScrew Shooter--------------------------
	/* DoubleScrewShooter : 더블로 시계방향, 반시계방향으로 돌면서 쏘는 패턴*/
	class CDoubleScrewShooter : public CEnemy {

	public:
		/* create를 호출하면 operator new가 호출되면서 CObjectManager에서 메모리를 받는다.
		받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다. */
		static CDoubleScrewShooter* create(float speed, float interval, int bulletCount, SHOOTER_OPTION direction);				// direction = Bullet 방향 : false = 시계방향
		virtual void Execute(float delta) override;

	private:
		CDoubleScrewShooter(float speed, float interval, int bulletCount, SHOOTER_OPTION direction);
		virtual ~CDoubleScrewShooter(){}

	private:
		int m_nBulletColor;
		int m_nBulletCount;	// 한번에 쏘는 총알의 수
		SHOOTER_OPTION m_Direction;
	};

	void DoubleScrewShoot(float speed = 250.0f, float interval = 0.1f, int bulletCount = 1, SHOOTER_OPTION direction = RIGHT);
	//------------------------------------------------------------------


	//--------------------------Barrier Shooter--------------------------
	/* BarrierShooter : 탄막사이에 길이 있는 패턴*/
	class CBarrierShooter : public CEnemy {

	public:
		/* create를 호출하면 operator new가 호출되면서 CObjectManager에서 메모리를 받는다.
		받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다. */
		static CBarrierShooter* create(float speed, float interval, int waySize);				// waySize = 길의 크기
		virtual void Execute(float delta) override;

	private:
		CBarrierShooter(float speed, float interval, int waySize);
		virtual ~CBarrierShooter(){}

	private:
		int m_nBulletColor;
		int m_nWaySize;			// 탄막사이에 존재하는 길의 크기 1당 총알 한개크기
		char* m_BarrierSize;	// 탄막의 크기
	};

	void BarrierShoot(float speed = 250.0f, float interval = 0.1f, int waySize = 10);
	//------------------------------------------------------------------


	//--------------------------RandomMissile Shooter--------------------------
	/* RandomShooter : 360도 사방에서 랜덤으로 미사일을 쏘는 패턴, 미사일은 빠르다*/
	class CRandomMissileShooter : public CEnemy {
	public:
		/* create를 호출하면 operator new가 호출되면서 CObjectManager에서 메모리를 받는다.
		받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다. */
		static CRandomMissileShooter* create(float speed, float interval, int maxBulletCount);						// interval = Bullet 생성 간격
		virtual void Execute(float delta) override;

	private:
		CRandomMissileShooter(float speed, float interval, int maxBulletCount);
		virtual ~CRandomMissileShooter(){}

	private:
		int m_nMax;			// 한번에 쏘는 총알의 수 max값
		int m_nBulletCount;	// 한번에 쏘는 총알의 수
	};

	void RandomMissileShoot(float speed = 600.0f, float interval = 0.1f, int maxBulletCount = 1);
	//------------------------------------------------------------------


	//--------------------------AimingMissile Shooter--------------------------
	/* AimingMissileShooter : 캐릭터를 조준하고 미사일을 쏘는 패턴*/
	class CAimingMissileShooter : public CEnemy {
	public:
		/* create를 호출하면 operator new가 호출되면서 CObjectManager에서 메모리를 받는다.
		받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다. */
		static CAimingMissileShooter* create(float speed, float interval);						// interval = Bullet 생성 간격
		virtual void Execute(float delta) override;

	private:
		CAimingMissileShooter(float speed, float interval);
		virtual ~CAimingMissileShooter(){}
	};

	void AimingMissileShoot(float speed = 600.0f, float interval = 0.1f);
	//------------------------------------------------------------------
}