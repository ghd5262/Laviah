#include "ObjectManager.h"
#include "Planet.h"
#include "Player.h"
#include "BackGround.h"
#include "ItemManager.h"
#include "BulletCreator.h"
#include "Bullet/Bullet.h"
#include "../AI/States/StageStates.h"
#include "../Scene/GameScene.h"
#include <algorithm>

CObjectManager::CObjectManager()
: m_fStageTime(0.f)
, m_IsGamePause(true)
, m_fRotateAcceleration(0.f)
, m_BulletCreator(nullptr)
{}

CObjectManager* CObjectManager::Instance()
{
	static CObjectManager instance;
	return &instance;
}

void CObjectManager::Clear()
{
    m_fRotateAcceleration = 0.f;
	m_fStageTime = 0.f;
	m_IsGamePause = true;
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

/* bullet->Delete() :
 * 게임 종료시 가지고 있는 Non_Node계열의 포인터를 해제하기위해 */
void CObjectManager::removeAllBullet()
{											
	for (auto bullet : m_BulletList)
	{
		if (bullet->HasPointer()) 
			bullet->Delete();
	}
    m_BulletList.clear();
}

void CObjectManager::RemoveAllObject()
{
#if(USE_MEMORY_POOLING)
	removeAllBullet();
#endif
}

void CObjectManager::createBulletByTimer(float delta)
{
    m_fStageTime += delta;
	if (m_fStageTime < BULLETCREATOR::PATTERN_PADDING_LIMIT) return;

	if (!m_BulletCreator->getIsRunning())	{
		if (1)
		{
			m_BulletCreator->setPattern(CBulletPatternDataManager::Instance()->getRandomPattern()->_patternName);
			//m_BulletCreator->setPattern("pattern_32");
		}
		else
		{
            auto testPattern = CBulletPatternDataManager::Instance()->getTestPattern();            
			if (testPattern != nullptr){
				m_BulletCreator->setPattern(testPattern);
			}
		}
	}

	m_fStageTime = 0.f;
}

void CObjectManager::Execute(float delta)
{
    if (m_IsGamePause) return;
    
    m_fDelta = delta;
    
    this->RotationObject(1);
    this->createBulletByTimer(delta);
    m_BulletCreator->Update(delta);
    
    CItemManager::Instance()->Execute(delta);
    
    m_Player->Execute(delta);
    
    
    for (auto bullet : m_BulletList)
    {
        if (bullet->IsAlive()) {
            bullet->Execute(delta);
        }
    }
}

void CObjectManager::RotationObject(float dir)
{
    if (m_IsGamePause) return;
    
	for (auto bullet : m_BulletList)
	{
		if (bullet->IsAlive()) {
			bullet->Rotation(dir + (dir * m_fRotateAcceleration), m_fDelta);
		}
	}
    
    m_BulletCreator->setRotationAngle(dir + (dir * m_fRotateAcceleration), m_fDelta);
    m_Planet->Rotation(-dir + (-dir * m_fRotateAcceleration), m_fDelta);
	m_Player->Rotation(dir, m_fDelta);
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