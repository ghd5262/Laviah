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
#include "../MyUI/TutorialLayer.hpp"
#include "../MyUI/MyButton.h"
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
, m_GiantSpeed(1.f)
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
    
    auto insertLevel = [=](sLEVEL_BALANCE data){
        m_LevelList.emplace_back(data);
    };
    
////    insertLevel(sLEVEL_BALANCE(20,  50.f, 1, 1.f, 0, Vec2(540.f, 672.f)));
//    insertLevel(sLEVEL_BALANCE(40,  55.f,  1, 1.0f, 0, Vec2(540.f, 672.f)));
////    insertLevel(sLEVEL_BALANCE(60,  60.f, 2, 1.f, 0, Vec2(540.f, 672.f)));
//    insertLevel(sLEVEL_BALANCE(80,  65.f,  2, 1.0f, 0, Vec2(540.f, 672.f)));
////    insertLevel(sLEVEL_BALANCE(100, 70.f, 3, 1.f, 0, Vec2(540.f, 672.f)));
//    insertLevel(sLEVEL_BALANCE(120, 75.f,  3, 1.0f, 0, Vec2(540.f, 672.f)));
////    insertLevel(sLEVEL_BALANCE(140, 80.f, 4, 1.f, 0, Vec2(540.f, 672.f)));
////    insertLevel(sLEVEL_BALANCE(160, 85.f, 4, 1.f, 0, Vec2(540.f, 672.f)));
//    insertLevel(sLEVEL_BALANCE(180, 90.f,  4, 1.0f, 0, Vec2(540.f, 672.f)));
////    insertLevel(sLEVEL_BALANCE(200, 95.f, 5, 1.f, 0, Vec2(540.f, 672.f)));
////    insertLevel(sLEVEL_BALANCE(220, 90.f, 4, 1.f, 0, Vec2(540.f, 672.f)));
////    insertLevel(sLEVEL_BALANCE(240, 90.f, 4, 1.f, 0, Vec2(540.f, 672.f)));
//    insertLevel(sLEVEL_BALANCE(260, 100.f, 5, 1.0f, 0, Vec2(540.f, 672.f)));
//    insertLevel(sLEVEL_BALANCE(290, 120.f, 5, 0.8f, 0, Vec2(540.f, 672.f)));
//    insertLevel(sLEVEL_BALANCE(350, 90.f,  5, 1.2f, 0, Vec2(540.f, 672.f), true));
//    insertLevel(sLEVEL_BALANCE(410, 90.f,  5, 1.2f, 0, Vec2(540.f, 0.f)  , true));
//    insertLevel(sLEVEL_BALANCE(470, 90.f,  5, 1.0f, 0, Vec2(540.f, 672.f)));
//    insertLevel(sLEVEL_BALANCE(490, 100.f, 5, 1.0f, 90,Vec2(270.f, 960.f)));

    insertLevel(sLEVEL_BALANCE(20,  80.f,  1, 0.66f, 0, Vec2(540.f, 672.f)));
    
    insertLevel(sLEVEL_BALANCE(30,  90.f,  2, 0.66f, 0, Vec2(540.f, 672.f)));
    
    insertLevel(sLEVEL_BALANCE(40,  95.f,  3, 0.66f, 0, Vec2(540.f, 672.f)));
    
    insertLevel(sLEVEL_BALANCE(60,  100.f, 4, 0.66f, 0, Vec2(540.f, 672.f)));
    
    insertLevel(sLEVEL_BALANCE(80,  100.f, 5, 0.66f, 0, Vec2(540.f, 672.f)));
    insertLevel(sLEVEL_BALANCE(90,  100.f, 5, 0.45f, 0, Vec2(540.f, 672.f)));
    insertLevel(sLEVEL_BALANCE(100, 100.f, 5, 0.8f,  0, Vec2(540.f, 400.f)));
    
    insertLevel(sLEVEL_BALANCE(110, 110.f, 5, 0.66f, 0, Vec2(540.f, 672.f)));
    insertLevel(sLEVEL_BALANCE(120, 110.f, 5, 0.45f, 0, Vec2(540.f, 672.f)));
    insertLevel(sLEVEL_BALANCE(130, 110.f, 5, 0.8f,  0, Vec2(540.f, 400.f)));
    
    insertLevel(sLEVEL_BALANCE(140, 120.f, 5, 0.66f, 0, Vec2(540.f, 672.f)));
    insertLevel(sLEVEL_BALANCE(150, 120.f, 5, 0.45f, 0, Vec2(540.f, 672.f)));
    insertLevel(sLEVEL_BALANCE(160, 120.f, 5, 0.8f,  0, Vec2(540.f, 400.f)));
    
    insertLevel(sLEVEL_BALANCE(170, 120.f, 5, 0.8f,  0, Vec2(540.f, 100.f)));
    insertLevel(sLEVEL_BALANCE(190, 130.f, 5, 0.45f, 0, Vec2(540.f, 672.f)));
    insertLevel(sLEVEL_BALANCE(210, 130.f, 5, 0.66f, 0, Vec2(540.f, 672.f)));
    
    
