#pragma once
#include "../Enemy.h"

namespace Shooter{
	enum SHOOTER_OPTION{
		RIGHT = 0,
		LEFT = 1
	};


	//--------------------------Random Shooter--------------------------
	/* RandomShooter : 360�� ��濡�� �������� �Ѿ��� ��� ����*/
	class CRandomShooter : public CEnemy {
	public:
		/* create�� ȣ���ϸ� operator new�� ȣ��Ǹ鼭 CObjectManager���� �޸𸮸� �޴´�.
		���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�. */
		static CRandomShooter* create(float speed, float interval, int maxBulletCount);						// interval = Bullet ���� ����
		virtual void Execute(float delta) override;

	private:
		CRandomShooter(float speed, float interval, int maxBulletCount);
		virtual ~CRandomShooter(){}

	private:
		float m_fRandomInterval;
		int m_nBulletColor;
		int m_nMax;			// �ѹ��� ��� �Ѿ��� �� max��
		int m_nBulletCount;	// �ѹ��� ��� �Ѿ��� ��
	};

	void RandomShoot(float speed = 250.0f, float interval = 0.1f, int maxBulletCount = 1);
	//------------------------------------------------------------------


	//--------------------------Screw Shooter--------------------------
	/* ScrewShooter : �ð����, �ݽð�������� ���鼭 ��� ����*/
	class CScrewShooter : public CEnemy {

	public:
		/* create�� ȣ���ϸ� operator new�� ȣ��Ǹ鼭 CObjectManager���� �޸𸮸� �޴´�.
		���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�. */
		static CScrewShooter* create(float speed, float interval, int bulletCount, SHOOTER_OPTION direction);				// direction = Bullet ���� : false = �ð����
		virtual void Execute(float delta) override;

	private:
		CScrewShooter(float speed, float interval, int bulletCount, SHOOTER_OPTION direction);
		virtual ~CScrewShooter(){}

	private:
		int m_nBulletColor;
		int m_nBulletCount;	// �ѹ��� ��� �Ѿ��� ��
		SHOOTER_OPTION m_Direction;
	};

	void ScrewShoot(float speed = 250.0f, float interval = 0.1f, int bulletCount = 1, SHOOTER_OPTION direction = RIGHT);
	//------------------------------------------------------------------


	//--------------------------DoubleScrew Shooter--------------------------
	/* DoubleScrewShooter : ����� �ð����, �ݽð�������� ���鼭 ��� ����*/
	class CDoubleScrewShooter : public CEnemy {

	public:
		/* create�� ȣ���ϸ� operator new�� ȣ��Ǹ鼭 CObjectManager���� �޸𸮸� �޴´�.
		���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�. */
		static CDoubleScrewShooter* create(float speed, float interval, int bulletCount, SHOOTER_OPTION direction);				// direction = Bullet ���� : false = �ð����
		virtual void Execute(float delta) override;

	private:
		CDoubleScrewShooter(float speed, float interval, int bulletCount, SHOOTER_OPTION direction);
		virtual ~CDoubleScrewShooter(){}

	private:
		int m_nBulletColor;
		int m_nBulletCount;	// �ѹ��� ��� �Ѿ��� ��
		SHOOTER_OPTION m_Direction;
	};

	void DoubleScrewShoot(float speed = 250.0f, float interval = 0.1f, int bulletCount = 1, SHOOTER_OPTION direction = RIGHT);
	//------------------------------------------------------------------


	//--------------------------Barrier Shooter--------------------------
	/* BarrierShooter : ź�����̿� ���� �ִ� ����*/
	class CBarrierShooter : public CEnemy {

	public:
		/* create�� ȣ���ϸ� operator new�� ȣ��Ǹ鼭 CObjectManager���� �޸𸮸� �޴´�.
		���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�. */
		static CBarrierShooter* create(float speed, float interval, int waySize);				// waySize = ���� ũ��
		virtual void Execute(float delta) override;

	private:
		CBarrierShooter(float speed, float interval, int waySize);
		virtual ~CBarrierShooter(){}

	private:
		int m_nBulletColor;
		int m_nWaySize;			// ź�����̿� �����ϴ� ���� ũ�� 1�� �Ѿ� �Ѱ�ũ��
		char* m_BarrierSize;	// ź���� ũ��
	};

	void BarrierShoot(float speed = 250.0f, float interval = 0.1f, int waySize = 10);
	//------------------------------------------------------------------


	//--------------------------RandomMissile Shooter--------------------------
	/* RandomShooter : 360�� ��濡�� �������� �̻����� ��� ����, �̻����� ������*/
	class CRandomMissileShooter : public CEnemy {
	public:
		/* create�� ȣ���ϸ� operator new�� ȣ��Ǹ鼭 CObjectManager���� �޸𸮸� �޴´�.
		���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�. */
		static CRandomMissileShooter* create(float speed, float interval, int maxBulletCount);						// interval = Bullet ���� ����
		virtual void Execute(float delta) override;

	private:
		CRandomMissileShooter(float speed, float interval, int maxBulletCount);
		virtual ~CRandomMissileShooter(){}

	private:
		int m_nMax;			// �ѹ��� ��� �Ѿ��� �� max��
		int m_nBulletCount;	// �ѹ��� ��� �Ѿ��� ��
	};

	void RandomMissileShoot(float speed = 600.0f, float interval = 0.1f, int maxBulletCount = 1);
	//------------------------------------------------------------------


	//--------------------------AimingMissile Shooter--------------------------
	/* AimingMissileShooter : ĳ���͸� �����ϰ� �̻����� ��� ����*/
	class CAimingMissileShooter : public CEnemy {
	public:
		/* create�� ȣ���ϸ� operator new�� ȣ��Ǹ鼭 CObjectManager���� �޸𸮸� �޴´�.
		���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�. */
		static CAimingMissileShooter* create(float speed, float interval);						// interval = Bullet ���� ����
		virtual void Execute(float delta) override;

	private:
		CAimingMissileShooter(float speed, float interval);
		virtual ~CAimingMissileShooter(){}
	};

	void AimingMissileShoot(float speed = 600.0f, float interval = 0.1f);
	//------------------------------------------------------------------
}