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
#include "../Facebook/FacebookManager.hpp"
#include "../MyUI/Tutorial/TutorialHelper.hpp"
#include "../MyUI/MyButton.h"
#include "../MyUI/UILayer.hpp"
#include "../MyUI/ScoreUI.h"
#include "../MyUI/UrlSprite.hpp"
#include "../Common/StringUtility.h"
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
, m_GiantSpeed(1.f)
, m_BulletPatternPaddingLimit(0.f)
, m_OriginPatternLevel(-1)
, m_PhotoShareAble(false)
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

    insertLevel(sLEVEL_BALANCE(20,  80.f,  1, 0.66f, 0, 0, Vec2(540.f, 672.f)));
    
    insertLevel(sLEVEL_BALANCE(30,  90.f,  2, 0.66f, 0, 0, Vec2(540.f, 672.f)));
    
    insertLevel(sLEVEL_BALANCE(40,  95.f,  3, 0.66f, 0, 1, Vec2(540.f, 672.f)));
    
    insertLevel(sLEVEL_BALANCE(60,  100.f, 4, 0.66f, 0, 2, Vec2(540.f, 672.f)));
    
    insertLevel(sLEVEL_BALANCE(80,  100.f, 5, 0.66f, 0, 3, Vec2(540.f, 672.f)));
    insertLevel(sLEVEL_BALANCE(90,  100.f, 5, 0.45f, 0, 3, Vec2(540.f, 672.f)));
    insertLevel(sLEVEL_BALANCE(100, 100.f, 5, 0.8f,  0, 3, Vec2(540.f, 400.f)));
    
    insertLevel(sLEVEL_BALANCE(110, 110.f, 5, 0.66f, 0, 4, Vec2(540.f, 672.f)));
    insertLevel(sLEVEL_BALANCE(120, 110.f, 5, 0.45f, 0, 4, Vec2(540.f, 672.f)));
    insertLevel(sLEVEL_BALANCE(130, 110.f, 5, 0.8f,  0, 4, Vec2(540.f, 400.f)));
    
    insertLevel(sLEVEL_BALANCE(140, 120.f, 5, 0.66f, 0, 5, Vec2(540.f, 672.f)));
    insertLevel(sLEVEL_BALANCE(150, 120.f, 5, 0.45f, 0, 5, Vec2(540.f, 672.f)));
    insertLevel(sLEVEL_BALANCE(160, 120.f, 5, 0.8f,  0, 5, Vec2(540.f, 400.f)));
    
    insertLevel(sLEVEL_BALANCE(170, 120.f, 5, 0.8f,  0, 6, Vec2(540.f, 100.f)));
    insertLevel(sLEVEL_BALANCE(190, 130.f, 5, 0.45f, 0, 6, Vec2(540.f, 672.f)));
    insertLevel(sLEVEL_BALANCE(210, 130.f, 5, 0.66f, 0, 6, Vec2(540.f, 672.f)));
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
    m_BulletPatternPaddingLimit = 0.f;
    GLOBAL->STAGE_LEVEL = 0;
    m_SpeedController->setPositionX(0.f);
	m_IsGamePause = true;
    m_BulletCreator->Clear();
    m_Planet->Clear();
    m_Player->Clear();
	m_BarrierItemRange->Clear();
	m_StarItemRange->Clear();
	m_CoinItemRange->Clear();
    m_PhotoShareAble = false;
    
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

void CObjectManager::Intro(Node* obj,
                           float duration,
                           Vec2 tPos,
                           bool skip,
                           std::function<void()> endListener/* = nullptr*/)
{
    float delayTime = 2.5f;
    if(skip) {
        duration    *= 0.15f;
        delayTime   = 0.f;
    }
    obj->stopActionByTag(100);

    auto delay                    = DelayTime::create(delayTime);
    auto moveAction               = MoveTo::create(duration, tPos);
    FiniteTimeAction* sineAction1 = EaseSineInOut::create(moveAction);
    FiniteTimeAction* sineAction2 = EaseSineOut::create(moveAction);
    auto callFunc                 = CallFunc::create(endListener);
    auto sequence                 = Sequence::create(delay,
                                                     (skip ? sineAction2 : sineAction1),
                                                     callFunc,
                                                     nullptr);
    sequence->setTag(100);
    obj->runAction(sequence);
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
    this->MoveAction(CGameScene::getZoomLayer(), MOVE_DIRECTION::DOWN);
    m_Rocket->setVisible(false);
}

