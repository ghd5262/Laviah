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
#include "../MyUI/Tutorial/TutorialHelper.hpp"
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
, m_IsTutorial(false)
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

void CObjectManager::SpeedControl(float duration, float speed, bool force/* = false*/)
{
    if(!m_SpeedController) return;
    if(force) m_SpeedController->stopActionByTag(100);
    if(m_SpeedController->getActionByTag(100)) return;
    
    auto move = MoveTo::create(duration, Vec2(speed, 0));
    move->setTag(100);
    m_SpeedController->runAction(move);
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
    CGameScene::getGameScene()->MenuFadeIn();
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
    this->MoveAction(CGameScene::getZoomLayer(), MOVE_DIRECTION::DOWN);
    m_Rocket->setVisible(false);
}

void CObjectManager::ZoomOutRank()
{
    this->ZoomIn();
    m_Rocket->setVisible(true);
}

void CObjectManager::MoveAction(cocos2d::Node* owner, MOVE_DIRECTION dir)
{
    auto winSize    = Director::getInstance()->getWinSize();
    Vec2 posArray[] = {
        Vec2( -winSize.width * 0.5f,  winSize.height * 0.5f),
        Vec2(  winSize.width * 1.5f,  winSize.height * 0.5f),
        Vec2(  winSize.width * 0.5f,  winSize.height * 1.5f),
        Vec2(  winSize.width * 0.5f, -winSize.height * 0.5f),
        Vec2(  winSize.width * 0.5f,  winSize.height * 0.5f),
    };
    
    this->zoom(owner, posArray[dir], 0, 1.f, 1.2f, true);
}

void CObjectManager::GiantMode()
{
    m_GiantSpeed = 1.5f;
    auto levelData = m_LevelList.at(m_GameLevel);
    this->zoom(CGameScene::getZoomLayer(), levelData._pos, levelData._angle, 0.45f, 1.f, true);
}

void CObjectManager::NormalMode()
{
    m_GiantSpeed = 1.f;
    auto levelData = m_LevelList.at(m_GameLevel);
    this->zoom(CGameScene::getZoomLayer(), levelData._pos, levelData._angle, levelData._zoom, 1.f, true);
}

void CObjectManager::setGameStateByLevel()
{
    if(m_IsTutorial) return;
    
    auto levelData = m_LevelList.at(m_GameLevel);
    this->zoom(CGameScene::getZoomLayer(), levelData._pos, levelData._angle, levelData._zoom, 8);
    this->SpeedControl(1.f, levelData._speed);
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
    if(m_IsTutorial) return;
    
    m_PatternTimer += delta;
	if (m_PatternTimer < BULLETCREATOR::PATTERN_PADDING_LIMIT) return;

    auto levelData = m_LevelList.at(m_GameLevel);
    auto level = levelData._level;
    auto below = levelData._below;
    auto data = m_PatternManager->getRandomNormalPatternByLevel(level, below);
    m_BulletCreator->setPattern(data);
    
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
    if(m_IsTutorial) return;
    
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
}

