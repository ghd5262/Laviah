#include "ObjectManager.h"
#include "Planet.h"
#include "Player.h"
#include "BackGround.h"
#include "ItemManager.h"
#include "BulletCreator.h"
#include "Rocket.h"
#include "ItemRange.h"
#include "Bullet/Bullet.h"
#include "../AI/States/GameStates.h"
#include "../AI/States/RocketStates.h"
#include "../Scene/GameScene.h"
#include "../DataManager/UserDataManager.h"
#include <algorithm>

CObjectManager::CObjectManager()
: m_PatternTimer(0.f)
, m_LevelTimer(0.f)
, m_IsGamePause(true)
, m_RotationSpeed(0.f)
, m_BulletCreator(nullptr)
, m_Planet(nullptr)
, m_Player(nullptr)
, m_Rocket(nullptr)
, m_Background(nullptr)
, m_BarrierItemRange(nullptr)
, m_StarItemRange(nullptr)
, m_SpeedController(nullptr)
, m_CharacterParam(nullptr)
, m_ItemManager(CItemManager::Instance())
, m_PatternManager(CBulletPatternDataManager::Instance())
, m_Delta(0.f)
, m_GameLevel(0)
{
//    m_FSM = std::shared_ptr<CStateMachine<CObjectManager>>(new CStateMachine<CObjectManager>(this),
//                                                           [=](CStateMachine<CObjectManager>* fsm){
//        delete fsm;
//    });
//    this->ChangeState(CNormalState::Instance());
    
    m_BulletList.reserve(1000);
    
    m_SpeedController = Node::create();
    if(CGameScene::getGameScene()){
        CGameScene::getGameScene()->addChild(m_SpeedController);
    }
    
    m_LevelList.emplace_back(sLEVEL_BALANCE(20,     1));
    m_LevelList.emplace_back(sLEVEL_BALANCE(40,     2));
    m_LevelList.emplace_back(sLEVEL_BALANCE(60,     3, true));
    m_LevelList.emplace_back(sLEVEL_BALANCE(80,     3));
    m_LevelList.emplace_back(sLEVEL_BALANCE(120,    4, true));
    m_LevelList.emplace_back(sLEVEL_BALANCE(160,    4));
    m_LevelList.emplace_back(sLEVEL_BALANCE(240,    5, true));
    m_LevelList.emplace_back(sLEVEL_BALANCE(280,    5));
//    m_LevelList.emplace_back(sLEVEL_BALANCE(380,    6, true));
    m_LevelList.emplace_back(sLEVEL_BALANCE(300,    6));
    m_LevelList.emplace_back(sLEVEL_BALANCE(400,    7, true));
    m_LevelList.emplace_back(sLEVEL_BALANCE(440,    7));
}

CObjectManager* CObjectManager::Instance()
{
	static CObjectManager instance;
	return &instance;
}

void CObjectManager::Clear()
{
	m_PatternTimer = 0.f;
    m_LevelTimer = 0.f;
    m_GameLevel = 0;
    m_SpeedController->setPositionX(0.f);
	m_IsGamePause = true;
    m_BulletCreator->Clear();
    m_Planet->Clear();
    m_Player->Clear();
	m_BarrierItemRange->Clear();
	m_StarItemRange->Clear();
	m_CoinItemRange->Clear();

    this->ReturnToMemoryBlockAll();
//	this->EndBonusTime();
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
    m_Delta = delta;
    
//    m_FSM->Execute(delta);
    
    this->inGameUpdate();
    this->inMenuUpdate();
}

void CObjectManager::RotationObject(float dir)
{
    if (m_IsGamePause) return;
    
    m_RotationSpeed = (dir * (m_SpeedController->getPositionX() * m_Delta));
    
    this->bulletListRotate();
    m_BulletCreator->Rotation(m_RotationSpeed);
    m_Planet->Rotation(-m_RotationSpeed);
	m_Player->Rotation(m_RotationSpeed);
}

void CObjectManager::BonusTimeTouchEvent(float dir)
{
    if (m_IsGamePause) return;
    
    m_RotationSpeed = (dir * (m_SpeedController->getPositionX() * m_Delta));
    
    m_Rocket->Fly(m_RotationSpeed);
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
    m_CharacterParam = CCharacterDataManager::Instance()->getCharacterByIndex(index);
    
    if(m_Player)
        m_Player->setCharacterParam(m_CharacterParam);
    
    if(m_Planet)
        m_Planet->setPlanetTexture(m_CharacterParam->_planetTextureName);
    
    if(m_BulletCreator)
        m_BulletCreator->setCharacterInfo(m_CharacterParam);
}