void CObjectManager::ZoomOutRank()
{
    this->MoveAction(CGameScene::getZoomLayer(), MOVE_DIRECTION::MIDDLE);
    m_Rocket->setVisible(true);
}

void CObjectManager::MoveAction(MOVE_DIRECTION dir)
{
    this->MoveAction(CGameScene::getPopupLayer(), dir);
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
    
    this->zoom(owner, posArray[dir], -1, 1.f, 1.2f, true);
}

void CObjectManager::GiantMode()
{
    m_GiantSpeed = 1.5f;
    auto levelData = m_LevelList.at(GLOBAL->STAGE_LEVEL);
    this->zoom(CGameScene::getZoomLayer(), levelData._pos, levelData._angle, levelData._zoom * 1.25f, 1.f, true);
}

void CObjectManager::NormalMode()
{
    m_GiantSpeed = 1.f;
    auto levelData = m_LevelList.at(GLOBAL->STAGE_LEVEL);
    this->zoom(CGameScene::getZoomLayer(), levelData._pos, levelData._angle, levelData._zoom, 1.f, true);
}

void CObjectManager::setGameStateByLevel()
{
    if(CTutorialManager::Instance()->getIsRunning()) return;
    
    auto levelData = m_LevelList.at(GLOBAL->STAGE_LEVEL);
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
    if(CTutorialManager::Instance()->getIsRunning()) return;
    
    if(!m_BulletCreator->getIsRunning())
        m_PatternTimer += delta;
	if (m_PatternTimer < m_BulletPatternPaddingLimit) return;

    auto levelData = m_LevelList.at(GLOBAL->STAGE_LEVEL);
    auto pattern = levelData._pattern;
    auto below = levelData._below;
    auto data = m_PatternManager->getRandomNormalPatternByLevel(pattern, below);
    m_BulletCreator->setPattern(data);
    
    m_BulletPatternPaddingLimit = BULLETCREATOR::PATTERN_PADDING_LIMIT;
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
    if(CTutorialManager::Instance()->getIsRunning()) return;
    
    m_LevelTimer += m_Delta;
    auto levelData = m_LevelList.at(GLOBAL->STAGE_LEVEL);
    if(levelData._time < m_LevelTimer)
    {
        if(m_LevelList.size() > GLOBAL->STAGE_LEVEL+1){
            GLOBAL->STAGE_LEVEL++;
            GLOBAL->PATTERN_LEVEL = levelData._level;
            this->setGameStateByLevel();
    
            if(m_OriginPatternLevel != GLOBAL->PATTERN_LEVEL){
                CUILayer::Instance()->LevelUPNotice();
                m_OriginPatternLevel        = GLOBAL->PATTERN_LEVEL;
                m_BulletPatternPaddingLimit = 2.f;
                m_PatternTimer              = 0.f;
            }
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
    if(zoomAngle == -1) zoomAngle = obj->getRotation();
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
    auto layerSize = Director::getInstance()->getWinSize();
    
    auto createMessageBox = [=](std::string msg,
                                cocos2d::Vec2 pos = Vec2::ZERO,
                                cocos2d::Vec2 tailPos = Vec2::ANCHOR_MIDDLE_BOTTOM,
                                eMYBUTTON_STATE action = eMYBUTTON_STATE::BEGIN){
        if(pos == Vec2::ZERO)
            pos = Vec2(layerSize.width * 0.5f, layerSize.height * 0.65f);
        CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::BEGINER, msg, pos, tailPos, action);
    };
    
    auto createPattern    = [=](int index){
        CTutorialHelper::Instance()->CreateBulletPattern(TUTORIAL_KEY::BEGINER, index);
    };
    
    auto nextStep         = [=](float delayTime){
        CTutorialHelper::Instance()->NextStepAfterDelay(TUTORIAL_KEY::BEGINER, delayTime);
    };
    
    auto rotationEnable   = [=](bool enable){
        CTutorialHelper::Instance()->RotationEnable(TUTORIAL_KEY::BEGINER, enable);
    };

    // REVIVE
    {
        CTutorialHelper::Instance()->CreateMessageBox(TUTORIAL_KEY::REVIVE,
                                                      "자! 다시 한 번 도전해보세요!",
                                                      Vec2(layerSize.width * 0.5f, layerSize.height * 0.65f),
                                                      Vec2::ANCHOR_MIDDLE_BOTTOM,
                                                      eMYBUTTON_STATE::BEGIN);
        
        CTutorialStep::create()
        ->addUpdateListener([=](float delta, CTutorialStep* sender){
            if(sender->getTime() > 0.f)
                CTutorialManager::Instance()->LoadLastSavedStep(TUTORIAL_KEY::BEGINER);
        })
        ->build(TUTORIAL_KEY::REVIVE)
        ->setBackgroundVisible(true)
        ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA);
    }
    
    // BEGINER
    {
        CTutorialStep::create()
        ->addBeginListener([=](CTutorialStep* sender){
            this->zoom(CGameScene::getZoomLayer(),
                       PLANET_DEFINE::GAME_POS,
                       0, 0.8f, 2.f, true);
        })
        ->addUpdateListener([=](float delta, CTutorialStep* sender){
            if(sender->getTime() > 2.2)
                CTutorialManager::Instance()->NextStep();
        })
        ->build(TUTORIAL_KEY::BEGINER)
        ->setBackgroundVisible(true)
        ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA);
        
        rotationEnable(false);
        
        createMessageBox("우주 해적이 여기까지 쫒아 왔어요!!");
        createMessageBox("이 행성도 더 이상 안전하지 않은 것 같아요!!");
        createMessageBox("어서 빨리 탈출하세요!!");
        
        
        createPattern(0);
        nextStep(2.5f);
        createMessageBox("이건 해적들이 쏘는 총알이에요!!");
        createMessageBox("한번이라도 맞으면 끝이니까 조심해야해요!!");
        
        
        nextStep(3.f);
        createPattern(1);
        nextStep(1.f);
        rotationEnable(true);
        createMessageBox("화면을 길게 누르면 반대 방향으로 피할 수 있어요!!");
        
        
        nextStep(4.f);
//        createMessageBox("test1");
        createPattern(2);
        
        
        nextStep(7.f);
//        createMessageBox("test2");
        createPattern(3);
        nextStep(2.f);
        createMessageBox("하얀색 빛은 곧 미사일이 떨어진다는 표시이니 주의 해야해요.");
        
        
        nextStep(3.5f);
//        createMessageBox("test3");
        createPattern(4);
        nextStep(2.f);
        createMessageBox("위험해요!! 우주에 떠다니는 크리스탈 운석이에요.");
        createMessageBox("스치지 않도록 주의하세요.");
        nextStep(1.0f);
        createMessageBox("땅에 떨어지고 나서도 잠시동안 유지되니까 조심하세요!");
        
        
        nextStep(4.f);
//        createMessageBox("test4");
        createPattern(5);
        nextStep(1.f);
        createMessageBox("행성이 불안정하기 때문에 땅에서 위험한 것들이 솟아나기도 하니까 조심 해야해요.");
        
        
        nextStep(5.f);
//        createMessageBox("test5");
        createPattern(6);
        nextStep(1.f);
        createMessageBox("오! 별이에요!!");
        nextStep(3.5f);
        createMessageBox("별을 먹으면 점수가 올라요.",
                         Vec2(layerSize.width * 0.45f, layerSize.height * 0.91f),
                         Vec2::ANCHOR_TOP_LEFT);
        
        
        nextStep(2.5f);
        createMessageBox("별을 먹을수록 콤보가 올라가요.",
                         Vec2(layerSize.width * 0.5f, layerSize.height * 0.8f));
        createMessageBox("콤보가 높으면 별 한개 당 점수가 높아져요.",
                         Vec2(layerSize.width * 0.5f, layerSize.height * 0.8f));
        createMessageBox("일정시간 별을 먹지 않으면 콤보가 끊기니까 주의하세요!!",
                         Vec2(layerSize.width * 0.5f, layerSize.height * 0.8f));
        
//        createMessageBox("test6");
        createPattern(7);
        nextStep(0.5f);
        createMessageBox("이제 아이템을 설명할 차례네요.");
        createMessageBox("아이템은 공격을 피하는데 큰 도움이 될거에요.");
        createMessageBox("먼저 모든 것을 별로 바꿔주는 별마법 아이템이에요.");
        
        
        nextStep(4.5f);
//        createMessageBox("test7");
        createPattern(8);
        
        nextStep(9.0f);
//        createMessageBox("test8");
        createPattern(9);
        nextStep(1.f);
        createMessageBox("다음은 모든 것을 코인으로 바꿔주는 코인마법 아이템이에요.");
        createMessageBox("코인으로 아이템 능력을 업그레이드 시킬수도 있다는 사실!! 잊지마세요.");
        
        nextStep(4.0f);
//        createMessageBox("test9");
        createPattern(10);
        
        nextStep(9.0f);
//        createMessageBox("test10");
        createPattern(11);
        nextStep(2.5f);
        createMessageBox("이런, 이번엔 정말로 위험한 것 같은데요?!");
        nextStep(1.0f);
        createMessageBox("걱정하지 마시라!! 이게 바로 방어마법 아이템의 능력이죠!!");
        createMessageBox("방어도 하고 콤보도 올리고!! 이게 바로 일석이조!!");
        
        nextStep(2.f);
//        createMessageBox("test11");
        createPattern(12);
        nextStep(2.f);
        createMessageBox("자!! 이제 마지막 거인마법 아이템으로 모두 날려 버리세요!!");
        nextStep(3.f);
        createMessageBox("마찬가지로 날려버린 것들에 대해서 콤보가 올라갑니다.");
        nextStep(6.f);
        
        
        createMessageBox("이제 튜토리얼은 끝났습니다.");
        createMessageBox("튜토리얼을 다시 보고 싶다면 설정에서 다시보기 버튼을 눌러주세요.");
        createMessageBox("그럼, 힘내서 함께 탈출하자고요!!");
        CTutorialStep::create()
        ->addBeginListener([=](CTutorialStep* sender){
            GLOBAL->Clear();
        })
        ->addUpdateListener([=](float delta, CTutorialStep* sender){
            if(sender->getTime() > 0)
                CTutorialManager::Instance()->NextStep();
        })
        ->build(TUTORIAL_KEY::BEGINER)
        ->setBackgroundVisible(true)
        ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA);
        
        CTutorialManager::Instance()->ChangeTutorial(TUTORIAL_KEY::BEGINER);
    }
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

