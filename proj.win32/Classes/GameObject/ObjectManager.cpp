#include "ObjectManager.h"
#include "Planet.h"
#include "Player.h"
#include "ItemManager.h"
#include "Bullet/Bullet.h"
#include "Shooter/Shooter.h"
#include "../AI/States/StageStates.h"
#include "../DataManager/StageDataManager.h"
#include "../Scene/GameScene.h"
CObjectManager::CObjectManager()
	: m_fStageTime(0.f)
	, m_IsGamePause(true)
	, m_IsAbleRotation(false)
{
	m_FSM = new CStateMachine<CObjectManager>(this);

	if (m_FSM != nullptr){
		m_FSM->ChangeState(CGameCountDownState::Instance());
	}

	m_StageList = CStageDataManager::Instance()->getStageList();
}

CObjectManager* CObjectManager::Instance()
{
	static CObjectManager instance;
	return &instance;
}

void CObjectManager::Clear()
{
	m_BulletList.clear();
	m_ShooterList.clear();
	m_CurrentShooterIdx = 0;
	m_fStageTime = 0.f;
	m_IsGamePause = true;
	m_IsAbleRotation = false;
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

void CObjectManager::Auto_ReturnToMemoryBlock()
{

}

void CObjectManager::RemoveAllObject()
{
	RemoveAllBullet();
	RemoveAllShooter();
	m_BulletList.clear();
	m_ShooterList.clear();
}

void CObjectManager::CreateShooterByTimer()
{
	if (m_CurrentShooterIdx < m_StageList->size() && m_StageList->at(m_CurrentShooterIdx)._fStartTime < m_fStageTime)
	{
		if (m_StageList->at(m_CurrentShooterIdx)._ShooterName == "RandomShooter")
		{
			CGameScene::getGameScene()->addChild(
				CRandomShooter::create(m_StageList->at(m_CurrentShooterIdx)));
		}
		else if (m_StageList->at(m_CurrentShooterIdx)._ShooterName == "PatternShooter")
		{
			CGameScene::getGameScene()->addChild(
				CPatternShooter::create(m_StageList->at(m_CurrentShooterIdx)));
		}
        else if (m_StageList->at(m_CurrentShooterIdx)._ShooterName == "all pause")
        {
            ShooterPause();
        }
        else if (m_StageList->at(m_CurrentShooterIdx)._ShooterName == "all resume")
        {
            ShooterResume();
        }
        
		CCLOG("StageSize : %d Index : %d Shooter : %s Pattern : %s",static_cast<int>(m_StageList->size()), m_CurrentShooterIdx, m_StageList->at(m_CurrentShooterIdx)._ShooterName.c_str(), m_StageList->at(m_CurrentShooterIdx)._PatternName.c_str());

		m_CurrentShooterIdx++;
	}
}

void CObjectManager::ExecuteAllObject(float delta)
{
	if (m_IsGamePause)
		return;

	m_IsAbleRotation = true;
	m_fDelta = delta;
	m_fStageTime += delta;
	CreateShooterByTimer();
	CItemManager::Instance()->Execute(delta);
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

void CObjectManager::Execute(float delta)
{
	m_FSM->Execute(delta);
}

void CObjectManager::RotationObject(float dir)
{
	if (!m_IsAbleRotation)
		return;

	for (auto bullet : m_BulletList)
	{
		if (bullet->IsAlive()) {
			bullet->Rotation(dir, m_fDelta);
		}
	}
	m_Planet->Rotation(-dir, m_fDelta);
	m_Player->Rotation(dir, m_fDelta);
}

void CObjectManager::ShooterPause()
{
	for (auto shooter : m_ShooterList)
	{
		if (shooter->IsAlive()) {
			shooter->setAlive(false);
		}
	}
}

void CObjectManager::ShooterResume()
{
	for (auto shooter : m_ShooterList)
	{
		if (shooter->IsAlive()) {
			shooter->setAlive(true);
		}
	}
}

