#include "ObjectManager.h"
#include "Planet.h"
#include "Player.h"
#include "ItemManager.h"
#include "Bullet/Bullet.h"
#include "Shooter/Shooter.h"
#include "Stage/StageManager.h"

CObjectManager* CObjectManager::Instance()
{
	static CObjectManager instance;
	return &instance;
}

void CObjectManager::AddBullet(void* bullet)
{
	m_BulletList.emplace_back(static_cast<CBullet*>(bullet));
}

void CObjectManager::AddShooter(void* shooter)
{
	m_ShooterList.emplace_back(static_cast<CShooter*>(shooter));
}


/* bullet->Delete() :
 * ���� ����� ������ �ִ� Non_Node�迭�� �����͸� �����ϱ����� */
void CObjectManager::RemoveAllBullet()
{											
	for (auto bullet : m_BulletList)
	{
		if (bullet->HasPointer()) 
			bullet->Delete();
	}
}

/* shooter->Delete() :
 * ���� ����� ������ �ִ� Non_Node�迭�� �����͸� �����ϱ�����*/
void CObjectManager::RemoveAllShooter()
{
	for (auto shooter : m_ShooterList)
	{						
		if (shooter->HasPointer())
			shooter->Delete();
	}
}

void CObjectManager::RemoveAllObject()
{
	RemoveAllBullet();
	RemoveAllShooter();
}

void CObjectManager::Execute(float delta)
{
	CItemManager::Instance()->Execute(delta);
	CStageManager::Instance()->Execute(delta);
	for (auto bullet : m_BulletList)
	{
		if (bullet->IsAlive()) {
			bullet->Execute(delta);
		}
	}
	
	for (auto shooter : m_ShooterList)
	{
		if (shooter->IsAlive()) {
			shooter->Execute(delta);
		}
	}

	m_Planet->Execute();
	m_Player->Execute(delta);
}

void CObjectManager::RotationObject(int dir)
{
	for (auto bullet : m_BulletList)
	{
		if (bullet->IsAlive()) {
			bullet->Rotation(dir);
		}
	}
	m_Planet->Rotation(-dir);
	m_Player->Rotation(dir);
}