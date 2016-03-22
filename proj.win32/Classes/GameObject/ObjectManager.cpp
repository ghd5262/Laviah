#include "ObjectManager.h"
#include "Mover.h"
#include "Planet.h"
#include "Player.h"
#include "Bullet.h"
#include "Bullet/PlayItem.h"
#include "Shooter/Shooter.h"
#include "../Scene/GameScene.h"
#include "../AI/States/ObjectStates.h"
#include "../AI/States/BulletStates.h"

CObjectManager::CObjectManager(){
	m_pStateMachine = std::shared_ptr<CStateMachine<CObjectManager>>
		(new CStateMachine<CObjectManager>(this), [](CStateMachine<CObjectManager>* fsm)
	{
		delete fsm;
	});
	if (m_pStateMachine != nullptr)
	{
		m_pStateMachine->setCurState(CNormalState::Instance());
		m_pStateMachine->setPreState(CNormalState::Instance());
		m_pStateMachine->ChangeState(CNormalState::Instance());
	}
}

CObjectManager::~CObjectManager(){}

CObjectManager* CObjectManager::Instance()
{
	static CObjectManager instance;
	return &instance;
}

void CObjectManager::AddBullet(void* bullet)
{
	m_BulletList.emplace_back(static_cast<CBullet*>(bullet));
}

void CObjectManager::AddEnemy(void* enemy)
{
	m_EnemyList.emplace_back(static_cast<CEnemy*>(enemy));
}


/* bullet->Delete() :
게임 종료시 가지고 있는 Non_Node계열의 포인터를 해제하기위해*/
void CObjectManager::RemoveAllBullet()
{											
	for (auto bullet : m_BulletList)
	{
		if (bullet->HasPointer()) 
			bullet->Delete();
	}
}

/* enemy->Delete() :
게임 종료시 가지고 있는 Non_Node계열의 포인터를 해제하기위해*/
void CObjectManager::RemoveAllEnemy()
{
	for (auto enemy : m_EnemyList)
	{						
		if (enemy->HasPointer())
			enemy->Delete();
	}
}

void CObjectManager::RemoveAllObject()
{
	RemoveAllBullet();
	RemoveAllEnemy();
}

void CObjectManager::Execute(float delta)
{
	for (auto bullet : m_BulletList)
	{
		if (bullet->IsAlive()) {
			bullet->Execute(delta);
		}
	}
	
	for (auto enemy : m_EnemyList)
	{
		if (enemy->IsAlive()) {
			enemy->Execute(delta);
		}
	}

	//m_Planet->Execute();
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

void CObjectManager::PlayerGetItem(eITEM_FLAG itemType)
{
	m_CurrentItems |= itemType;

	CGameScene::getGameScene()->scheduleOnce([this, itemType](float dt){
		this->FinishItemTimer(itemType);
	}, 10.0f, MakeString("Item_%d_Timer", static_cast<int>(itemType)));
}