void CObjectManager::InitTutorialStep()
{
    m_IsTutorial = true;

    CTutorialStep::create()
    ->addBeginListener([=](CTutorialStep* sender){
        this->zoom(CGameScene::getZoomLayer(),
                   PLANET_DEFINE::GAME_POS,
                   0,
                   0.8f,
                   2.f,
                   true);
    })
    ->addUpdateListener([=](float delta, CTutorialStep* sender){
        if(sender->getTime() > 2.2)
            CTutorialManager::Instance()->NextStep();
    })
    ->build(TUTORIAL_KEY::BEGINER)
    ->setBackgroundVisible(true)
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA);
    
    
    CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::BEGINER, "바이킹이 여기까지 쫒아 왔어요!!");
    CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::BEGINER, "이 행성도 더 이상 안전하지 않은 것 같아요!!");
    CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::BEGINER, "어서 빨리 탈출하세요!!");
    
    
    CTutorialHelper::Instance()->CreateBulletPattern(TUTORIAL_KEY::BEGINER, 0);
    CTutorialHelper::Instance()->NextStepAfterDelay(TUTORIAL_KEY::BEGINER, 5.f, true);
    CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::BEGINER, "이건 바이킹이 쏘는 총알이에요!!", true);
    CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::BEGINER, "한번이라도 맞으면 끝이니까 조심해야해요!!", true);
    CTutorialHelper::Instance()->NextStepAfterDelay(TUTORIAL_KEY::BEGINER, 1.f, true);
    
    
    CTutorialHelper::Instance()->CreateBulletPattern(TUTORIAL_KEY::BEGINER, 1);
    CTutorialHelper::Instance()->NextStepAfterDelay(TUTORIAL_KEY::BEGINER, 4.f);
    CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::BEGINER, "화면을 길게 누르면 반대 방향으로 피할 수 있어요!!");
    
    CTutorialHelper::Instance()->CreateBulletPattern(TUTORIAL_KEY::BEGINER, 2);
    CTutorialHelper::Instance()->NextStepAfterDelay(TUTORIAL_KEY::BEGINER, 5.f);

    
    CTutorialHelper::Instance()->CreateBulletPattern(TUTORIAL_KEY::BEGINER, 3);
    CTutorialHelper::Instance()->NextStepAfterDelay(TUTORIAL_KEY::BEGINER, 4.f);
    CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::BEGINER, "하얀색 빛은 곧 미사일이 떨어진다는 표시이니 주의 해야해요.");
    
    
    CTutorialHelper::Instance()->CreateBulletPattern(TUTORIAL_KEY::BEGINER, 4);
    CTutorialHelper::Instance()->NextStepAfterDelay(TUTORIAL_KEY::BEGINER, 4.f);
    CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::BEGINER, "엇!! 우주에 떠다니는 크리스탈 운석이에요!! 부딪히지 않도록 주의하세요.");
    CTutorialHelper::Instance()->NextStepAfterDelay(TUTORIAL_KEY::BEGINER, 0.5f);
    CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::BEGINER, "땅에 떨어지고 나서도 잠시동안 유지되니 조심하세요!");
    
    
    CTutorialHelper::Instance()->CreateBulletPattern(TUTORIAL_KEY::BEGINER, 5);
    CTutorialHelper::Instance()->NextStepAfterDelay(TUTORIAL_KEY::BEGINER, 4.f);
    CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::BEGINER, "행성이 불안정하기 때문에 땅에서 위험한 것들이 솟아나기도 하니까 조심 해야해요.");

    
    CTutorialHelper::Instance()->CreateBulletPattern(TUTORIAL_KEY::BEGINER, 6);
    CTutorialHelper::Instance()->NextStepAfterDelay(TUTORIAL_KEY::BEGINER, 4.f);
    CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::BEGINER, "오! 별이에요!! 별은 곧 점수나 마찬가지에요.");
    
    
    CTutorialHelper::Instance()->NextStepAfterDelay(TUTORIAL_KEY::BEGINER, 2.5f);
    CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::BEGINER, "별을 연속해서 먹을수록 콤보가 올라가요.");
    CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::BEGINER, "콤보가 높을수록 별 한개 당 점수가 높아져요.");
    CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::BEGINER, "콤보가 끊기지 않도록 별을 놓치지 않는 것이 중요해요!!");
 
    
    CTutorialHelper::Instance()->CreateBulletPattern(TUTORIAL_KEY::BEGINER, 7);
    CTutorialHelper::Instance()->NextStepAfterDelay(TUTORIAL_KEY::BEGINER, 3.5f);
    CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::BEGINER, "이제 아이템을 설명할 차례네요.");
    CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::BEGINER, "아이템은 공격을 피하는데 큰 도움이 될거에요.");
    CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::BEGINER, "먼저 모든 것을 별로 바꿔주는 별마법 아이템이에요.");
    
    CTutorialHelper::Instance()->CreateBulletPattern(TUTORIAL_KEY::BEGINER, 8);
    CTutorialHelper::Instance()->NextStepAfterDelay(TUTORIAL_KEY::BEGINER, 5.f);

    
    CTutorialHelper::Instance()->CreateBulletPattern(TUTORIAL_KEY::BEGINER, 9);
    CTutorialHelper::Instance()->NextStepAfterDelay(TUTORIAL_KEY::BEGINER, 3.5f);
    CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::BEGINER, "다음은 모든 것을 코인으로 바꿔주는 코인마법 아이템이에요.");
    CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::BEGINER, "코인으로 아이템 능력을 업그레이드 시킬수도 있다는 것 잊지마세요.");
    
    CTutorialHelper::Instance()->CreateBulletPattern(TUTORIAL_KEY::BEGINER, 10);
    CTutorialHelper::Instance()->NextStepAfterDelay(TUTORIAL_KEY::BEGINER, 5.f);

    
    CTutorialHelper::Instance()->CreateBulletPattern(TUTORIAL_KEY::BEGINER, 11);
    CTutorialHelper::Instance()->NextStepAfterDelay(TUTORIAL_KEY::BEGINER, 4.f);
    CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::BEGINER, "엇!! 이번엔 정말로 위험한 것 같은데요?!");
    CTutorialHelper::Instance()->NextStepAfterDelay(TUTORIAL_KEY::BEGINER, 1.f);
    CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::BEGINER, "걱정하지 마시라!! 이게 바로 방어마법 아이템의 능력이죠!!");
    CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::BEGINER, "방어도 하고 콤보도 올리고!! 이게 바로 일석이조!!");
    CTutorialHelper::Instance()->NextStepAfterDelay(TUTORIAL_KEY::BEGINER, 1.f);

    
    CTutorialHelper::Instance()->CreateBulletPattern(TUTORIAL_KEY::BEGINER, 12);
    CTutorialHelper::Instance()->NextStepAfterDelay(TUTORIAL_KEY::BEGINER, 4.f);
    CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::BEGINER, "자!! 이제 마지막 거인마법 아이템으로 모두 날려 버리세요!!");
    CTutorialHelper::Instance()->NextStepAfterDelay(TUTORIAL_KEY::BEGINER, 5.f);
    CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::BEGINER, "마찬가지로 날려버린 것들에 대해서 콤보가 올라갑니다.");
    CTutorialHelper::Instance()->NextStepAfterDelay(TUTORIAL_KEY::BEGINER, 2.f);

    
    CTutorialHelper::Instance()->NextStepAfterDelay(TUTORIAL_KEY::BEGINER, 5.f);
    CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::BEGINER, "이제 튜토리얼은 끝났습니다. 튜토리얼을 다시 보고 싶다면 설정에서 다시보기 버튼을 눌러주세요!!");

    
    CTutorialStep::create()
    ->addBeginListener([=](CTutorialStep* sender){
        m_IsTutorial = false;
    })
    ->addUpdateListener([=](float delta, CTutorialStep* sender){
        if(sender->getTime() > 2.f)
            CTutorialManager::Instance()->NextStep();
    })
    ->build(TUTORIAL_KEY::BEGINER)
    ->setBackgroundVisible(true)
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA);
    
    
    CTutorialManager::Instance()->ChangeTutorial(TUTORIAL_KEY::BEGINER);
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