void CObjectManager::ShowCapturingAction(){
    
    auto gameScene  = CGameScene::getGameScene();
    auto popupLayer = CGameScene::getPopupLayer();
    auto layerSize  = popupLayer->getContentSize();
    gameScene->scheduleOnce([=](float delta){
        
        m_PhotoShareAble = true;
        this->SaveCaptureInfo();
        auto pic     = this->Capture(layerSize.width, layerSize.height, false);
        pic->setPosition(popupLayer->getContentSize() / 2);
        popupLayer->addChild(pic, ZORDER::POPUP);
        pic->setOpacity(255 * 0.4f);
        auto scale   = ScaleTo::create(0.3f, 0.9f);
        auto easeOut = EaseSineOut::create(scale);
        auto delay   = DelayTime::create(0.2f);
        auto move    = MoveBy::create(0.3f, Vec2(0, 200));
        auto fadeOut = FadeTo::create(0.2f, 0);
        auto spawn   = Spawn::createWithTwoActions(move, fadeOut);
        auto ease    = EaseSineIn::create(spawn);
        auto func    = CallFunc::create([=](){
            pic->removeFromParent();
        });
        auto sequence = Sequence::create(easeOut, delay, ease, func, nullptr);
        
        pic->runAction(sequence);
        
    }, 0.5f, "capturingDelay");
}

