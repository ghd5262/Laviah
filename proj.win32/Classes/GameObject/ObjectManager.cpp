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
 * 게임 종료시 가지고 있는 Non_Node계열의 포인터를 해제하기위해 */
void CObjectManager::RemoveAllBullet()
{											
	for (auto bullet : m_BulletList)
	{
		if (bullet->HasPointer()) 
			bullet->Delete();
	}
}

/* shooter->Delete() :
 * 게임 종료시 가지고 있는 Non_Node계열의 포인터를 해제하기위해*/
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