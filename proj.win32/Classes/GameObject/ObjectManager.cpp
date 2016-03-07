#include "ObjectManager.h"
#include "Mover.h"
#include "Planet.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "../Scene/GameScene.h"
#include "Shooter\Shooter.h"
CObjectManager::CObjectManager()
{
}

CObjectManager::~CObjectManager()
{
}

CObjectManager* CObjectManager::Instance()
{
	static CObjectManager instance;
	return &instance;
}

void CObjectManager::ExitGame()
{
	for (auto enemy : m_EnemyList)
	{
		if (!enemy->IsAlive())
			enemy->release();
	}
	m_EnemyList.clear();
	for (auto bullet : m_BulletList)
	{
		bullet->release();
	}
	m_BulletList.clear();
	/*for (auto item : m_ItemList)
	{
	if (item->IsAlive()) {
	item->ex
	}
	}*/
	//m_Planet->release();
	//m_Player->release();
}

void CObjectManager::CreateBulletList(size_t count, size_t size)
{
	m_BulletSize = size;
	while (count--)
	{
		CBullet* temp = (CBullet*)new char[m_BulletSize];
		temp->setAlive(false);
		m_BulletList.emplace_back(temp);
		temp = (CBullet*)new(std::nothrow)CBullet();
		temp->autorelease();
		// operator new이다.  operator new에서 pool에 저장한 현재 메모리를 가져오기 때문에 
		// 결과적으로는 위의 temp나 여기서 받은 temp나 같은 메모리이다. 하지만 
		// 이렇게 pool에서 다시 가져온 메모리를 사용하는 이유는 초기화된 temp를 
		// 미리 gameScene에 addChild하기 위해서 이다. addChild를 미리 하는 이유는 
		// Cocos2d_x의 특성상 addChild된 오브젝트만 그려주기 때문이다.
		// 그냥 위에서 받은 char*형 temp는 생성자 초기화가 안되어 있어 모든 변수에 쓰레기값이 들어있다.
		// 때문에 AddChild가 되지 않는다.
		CGameScene::getGameScene()->addChild(temp);
	}
	// 오브젝트 풀에서 메모리를 가져오면 alive가 true가 됨
	// 때문에 초기 상태인 false로 바꿔 줘야 한다.
	for (auto bullet : m_BulletList)
	{
		bullet->setAlive(false);
	}
}

void CObjectManager::CreateItemList(size_t count, size_t size)
{

}

void CObjectManager::CreateEnemyList(size_t count, size_t size)
{
	m_EnemySize = size;
	while (count--)
	{
		CEnemy* temp = (CEnemy*)new char[m_EnemySize];
		temp->setAlive(false);
		m_EnemyList.emplace_back(temp);
		temp = (CEnemy*)new(std::nothrow)CEnemy();
		temp->autorelease();
		// operator new이다.  operator new에서 pool에 저장한 현재 메모리를 가져오기 때문에 
		// 결과적으로는 위의 temp나 여기서 받은 temp나 같은 메모리이다. 하지만 
		// 이렇게 pool에서 다시 가져온 메모리를 사용하는 이유는 초기화된 temp를 
		// 미리 gameScene에 addChild하기 위해서 이다. addChild를 미리 하는 이유는 
		// Cocos2d_x의 특성상 addChild된 오브젝트만 그려주기 때문이다.
		// 그냥 위에서 받은 char*형 temp는 생성자 초기화가 안되어 있어 모든 변수에 쓰레기값이 들어있다.
		// 때문에 AddChild가 되지 않는다.
		CGameScene::getGameScene()->addChild(temp);
	}
	// 오브젝트 풀에서 메모리를 가져오면 alive가 true가 됨
	// 때문에 초기 상태인 false로 바꿔 줘야 한다.
	for (auto enemy : m_EnemyList)
	{
		enemy->setAlive(false);
	}
}

CBullet* CObjectManager::BulletNew()
{
	for(auto bullet : m_BulletList)
	{
		if (!bullet->IsAlive()) { 
			bullet->setAlive(true);
			return bullet;
		}
	}
	//CCASSERT(false, "BULLET LIST OVERFLOWED");
	CBullet* tempBullet = (CBullet*)new char[m_BulletSize];
	tempBullet->setAlive(false);
	m_BulletList.emplace_back(tempBullet);
	tempBullet = (CBullet*)new(std::nothrow)CBullet();
	// operator new이다.  operator new(size_t)에서 pool에 저장한 현재 메모리를 가져오기 때문에 
	// 결과적으로는 위의 temp나 여기서 받은 temp나 같은 메모리이다. 하지만 
	// 이렇게 pool에서 다시 가져온 메모리를 사용하는 이유는 Bullet 생성자로 초기화된 temp를 
	// 미리 gameScene에 addChild하기 위해서 이다. addChild를 미리 하는 이유는 
	// Cocos2d_x의 특성상 addChild된 오브젝트만 그려주기 때문
	// 그냥 위에서 받은 char*형 temp는 생성자 초기화가 안되어 있어 모든 변수에 쓰레기값이 들어있다.
	// 때문에 AddChild가 되지 않는다.
	CGameScene::getGameScene()->addChild(tempBullet);
	return tempBullet;
}

CEnemy* CObjectManager::EnemyNew()
{
	for (auto enemy : m_EnemyList)
	{
		if (!enemy->IsAlive()) {
			enemy->setAlive(true);
			return enemy;
		}
	}
	CCASSERT(false, "ENEMY LIST OVERFLOWED");
	
	CEnemy* tempEnemy = (CEnemy*)new char[m_EnemySize];
	tempEnemy->setAlive(false);
	m_EnemyList.emplace_back(tempEnemy);
	tempEnemy = (CEnemy*)new(std::nothrow)CEnemy();
	// operator new이다.  operator new(size_t)에서 pool에 저장한 현재 메모리를 가져오기 때문에 
	// 결과적으로는 위의 temp나 여기서 받은 temp나 같은 메모리이다. 하지만 
	// 이렇게 pool에서 다시 가져온 메모리를 사용하는 이유는 Enemy 생성자로 초기화된 temp를 
	// 미리 gameScene에 addChild하기 위해서 이다. addChild를 미리 하는 이유는 
	// Cocos2d_x의 특성상 addChild된 오브젝트만 그려주기 때문
	// 그냥 위에서 받은 char*형 temp는 생성자 초기화가 안되어 있어 모든 변수에 쓰레기값이 들어있다.
	// 때문에 AddChild가 되지 않는다.
	CGameScene::getGameScene()->addChild(tempEnemy);
	return tempEnemy;
}

void CObjectManager::ObjectDelete(CMover* object)
{
	object->setVisible(false);
	object->setAlive(false);
}

void CObjectManager::EnemyDeleteAll()
{
	for (auto enemy : m_EnemyList)
	{
		enemy->setVisible(false);
		enemy->setAlive(false);
	}
}

void CObjectManager::BulletDeleteAll()
{
	for (auto bullet : m_BulletList)
	{
		bullet->setVisible(false);
		bullet->setAlive(false);
	}
}

void CObjectManager::Execute(float delta)
{
	for (auto enemy : m_EnemyList)
	{
		if (enemy->IsAlive()) {
			enemy->Execute(delta);
		}
	}
	for (auto bullet : m_BulletList)
	{
		if (bullet->IsAlive()) {
			bullet->setVisible(true);
			bullet->Execute(delta);
		}
	}
	/*for (auto item : m_ItemList)
	{
		if (item->IsAlive()) {
			item->ex
		}
	}*/
	
	m_Planet->Execute();
	//m_Player->Execute();
}