void CObjectManager::SaveCaptureInfo(){
    
    m_CaptureInfo._backColorStart = m_Background->getCurrentStartColor();
    m_CaptureInfo._backColorEnd   = m_Background->getCurrentEndColor();
    m_CaptureInfo._planetAngle    = m_Planet->getAngle();
    m_CaptureInfo._characterAngle = m_Player->getAngle();
}

cocos2d::Node* CObjectManager::Capture(float width/* = 1080*/,
                                       float height/* = 1920*/,
                                       bool uiEnable/* = true*/)
{
    auto captureNode = cocos2d::Node::create();
    captureNode->setCascadeOpacityEnabled(true);
    captureNode->setContentSize(Director::getInstance()->getWinSize());

    captureNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    auto layerSize   = captureNode->getContentSize();

    captureNode->setVisible(true);
    captureNode->removeAllChildren();
    captureNode->setScaleX(width / layerSize.width);
    captureNode->setScaleY(height / layerSize.height);
    
    
    // clipper
    auto stencil = DrawNode::create();
    stencil->drawSolidRect(Vec2(0, 420), Vec2(1080, 1920), Color4F::GREEN);
    
    auto clipper = ClippingNode::create(stencil);
    clipper->setInverted(false);
    clipper->setCascadeOpacityEnabled(true);
    captureNode->addChild(clipper);
    
    
    // background
    auto background = LayerGradient::create();
    background->setCascadeOpacityEnabled(true);
    background->setContentSize(layerSize);
    background->setIgnoreAnchorPointForPosition(false);
    background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    background->setPosition(Vec2(layerSize.width * 0.5f, layerSize.height * 0.5f));
    background->setStartColor(m_CaptureInfo._backColorStart);
    background->setEndColor(m_CaptureInfo._backColorEnd);
    clipper->addChild(background);
    
    
    if(uiEnable){
        auto createIcon = [=](std::string iconImg, Vec2 pos){
            auto icon = Sprite::create(iconImg);
            icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            icon->setPosition(Vec2(pos.x, pos.y));
            icon->setOpacity(255 * 0.8f);
            icon->setScale(45 / icon->getContentSize().height);
            background->addChild(icon);
        };
        
        auto createLabel = [=](std::string value, float size, Vec2 pos){
            auto label = Label::createWithTTF(value, FONT::MALGUNBD, size);
            label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            label->setPosition(pos);
            label->setOpacity(255 * 0.8f);
            label->enableOutline(COLOR::BRIGHT_WHITEGRAY_ALPHA, 3);
            background->addChild(label);
            return label;
        };
        
        auto createScoreUI = [=](std::string iconImg, int value, Vec2 pos){
            createIcon(iconImg, Vec2(pos.x + 45, pos.y));
            createLabel(StringUtility::toCommaString(value), 45, Vec2(pos.x + (45 * 2.f), pos.y));
        };
        
        // score
        {
            createScoreUI("starIcon.png", GLOBAL->TOTAL_SCORE, Vec2(layerSize.width * 0.025f,
                                                                    layerSize.height * 0.96f ));
        }
        
        // rank
        {
            auto rank = CFacebookManager::Instance()->getMyRank();
            createScoreUI("rankingIcon.png", rank + 1, Vec2(layerSize.width * 0.025f,
                                                            layerSize.height * 0.925f ));
        }
        
        // level
        {
            auto label = createLabel(StringUtils::format("LEVEL %d", GLOBAL->PATTERN_LEVEL + 1), 50,
                                     Vec2(layerSize.width * 0.5f, layerSize.height * 0.96f ));
            label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            label->setOpacity(255 * 0.4f);
            //        auto label = Label::createWithTTF(StringUtils::format("LEVEL %d", GLOBAL->PATTERN_LEVEL),
            //                                          FONT::MALGUNBD, 150);
            //        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            //        label->setPosition(Vec2(layerSize.width * 0.5f, layerSize.height * 0.825f));
            //        captureNode->addChild(label);
        }
        
        // logo
        {
            auto logo = Sprite::create("background_0.png");
            logo->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            logo->setPosition(Vec2(layerSize.width * 0.9f, layerSize.height * 0.26f));
            logo->setScale(0.3f);
            background->addChild(logo);
        }
    }
    
    // zoomLayer
    auto zoomLayer = Node::create();
    zoomLayer->setCascadeOpacityEnabled(true);
    zoomLayer->setContentSize(layerSize);
    zoomLayer->setPosition(layerSize / 2);
    zoomLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    zoomLayer->setScale(0.7f);
    background->addChild(zoomLayer);
    
    // stars
    {
        for(int i = 0; i < 15; i++)
        {
            auto star = Sprite::create("whiteSquare.png");
            star->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            star->setPosition(Vec2(random<int>(30, 1050), random<int>(1000, 1900)));
            star->setOpacity(random<float>(255 * 0.1f, 255 * 0.7f));
            star->setScale(random<float>(0.4f, 1.2f));
            star->setRotation(random<int>(0, 360));
            background->addChild(star);
        }
    }
    
    // planet
    {
        auto planet  = Sprite::createWithSpriteFrameName(m_CharacterParam->_planetTextureName);
        planet->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        planet->setPosition(Vec2(layerSize.width * 0.5f, layerSize.height * 0.35f));
        planet->setRotation(m_CaptureInfo._planetAngle);
        zoomLayer->addChild(planet);
    }
    
    // character
    {
        auto character = Sprite::createWithSpriteFrameName(m_CharacterParam->_normalTextureName);
        character->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        character->setPosition(Vec2(layerSize.width * 0.5f, layerSize.height * 0.35f
                                    +  PLANET_DEFINE::BOUNDING_RADIUS
                                    + (PLAYER_DEFINE::NORMAL_BOUNDING_RADIUS * 0.8f)));
        character->setRotation(m_CaptureInfo._characterAngle);
        zoomLayer->addChild(character);
    }
    
    // rival
    auto createRival = [=](int count, int max, float angle)
    {
        auto myRank    = CFacebookManager::Instance()->getMyRank();
        auto rivalRank = myRank + 1; // 나보다 1낮은 등급
        auto rivalData = CFacebookManager::Instance()->getFriendByRank(rivalRank);
        auto pos       = Vec2((layerSize.width * 0.7f) + (count * 70),
                              (layerSize.height * 0.78f) + (count * 100));
        auto opacity   = (255 - ((max - count) * 50));
        angle = angle - ((max - count) * 30);
        auto ship      = Sprite::createWithSpriteFrameName("ship.png");
        ship->setCascadeOpacityEnabled(true);
        ship->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        ship->setPosition(pos);
        ship->setRotation(angle);
        ship->setOpacity(opacity);
        ship->setScale(1.2f);
        
        zoomLayer->addChild(ship);
        
        auto flag      = Sprite::createWithSpriteFrameName("flag.png");
        flag->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        flag->setPosition(Vec2(ship->getContentSize().width * 1.1f, ship->getContentSize().height * 0.4f));
        flag->setScale(1.5f);
        ship->addChild(flag);
    
        auto label     = Label::createWithTTF(StringUtils::format("%d", rivalRank), FONT::MALGUNBD, 30);
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        label->setPosition(Vec2(flag->getContentSize().width * 0.7f, flag->getContentSize().height * 0.6f));
        label->setRotation(90);
        label->setColor(COLOR::DARKGRAY);
        flag->addChild(label);
        
        // cliper
        auto circleStencil = DrawNode::create();
        circleStencil->drawSolidCircle(Vec2(ship->getContentSize().width * 0.6f,
                                            ship->getContentSize().height * 0.48f),
                                            35, 0, 20, 1, 1, Color4F::GREEN);
        
        auto circleClipper = ClippingNode::create(circleStencil);
        circleClipper->setCascadeOpacityEnabled(true);
        circleClipper->setInverted(false);
        ship->addChild(circleClipper);
        
        auto pic       = CUrlSprite::create()
        ->setSize(Size(70, 70))
        ->setUrl(rivalData->_url, rivalData->_url)
        ->setSaveToFileEnable(true)
        ->build(circleClipper, -1);
        pic->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        pic->setPosition(Vec2(ship->getContentSize().width * 0.6f, ship->getContentSize().height * 0.48f));
        pic->setRotation(90);
    };
    
    for(int i = 0; i <= 3; i++)
        createRival(i, 3, 300);
    
    return captureNode;
}