//    m_LevelList.emplace_back(sLEVEL_BALANCE(20,  50,   1, 1.f , 0,   PLANET_DEFINE::GAME_POS));
//    m_LevelList.emplace_back(sLEVEL_BALANCE(40,  55.f, 2, 1.f , 90,  Vec2(270, 960)));
//    m_LevelList.emplace_back(sLEVEL_BALANCE(60,  60.f, 3, 1.f , 180, Vec2(540, 1248), true));
//    m_LevelList.emplace_back(sLEVEL_BALANCE(80,  65.f, 3, 1.f , 270, Vec2(810, 960)));
//    m_LevelList.emplace_back(sLEVEL_BALANCE(100, 70.f, 4, 1.f , 0,   PLANET_DEFINE::GAME_POS, true));
//    m_LevelList.emplace_back(sLEVEL_BALANCE(160, 75.f, 4, 1.2f, 0,   Vec2(540, 0)));
//    m_LevelList.emplace_back(sLEVEL_BALANCE(220, 75.f, 5, 1.f , 180, PLANET_DEFINE::GAME_POS, true));
//    m_LevelList.emplace_back(sLEVEL_BALANCE(240, 65.f, 5, 0.8f, 0,   PLANET_DEFINE::GAME_POS, true));
//    m_LevelList.emplace_back(sLEVEL_BALANCE(280, 70.f, 5, 1.2f, 0,   PLANET_DEFINE::GAME_POS));
//    m_LevelList.emplace_back(sLEVEL_BALANCE(300, 70.f, 6, 1.2f, 0,   PLANET_DEFINE::GAME_POS));
//    m_LevelList.emplace_back(sLEVEL_BALANCE(400, 75.f, 7, 1.2f , 0,  Vec2(540, 0), true));
//    m_LevelList.emplace_back(sLEVEL_BALANCE(440, 75.f, 7, 1.f, 215,  PLANET_DEFINE::GAME_POS));
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
//    this->setGameStateByLevel();
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
//    m_FSM->Execute(delta);
    
    this->inGameUpdate(delta);
    this->inMenuUpdate(delta);
}

void CObjectManager::RotationObject(float dir)
{
    if (m_IsGamePause) return;
    
    m_RotationSpeed *= dir;
    this->bulletListRotate();
    m_BulletCreator->Rotation(m_RotationSpeed * m_GiantSpeed);
    m_Planet->Rotation(-m_RotationSpeed * m_GiantSpeed);
	m_Player->Rotation(m_RotationSpeed * m_GiantSpeed);
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

void CObjectManager::ChangeRocket()
{
    auto index = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::ROCKET);
    m_RocketParam = CRocketDataManager::Instance()->getRocketByIndex(index);
    
    if(m_Rocket)
        m_Rocket->setRocketParam(m_RocketParam);
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
    this->zoom(CGameScene::getZoomLayer(),
               PLANET_DEFINE::MENU_POS,
               0,
               PLANET_DEFINE::MENU_SIZE,
               1.2f,
               true);
    m_Rocket->setVisible(true);
    m_Player->setVisible(true);
}

void CObjectManager::ZoomIn2()
{
    this->zoom(CGameScene::getZoomLayer(),
               PLANET_DEFINE::SELECT_POS,
               0,
               PLANET_DEFINE::SELECT_SIZE,
               1.2f,
               true);
    m_Rocket->setVisible(false);
}

