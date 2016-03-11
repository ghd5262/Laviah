#pragma once

#include "../Bullet.h"
class CGameObject;

class CNormalBullet : public CBullet {
public:
	/*create�� ȣ���ϸ� CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�.*/
	static CNormalBullet* create(
		std::string textureName,	//bullet �̹���
		float boundingRadius,		//bullet �浹 ����
		float angle,				//bullet �ʱ� ���� 
		float speed,				//bullet �ʱ� �ӵ�
		CGameObject* target);		//bullet Ÿ��

	virtual void Execute(float delta = 0.f) override;

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CNormalBullet(std::string textureName,
		float boundingRadius,
		float angle,
		float speed,
		CGameObject* target);
	virtual ~CNormalBullet(){};
};


class CTargetMark : public CBullet {
public:
	/*create�� ȣ���ϸ� CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�.*/
	static CTargetMark* create(
		std::string textureName,	//bullet �̹���
		float boundingRadius,		//bullet �浹 ����
		float angle,				//bullet �ʱ� ���� 
		float speed,				//bullet �ʱ� �ӵ�
		CGameObject* target,		//bullet Ÿ��
		CBullet* owner);			//���� Bullet

	virtual void Execute(float delta = 0.f) override;

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CTargetMark(std::string textureName,
		float boundingRadius,
		float angle,
		float speed,
		CGameObject* target,
		CBullet* owner);
	virtual ~CTargetMark(){};

private:
	CBullet* m_OwnerBullet;
	Rect m_ScreenRect;
};


class CNormalMissile : public CBullet {
public:
	/*create�� ȣ���ϸ� CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�.*/
	static CNormalMissile* create(
		std::string textureName,	//bullet �̹���
		float boundingRadius,		//bullet �浹 ����
		float angle,				//bullet �ʱ� ���� 
		float speed,				//bullet �ʱ� �ӵ�
		CGameObject* target,		//bullet Ÿ��
		bool isAiming = false);		//���ع̻������� ���� true = CrushShake ȣ��
	virtual void Execute(float delta = 0.f) override;

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CNormalMissile(std::string textureName,
		float boundingRadius,
		float angle,
		float speed,
		CGameObject* target,
		bool isAiming = false);				
	virtual ~CNormalMissile(){};

private:
	bool m_bIsAimingMissile;		// ���ع̻������� ���� true = CrushShake ȣ��
};