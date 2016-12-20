#include "ObjectManager.h"
#include "Planet.h"
#include "Player.h"
#include "BackGround.h"
#include "ItemManager.h"
#include "BulletCreator.h"
#include "Rocket.h"
#include "Bullet/Bullet.h"
#include "../AI/States/GameStates.h"
#include "../Scene/GameScene.h"
#include "../DataManager/UserDataManager.h"
#include <algorithm>

CObjectManager::CObjectManager()
: m_fStageTime(0.f)
, m_IsGamePause(true)
, m_RotationSpeed(0.f)
, m_RotateAcceleration(0.f)
, m_BulletCreator(nullptr)
, m_Planet(nullptr)
, m_Player(nullptr)
, m_Rocket(nullptr)
, m_Background(nullptr)
, m_SpeedController(nullptr)
, m_ItemManager(CItemManager::Instance())
, m_fDelta(0.f)
{
//    m_FSM = std::shared_ptr<CStateMachine<CObjectManager>>(new CStateMachine<CObjectManager>(this),
//                                                           [=](CStateMachine<CObjectManager>* fsm){
//        delete fsm;
//    });
//    this->ChangeState(CNormalState::Instance());
    
    m_SpeedController = Node::create();
    if(CGameScene::getGameScene()){
        m_SpeedController->setPositionX(BULLETCREATOR::ROTATION_SPEED);
        CGameScene::getGameScene()->addChild(m_SpeedController);
    }
}

CObjectManager* CObjectManager::Instance()
{
	static CObjectManager instance;
	return &instance;
}

void CObjectManager::Clear()
{
    m_RotateAcceleration = 0.f;
	m_fStageTime = 0.f;
	m_IsGamePause = true;
    m_BulletCreator->Clear();
    m_Planet->Clear();
    m_Player->Clear();
    this->ReturnToMemoryBlockAll();
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

void CObjectManager::Execute(float delta)
{
    m_fDelta = delta;
//    m_FSM->Execute(delta);
    
    this->inGameUpdate();
    this->inMenuUpdate();
}

void CObjectManager::RotationObject(float dir)
{
    if (m_IsGamePause) return;
    
    m_RotationSpeed = (dir * (m_SpeedController->getPositionX() * m_fDelta));
    
    this->bulletListRotate();
    m_BulletCreator->setRotationAngle(m_RotationSpeed);
    m_Planet->Rotation(-m_RotationSpeed);
    m_Player->Rotation(dir, m_fDelta);
}

void CObjectManager::SpeedControl(float duration, float speed)
{
    if(m_SpeedController)
    {
        m_SpeedController->runAction(MoveTo::create(duration, Vec2(speed, 0)));
    }
}

void CObjectManager::ChangeCharacter()
{
    auto index = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::CHARACTER);
    m_CharacterParam = CCharacterDataManager::Instance()->getCharacterInfoByIndex(index);
    
    if(m_Player)
        m_Player->setCharacterParam(m_CharacterParam);
    
    if(m_Planet)
        m_Planet->setPlanetTexture(m_CharacterParam._planetTextureName);
    
    if(m_BulletCreator)
        m_BulletCreator->setCharacterInfo(m_CharacterParam);
}

void CObjectManager::StartBonusTime()
{
    // Rotation speed down
    CObjectManager::Instance()->SpeedControl(1.5f, 0);
    m_Rocket->
}

void CObjectManager::EndBonusTime()
{
    // Rotation speed up to origin
    CObjectManager::Instance()->SpeedControl(1.0f, BULLETCREATOR::ROTATION_SPEED);
}

void CObjectManager::RemoveAllObject()
{
#if(USE_MEMORY_POOLING)
	removeAllBullet();
#endif
}

void CObjectManager::ReturnToMemoryBlockAll()
{
    for(auto it : m_BulletList)
    {
        if(it->IsAlive())
            it->ReturnToMemoryBlock();
    }
}

void CObjectManager::createBulletByTimer(float delta)
{
    m_fStageTime += delta;
	if (m_fStageTime < BULLETCREATOR::PATTERN_PADDING_LIMIT) return;

	if (!m_BulletCreator->getIsRunning()) {
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

void CObjectManager::inGameUpdate()
{
    if (m_IsGamePause) return;
    
    this->RotationObject(1);
    this->createBulletByTimer(m_fDelta);
    m_ItemManager->Execute(m_fDelta);
    m_BulletCreator->Update(m_fDelta);
    m_Player->Execute(m_fDelta);
    this->bulletListExecute();
}

void CObjectManager::inMenuUpdate()
{
    m_Rocket->Execute(m_fDelta);
}

void CObjectManager::inBonusGameUpdate()
{
    
}

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

void CObjectManager::bulletListExecute()
{
    for (auto bullet : m_BulletList)
    {
        if (bullet->IsAlive()) {
            bullet->Execute(m_fDelta);
        }
    }
}

void CObjectManager::bulletListRotate()
{
    for (auto bullet : m_BulletList)
    {
        if (bullet->IsAlive()) {
            bullet->Rotation(m_RotationSpeed);
        }
    }
}