void CObjectManager::ZoomInRank()
{
    this->zoom(CGameScene::getZoomLayer(),
               Vec2(PLANET_DEFINE::MENU_POS.x, -PLANET_DEFINE::MENU_POS.y),
               0,
               PLANET_DEFINE::MENU_SIZE,
               1.2f,
               true);
    
    m_Rocket->setVisible(false);
}

void CObjectManager::ZoomOut()
{
    this->zoom(CGameScene::getZoomLayer(),
               PLANET_DEFINE::GAME_POS,
               0,
               PLANET_DEFINE::GAME_SIZE,
               1.2f,
               true);
}

void CObjectManager::ZoomOut2()
{
    this->zoom(CGameScene::getZoomLayer(),
               PLANET_DEFINE::MENU_POS,
               0,
               0.7f,
               1.2f,
               true);
}

void CObjectManager::ZoomOutRank()
{
    this->ZoomIn();
    m_Rocket->setVisible(true);
}

void CObjectManager::GiantMode()
{
//    this->SpeedControl(0.5f, BULLETCREATOR::ROTATION_SPEED * 1.5f);
    
    m_GiantSpeed = 1.5f;
    //ver-1
    
    auto levelData = m_LevelList.at(m_GameLevel);
    this->zoom(CGameScene::getZoomLayer(), levelData._pos, levelData._angle, 0.45f, 1.f, true);
    
//    auto scaleAction1  = ScaleTo::create(1.f,  0.7f);
//    auto exponential1  = EaseExponentialInOut::create(scaleAction1);
//    exponential1->setTag(100);
//    CGameScene::getZoomLayer()->runAction(exponential1);
}

void CObjectManager::NormalMode()
{
    m_GiantSpeed = 1.f;
    //ver-1
    auto levelData = m_LevelList.at(m_GameLevel);
    this->zoom(CGameScene::getZoomLayer(), levelData._pos, levelData._angle, levelData._zoom, 1.f, true);
//    this->SpeedControl(1.f, levelData._speed);
}

void CObjectManager::setGameStateByLevel()
{
    //ver-1
    auto levelData = m_LevelList.at(m_GameLevel);
    this->zoom(CGameScene::getZoomLayer(), levelData._pos, levelData._angle, levelData._zoom, 8);
    this->SpeedControl(8.f, levelData._speed);
}

