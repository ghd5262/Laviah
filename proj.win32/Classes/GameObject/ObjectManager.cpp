#include "ObjectManager.h"
#include "Planet.h"
#include "Player.h"
#include "BackGround.h"
#include "ItemManager.h"
#include "BulletCreator.h"
#include "Bullet/Bullet.h"
#include "Shooter/ShooterLegacy.h"
#include "../AI/States/StageStates.h"
#include "../DataManager/StageDataManager.h"
#include "../Scene/GameScene.h"
#include <algorithm>

CObjectManager::CObjectManager()
: m_fStageTime(0.f)
, m_IsGamePause(true)
, m_IsAbleRotation(false)
, m_fRotateAcceleration(0.f)
, m_BulletCreator(nullptr)
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
    m_fRotateAcceleration = 0.f;
	m_fStageTime = 0.f;
	m_IsGamePause = true;
	m_IsAbleRotation = false;
}

#if(USE_MEMORY_POOLING)
void CObjectManager::AddBullet(void* bullet)
{
	m_BulletList.emplace_back(static_cast<CBullet*>(bullet));
}
#else
void CObjectManager::AddBullet(CBullet* bullet)
{
    m_BulletList.emplace_back(bullet);
}
#endif

void CObjectManager::AddShooter(void* shooter)
{
	//m_ShooterList.emplace_back(shooter);
	m_ShooterList.emplace_back(static_cast<CShooterLegacy*>(shooter));
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

void CObjectManager::Auto_ReturnToMemoryBlock()
{

}

void CObjectManager::RemoveAllObject()
{
#if(USE_MEMORY_POOLING)
	RemoveAllBullet();
#endif
	RemoveAllShooter();
	Clear();
}

void CObjectManager::CreateShooterByTimer()
{
	if (m_fStageTime < BULLETCREATOR::PATTERN_PADDING_LIMIT) return;

	if (!m_BulletCreator->getIsRunning())	{
		if (1)
		{
			m_BulletCreator->setPattern(CBulletPatternDataManager::Instance()->getRandomPattern()->_patternName);
//			m_BulletCreator->setPattern("pattern_7");
		}
		else
		{
			auto testPattern = CBulletPatternDataManager::Instance()->getTestPattern();
//            auto patternData = const_cast<sBULLET_PATTERN*>(testPattern);
//            std::string pattern = "                 65211";
            
//            std::copy(std::begin(pattern), std::end(pattern), std::begin(patternData->_pattern));
//            patternData->_width = pattern.size();
//            patternData->_height = 1;
//            patternData->_index = 10000;
//            patternData->_patternName = "testPattern";
//            patternData->_widthPadding = 5.f;
            
			if (testPattern != nullptr){
				m_BulletCreator->setPattern(testPattern);
			}
		}
	}

	m_fStageTime = 0.f;
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

//	for (auto shooter : m_ShooterList)
//	{
//		if (shooter->IsAlive()) {
//			shooter->Execute(delta);
//		}
//	}
    m_BulletCreator->Update(delta);
    m_Planet->Execute();
    m_Player->Execute(delta);
	this->RotationObject(1);
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
			bullet->Rotation(dir + (dir * m_fRotateAcceleration), m_fDelta);
		}
	}
    
//    for (auto shooter : m_ShooterList)
//    {
//        if (shooter->IsAlive()) {
//            shooter->Rotation(dir + (dir * m_fRotateAcceleration), m_fDelta);
//        }
//    }
    
    m_BulletCreator->setRotationAngle(dir + (dir * m_fRotateAcceleration), m_fDelta);
    m_Planet->Rotation(-dir + (-dir * m_fRotateAcceleration), m_fDelta);
	m_Player->Rotation(dir, m_fDelta);
}

void CObjectManager::ShooterPause()
{
	m_BulletCreator->Pause();
	//for (auto shooter : m_ShooterList)
	//{
	//	if (shooter->IsAlive()) {
	//		//shooter->setAlive(false);
	//		shooter->setShooterPause(true);
	//	}
	//}
}

void CObjectManager::ShooterResume()
{
	m_BulletCreator->Resume();
	//for (auto shooter : m_ShooterList)
	//{
	//	if (shooter->IsAlive()) {
	//		//shooter->setAlive(true);
	//		shooter->setShooterPause(false);
	//	}
	//}
}

void CObjectManager::RotateAccelerationUpdate(float value){
    // value가 음수
    if(value < 0.f)
    {
        if(m_fRotateAcceleration + value > 0)
            m_fRotateAcceleration += value;
        else
            m_fRotateAcceleration = 0;
    }
    else{
        if(m_fRotateAcceleration + value < ROTATE_ACCEL_MAX)
            m_fRotateAcceleration += value;
        else
            m_fRotateAcceleration = ROTATE_ACCEL_MAX;
    }
}

void CObjectManager::removeBulletFromList(CBullet* bullet)
{
    int idx = 0;
    
    for(auto it : m_BulletList)
    {
        if(it == bullet)
        {
            m_BulletList.erase(m_BulletList.begin() + idx);
        }
        idx++;
    }
}