void CObjectManager::StartBonusTime()
{
    // Rotation speed down
    this->SpeedControl(1.5f, 0);
	this->ReturnToMemoryBlockAll();
    m_Rocket->BonusTimeBegan();
	m_BulletCreator->Clear();
	m_BulletCreator->setRotationAngle(0.f);
    CGameScene::getGameScene()->BonusTimeStart();
}

void CObjectManager::EndBonusTime()
{
    // Rotation speed up to origin
    this->SpeedControl(1.0f, BULLETCREATOR::ROTATION_SPEED);
	this->ReturnToMemoryBlockAll();
    m_Rocket->BonusTimeEnd();
	m_BulletCreator->Clear();
	m_BulletCreator->setRotationAngle(0.f);
    CGameScene::getGameScene()->BonusTimeEnd();
}

void CObjectManager::ZoomIn()
{
    this->zoom(m_Planet, PLANET_DEFINE::ZOOMIN_POS, PLANET_DEFINE::ZOOMIN_SIZE);
    this->zoom(m_Player, PLAYER_DEFINE::ZOOMIN_POS, PLAYER_DEFINE::ZOOMIN_SIZE);
    m_Rocket->ZoomIn();
    m_Rocket->setVisible(true);
    m_Player->setVisible(true);
}

void CObjectManager::ZoomIn2()
{
    this->zoom(m_Planet, PLANET_DEFINE::ZOOMIN_2_POS, PLANET_DEFINE::ZOOMIN_2_SIZE);
    this->zoom(m_Player, PLAYER_DEFINE::ZOOMIN_2_POS, PLAYER_DEFINE::ZOOMIN_2_SIZE);
    m_Rocket->setVisible(false);
}

void CObjectManager::ZoomOut()
{
    this->zoom(m_Planet, PLANET_DEFINE::ZOOMOUT_POS, PLANET_DEFINE::SCALE_SIZE);
    this->zoom(m_Player, PLAYER_DEFINE::ZOOMOUT_POS, PLAYER_DEFINE::ZOOMOUT_SIZE);
    m_Rocket->ZoomOut();
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
    m_PatternTimer += delta;
	if (m_PatternTimer < BULLETCREATOR::PATTERN_PADDING_LIMIT) return;

	if (!m_BulletCreator->getIsRunning()) {
		if (1)
		{
			if (CItemManager::Instance()->isCurrentItem(eITEM_FLAG_bonustime))
				m_BulletCreator->setPattern(m_PatternManager->getRandomBonusTimePattern());
            else{
                auto level = m_LevelList.at(m_GameLevel)._level;
                auto below = m_LevelList.at(m_GameLevel)._below;
                auto data = m_PatternManager->getRandomNormalPatternByLevel(level, below);
                m_BulletCreator->setPattern(data);
//               m_BulletCreator->setPattern("pattern_32");
//               m_BulletCreator->setPattern(0);
            }
        }
		else
		{
            auto testPattern = m_PatternManager->getTestPattern();
			if (testPattern != nullptr){
				m_BulletCreator->setPattern(testPattern);
			}
		}
	}

	m_PatternTimer = 0.f;
}

void CObjectManager::inGameUpdate()
{
    m_Player->ParticleVisible(!m_IsGamePause); //;;;
    if (m_IsGamePause) return;
    
    this->RotationObject(1);
    this->createBulletByTimer(m_Delta);
    m_ItemManager->Execute(m_Delta);
    m_BulletCreator->Update(m_Delta);
    m_Player->Execute(m_Delta);
    this->bulletListExecute();
    this->setGameLevelByTimer();
}

void CObjectManager::inMenuUpdate()
{
    m_Rocket->Execute(m_Delta);
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
            bullet->Execute(m_Delta);
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

void CObjectManager::setGameLevelByTimer()
{
    if(m_LevelList.size() <= m_GameLevel) return;
    
    m_LevelTimer += m_Delta;
    if(m_LevelList.at(m_GameLevel)._time < m_LevelTimer)
    {
        m_GameLevel++;
    }
}

void CObjectManager::zoom(cocos2d::Node* obj,
                          cocos2d::Vec2 zoomPos,
                          float zoomSize)
{
    auto scaleAction = ScaleTo::create(1.2f, zoomSize);
    auto moveAction  = MoveTo::create(1.2f,  zoomPos);
    auto spawnAction = Spawn::createWithTwoActions(scaleAction, moveAction);
    auto exponential = EaseExponentialInOut::create(spawnAction);
    obj->runAction(exponential);
}