void CObjectManager::Shake(float interval,
                           float duration,
                           float speed,
                           float magnitude,
                           cocos2d::Node* owner,
                           cocos2d::Vec2 originPos)
{
    auto noise = [](int x, int y) {
        int n = x + y * 57;
        n = (n << 13) ^ n;
        return (1.0 - ((n * ((n * n * 15731) + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
    };
    
    //experiment more with these four values to get a rough or smooth effect!
    if(owner->isScheduled("Shake")) return;
    
    owner->scheduleOnce([=](float delta){
        owner->unschedule("Shake");
        owner->setPosition(originPos);
    }, duration, "ShakeLimit");
    
    owner->schedule([=](float dt) {
        float randomStart = random(-1000.0f, 1000.0f);
        float percentComplete =  0.5f;
        
        // We want to reduce the shake from full power to 0 starting half way through
        float damper = 1.0f - clampf(2.0f * percentComplete - 1.0f, 0.0f, 1.0f);
        
        // Calculate the noise parameter starting randomly and going as fast as speed allows
        float alpha = randomStart + speed * percentComplete;
        
        // map noise to [-1, 1]
        float x = noise(alpha, 0.0f) * 2.0f - 1.0f;
        float y = noise(0.0f, alpha) * 2.0f - 1.0f;
        
        x *= magnitude * damper;
        y *= magnitude * damper;
        
        owner->setPosition(Vec2(originPos + Vec2(x, y)));
    }, interval, CC_REPEAT_FOREVER, 0.f, "Shake");
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
//			if (CItemManager::Instance()->isCurrentItem(eITEM_FLAG_bonustime))
//				m_BulletCreator->setPattern(m_PatternManager->getRandomBonusTimePattern());
            if(!CTutorialLayer::Instance()->getIsRunning()){
                auto levelData = m_LevelList.at(m_GameLevel);
                
                auto level = levelData._level;
                auto below = levelData._below;
                auto data = m_PatternManager->getRandomNormalPatternByLevel(level, below);
                m_BulletCreator->setPattern(data);
//                this->zoom(CGameScene::getZoomLayer(), levelData._pos, levelData._angle, levelData._zoom, 8);
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

void CObjectManager::inGameUpdate(float delta)
{
    CObjectManager::Instance()->getPlayer()->ParticleVisible(!m_IsGamePause); //;;;

    if (m_IsGamePause) return;
    
    m_BulletCreator->setLineIntervalLimit((360.f / m_SpeedController->getPositionX()) / 25);
    m_RotationSpeed = ((m_SpeedController->getPositionX() * delta));
    m_Delta = MIN(delta * fabs(m_RotationSpeed), delta);
    
    this->RotationObject(1);
    this->createBulletByTimer(m_Delta);
    m_ItemManager->Execute(m_Delta);
    m_BulletCreator->Update(m_Delta);
    m_Player->Execute(m_Delta);
    m_Planet->Execute(m_Delta);
    this->bulletListExecute();
    this->setGameLevelByTimer();
}

void CObjectManager::inMenuUpdate(float delta)
{
    m_Rocket->Execute(delta);
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
            bullet->Rotation(m_RotationSpeed * m_GiantSpeed);
        }
    }
}

void CObjectManager::setGameLevelByTimer()
{
    m_LevelTimer += m_Delta;
    if(m_LevelList.at(m_GameLevel)._time < m_LevelTimer)
    {
        if(m_LevelList.size() > m_GameLevel+1){
            m_GameLevel++;
            this->setGameStateByLevel();
        }
    }
}

void CObjectManager::zoom(cocos2d::Node* obj,
                          cocos2d::Vec2 zoomPos,
                          float zoomAngle,
                          float zoomSize,
                          float duration,
                          bool force)
{
    if(force) obj->stopActionByTag(100);
    if(obj->getActionByTag(100)) return;
    
    auto scaleAction  = ScaleTo::create(duration,  zoomSize);
    auto moveAction   = MoveTo::create(duration,   zoomPos);
    auto rotateAction = RotateTo::create(duration, zoomAngle);
    auto spawnAction  = Spawn::create(moveAction, scaleAction , rotateAction, nullptr);
    auto exponential  = EaseExponentialInOut::create(spawnAction);
    exponential->setTag(100);
    obj->runAction(exponential);
//    spawnAction->setTag(100);
//    obj->runAction(spawnAction);
}

void CObjectManager::InitTutorialStep()
{
    auto createDelayStep = [=](float delay){
        CTutorialObject::create()
        ->addBeginListener([=](Node* sender){
            m_LevelTimer = 0.f;
        })
        ->addUpdateListener([=](float delta){
            if(m_LevelTimer > delay) CTutorialLayer::Instance()->NextStep();
        })
        ->setTouchEnable(false)
        ->build("first tutorial");
    };
    
    auto createMessageBoxStep = [=](std::string message){
        CTutorialObject::create()
        ->addTouchListener([=](Node* sender){
            CTutorialLayer::Instance()->NextStep();
        })
        ->addBeginListener([=](Node* sender){
            this->SpeedControl(0.5f, 0);
        })
        ->setTouchEnable(false)
        ->addMessageBox(message)
        ->build("first tutorial");
    };
    
    // 3초 후 다음 스텝
    createDelayStep(3.f);

    // 메시지 박스 출력 터치 시 다음 스텝
    createMessageBoxStep("별은 점수나 마찬가지에요.");
    
    // 별을 1개 이상 획득 시 다음 스텝
    CTutorialObject::create()
    ->addBeginListener([=](Node* sender){
        GLOBAL->STAR_COUNT = 0;
        GLOBAL->COLLISION_COUNT = 0;
        auto data = m_PatternManager->getTutorialPatternByIndex(0);
        m_BulletCreator->setPattern(data);
        m_BulletCreator->setIsFlip(false);
        this->SpeedControl(0.5f, BULLETCREATOR::ROTATION_SPEED);
    })
    ->addUpdateListener([=](float delta){
        if(GLOBAL->STAR_COUNT >= 6) {
            CTutorialLayer::Instance()->NextStep();
            return;
        }
    })
    ->setTouchEnable(false)
    ->build("first tutorial");
    
    
    // 메시지 박스 출력 터치 시 다음 스텝
    createMessageBoxStep("화면을 터치하면 반대방향으로 구를 수 있어요.");
    
    // 별을 1개 이상 획득 시 다음 스텝
    CTutorialObject::create()
    ->addBeginListener([=](Node* sender){
        GLOBAL->STAR_COUNT = 0;
        this->SpeedControl(0.5f, BULLETCREATOR::ROTATION_SPEED);
    })
    ->addUpdateListener([=](float delta){
        if(GLOBAL->STAR_COUNT >= 9) {
            CTutorialLayer::Instance()->NextStep();
            return;
        }
    })
    ->build("first tutorial");
    
    // 메시지 박스 출력 터치 시 다음 스텝
    createMessageBoxStep("게임 플레이에 도움이 되는 아이템을 획득할 수 있어요!");
    
    // 별 아이템 획득시 다음 스텝
    CTutorialObject::create()
    ->addBeginListener([=](Node* sender){
        GLOBAL->COLLISION_COUNT = 0;
        GLOBAL->STAR_ITEM_USE = 0;
        this->SpeedControl(0.5f, BULLETCREATOR::ROTATION_SPEED);
    })
    ->addUpdateListener([=](float delta){
        if(GLOBAL->STAR_ITEM_USE >= 1) {
            CTutorialLayer::Instance()->NextStep();
            return;
        }
    })
    ->build("first tutorial");
    
    // 메시지 박스 출력 터치 시 다음 스텝
    createMessageBoxStep("미사일은 빠르니까 특별히 조심해야해요!");
    
    // 미사일 피하면 다음 스텝
    CTutorialObject::create()
    ->addBeginListener([=](Node* sender){
        GLOBAL->COLLISION_COUNT = 0;
        this->SpeedControl(0.5f, BULLETCREATOR::ROTATION_SPEED);
    })
    ->addUpdateListener([=](float delta){
        if(GLOBAL->COLLISION_COUNT >= 6)
            CTutorialLayer::Instance()->NextStep();
    })
    ->build("first tutorial");
    
    
    // 1초 후 다음 스텝
    createDelayStep(1.f);
    
    // 메시지 박스 출력 터치 시 다음 스텝
    createMessageBoxStep("다른 방법으로 피할 수도 있어요.");
    
    // 미사일 피하면 다음 스텝
    CTutorialObject::create()
    ->addBeginListener([=](Node* sender){
        this->SpeedControl(0.5f, BULLETCREATOR::ROTATION_SPEED);
    })
    ->addUpdateListener([=](float delta){
        if(GLOBAL->STAR_ITEM_USE >= 1)
            CTutorialLayer::Instance()->NextStep();
    })
    ->build("first tutorial");
    
    
    // 코인 획득시 다음스텝
//    CTutorialObject::create()
//    ->addBeginListener([=](Node* sender){
//        this->SpeedControl(0.5f, BULLETCREATOR::ROTATION_SPEED);
//    })
//    ->addUpdateListener([=](float delta){
//        if(GLOBAL->COIN_COUNT >= 1) CTutorialLayer::Instance()->NextStep();
//    })
//    ->build("first tutorial");
//    
    // 메시지 박스 출력 터치 시 다음 스텝
    createMessageBoxStep("코인이에요! 무엇이든 구매하거나 업그레이드 할 수 있어요!");
    
    CTutorialLayer::Instance()->ChangeTutorial("first tutorial");
}

void CObjectManager::Share()
{
    auto screenSize = Director::getInstance()->getWinSize();
    
    auto rt = RenderTexture::create(screenSize.width, screenSize.height, Texture2D::PixelFormat::RGBA8888);
    rt->retain();
    rt->begin();
//    CGameScene::getGameScene()->visit();
    m_Background->visit();
    m_Planet->visit();
    m_Player->visit();
    m_Rocket->visit();
    rt->end();
    
    auto completionCallback = [=](RenderTexture* sender, const std::string& filename)
    {
        sender->release();
        auto picture = cocos2d::ui::Button::create(filename);
        picture->addClickEventListener([=](Ref* sender){
            picture->removeFromParent();
            Director::getInstance()->getTextureCache()->removeTextureForKey(filename);
        });
        picture->setPosition(screenSize / 2);
        picture->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        picture->setScale(0.5f);
        CGameScene::getGameScene()->addChild(picture, ZORDER::POPUP);
        
    };
    
    rt->saveToFile("screenshot.png", true, completionCallback);
}