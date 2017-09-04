#include "ObjectManager.h"
#include "Planet.h"
#include "Player.h"
#include "BackGround.h"
#include "ItemManager.h"
#include "BulletCreator.h"
#include "Rocket.h"
#include "ItemRange.h"
#include "MagnetEffect.h"
#include "Bullet/Bullet.h"
#include "../Task/PoolingManager.h"
#include "../AI/States/GameStates.h"
#include "../AI/States/RocketStates.h"
#include "../Scene/GameScene.h"
#include "../DataManager/UserDataManager.h"
#include "../DataManager/CharacterDataManager.h"
#include "../DataManager/RocketDataManager.hpp"
#include "../DataManager/PlanetDataManager.hpp"
#include "../DataManager/CostumeDataManager.hpp"
#include "../DataManager/BulletPatternDataManager.h"
#include "../SDKBOX/SDKBoxHeaders.h"
#include "../MyUI/Tutorial/TutorialHelper.hpp"
#include "../MyUI/MyButton.h"
#include "../MyUI/UILayer.hpp"
#include "../MyUI/ScoreUI.h"
#include "../MyUI/UrlSprite.hpp"
#include "../MyUI/ComboScore.h"
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
, m_MagnetItemRange(nullptr)
, m_SpeedController(nullptr)
, m_CharacterParam(nullptr)
, m_CaptureNode(nullptr)
, m_ItemManager(CItemManager::Instance())
, m_PatternManager(CBulletPatternDataManager::Instance())
, m_Delta(0.f)
, m_GiantSpeed(1.f)
, m_BulletPatternPaddingLimit(0.f)
, m_OriginPatternLevel(-1)
, m_PhotoShareAble(false)
, m_SlowMotionAble(false)
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
    GVALUE->STAGE_LEVEL = 0;
    m_SpeedController->setScale(0.f);
	m_IsGamePause = true;
    m_BulletCreator->Clear();
    m_Planet->Clear();
    m_Player->Clear();
	m_BarrierItemRange->Clear();
	m_StarItemRange->Clear();
	m_CoinItemRange->Clear();
    m_MagnetItemRange->Clear();
    m_PhotoShareAble = false;
    CComboScore::Instance()->ComboScoreReset();
    m_CurrentStage = CStageDataManager::Instance()->getStageByIndex(0)->_stageDataLiat;
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
    
    auto move = ScaleTo::create(duration, speed);
    move->setTag(100);
    m_SpeedController->runAction(move);
}

void CObjectManager::ChangeCharacter()
{
    m_CharacterParam = CCharacterDataManager::Instance()->getCurCharacter();
    if(m_Player) m_Player->setCharacterParam(m_CharacterParam);
    this->ChangeCostume();
}

void CObjectManager::ChangeCostume()
{
    auto character = CCharacterDataManager::Instance()->getCurCharacter();
    auto index     = character->_index;
    m_CostumeParam = CCostumeDataManager::Instance()->getCurCostumeByCharacter(index);
    if(m_Player) m_Player->setCostumeParam(m_CostumeParam);
}

void CObjectManager::ChangeRocket()
{
    m_RocketParam = CRocketDataManager::Instance()->getCurRocket();
    if(m_Rocket) m_Rocket->setRocketParam(m_RocketParam);
}

void CObjectManager::ChangePlanet()
{
    m_PlanetParam = CPlanetDataManager::Instance()->getCurPlanet();
    if(m_Planet)        m_Planet->setPlanetParam(m_PlanetParam);
    if(m_BulletCreator) m_BulletCreator->setPlanetInfo(m_PlanetParam);
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
    float delayTime = 1.9f;
    if(skip) {
        duration    = 0.0f;
        delayTime   = 0.f;
    }
    obj->stopActionByTag(100);

    auto delay                    = DelayTime::create(delayTime);
    auto moveAction               = MoveTo::create(duration, tPos);
    FiniteTimeAction* sineAction1 = EaseSineInOut::create(moveAction);
    FiniteTimeAction* sineAction2 = EaseSineOut::create(moveAction);
    auto callFunc                 = CallFunc::create([=](){
        if(endListener){
            endListener();
        }
    });
    auto sequence                 = Sequence::create(delay,
                                                     (skip ? sineAction2 : sineAction1),
                                                     callFunc,
                                                     nullptr);
    sequence->setTag(100);
    obj->runAction(sequence);
}

void CObjectManager::ZoomMoveDown()
{
    this->MoveAction(CGameScene::getZoomLayer(), MOVE_DIRECTION::DOWN);
//    if(m_Rocket)
//        m_Rocket->setVisible(false);
}

void CObjectManager::ZoomMoveMiddle()
{
    this->MoveAction(CGameScene::getZoomLayer(), MOVE_DIRECTION::MIDDLE);
//    if(m_Rocket)
//        m_Rocket->setVisible(true);
}

void CObjectManager::MoveAction(cocos2d::Node* owner, MOVE_DIRECTION dir)
{
    auto winSize    = Director::getInstance()->getWinSize();
    Vec2 posArray[] = {
        Vec2( -winSize.width * 0.5f,  900.f),
        Vec2(  winSize.width * 1.5f,  900.f),
        Vec2(  winSize.width * 0.5f,  winSize.height * 1.5f),
        Vec2(  winSize.width * 0.5f, -900.f),
        Vec2(  winSize.width * 0.5f,  900.f),
    };
    
    this->MoveAction(owner, posArray[dir]);
}

void CObjectManager::MoveAction(cocos2d::Node* owner, cocos2d::Vec2 pos)
{
    this->zoom(owner, pos, 0, 1.f, 1.2f, true);
}

void CObjectManager::GiantMode()
{
    m_GiantSpeed = 1.2f;
    if(CTutorialManager::Instance()->getIsRunning()) {
        this->zoom(CGameScene::getZoomLayer(),
                   PLANET_DEFINE::GAME_POS,
                   0, 0.9f, 1.f, true, true);
    }else{
        auto data = m_CurrentStage.at(GVALUE->STAGE_LEVEL);
        this->zoom(CGameScene::getZoomLayer(),
                   data._pos, data._zoomAngle,
                   data._zoomSize * 1.25f, 1.f,
                   true, true);
    }
}

void CObjectManager::NormalMode()
{
    m_GiantSpeed = 1.f;
    if(CTutorialManager::Instance()->getIsRunning()) {
        this->zoom(CGameScene::getZoomLayer(),
                   PLANET_DEFINE::GAME_POS,
                   0, 0.8f, 2.f, true, true);
    }else{
        auto data = m_CurrentStage.at(GVALUE->STAGE_LEVEL);
        this->zoom(CGameScene::getZoomLayer(),
                   data._pos, data._zoomAngle,
                   data._zoomSize, 2.f,
                   true, true);
    }
}

void CObjectManager::setGameStateByLevel()
{
    if(CTutorialManager::Instance()->getIsRunning()) return;
    
    auto data = m_CurrentStage.at(GVALUE->STAGE_LEVEL);
    this->zoom(CGameScene::getZoomLayer(),
               data._pos, data._zoomAngle,
               data._zoomSize, data._duration,
               false, true);
    
    this->SpeedControl(1.f, data._speed / BULLETCREATOR::ROTATION_SPEED);
}

void CObjectManager::SlowMotion()
{
    if(!m_SlowMotionAble) return;
    this->SpeedControl(0.5f, 0.1f, true);
    m_SlowMotionAble = false;
    CGameScene::getZoomLayer()->scheduleOnce([=](float delta){
        this->SlowMotionFinish();
    }, 1.f, "StopSlowMotion");
}

void CObjectManager::SlowMotionFinish()
{
    auto data = m_CurrentStage.at(GVALUE->STAGE_LEVEL);
    this->SpeedControl(0.5f, data._speed / BULLETCREATOR::ROTATION_SPEED, true);
}

bool CObjectManager::IsHitWithSlowPoint(CBullet* bullet)
{
    auto layerSize = Director::getInstance()->getWinSize();
    if (bullet->IsHit(Vec2(layerSize.width * 0.5f, layerSize.height * 0.78f), 40)) return true;
//    if (bullet->IsHit(Vec2(layerSize.width * 0.4f, layerSize.height * 0.75f), 40)) return true;
    
    return false;
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
    
    this->returnToMemoryBlock();
}

void CObjectManager::createBulletByTimer(float delta)
{
    if(CTutorialManager::Instance()->getIsRunning()) return;
    
    if(!m_BulletCreator->getIsRunning())
        m_PatternTimer += delta;
	if (m_PatternTimer < m_BulletPatternPaddingLimit) return;

    auto data    = m_CurrentStage.at(GVALUE->STAGE_LEVEL);
    auto pattern = data._patternLevel;
    auto below   = (data._type == STAGE_DATA_TYPE::BELOW_RANDOM);
    auto patternData = m_PatternManager->getRandomNormalPatternByLevel(pattern, below);
    m_BulletCreator->setPattern(patternData);
    
    m_BulletPatternPaddingLimit = BULLETCREATOR::PATTERN_PADDING_LIMIT;
	m_PatternTimer = 0.f;
}

void CObjectManager::inGameUpdate(float delta)
{
    CObjectManager::Instance()->getPlayer()->ParticleVisible(!m_IsGamePause); //;;;

    if (m_IsGamePause) return;
    
    m_Delta         = delta * m_SpeedController->getScale();
    m_RotationSpeed = BULLETCREATOR::ROTATION_SPEED * m_Delta;
    
    this->returnToMemoryBlock();
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
    if(m_Rocket)
        m_Rocket->Execute(delta);
}

void CObjectManager::returnToMemoryBlock()
{
    for(auto bullet : m_BulletList)
    {
        if(bullet->IsReturnToMemoryBlock()){
            bullet->setAlive(false);
            bullet->removeAllChildren();
            bullet->removeFromParent();
            CPoolingManager::Instance()->Bullet_ReturnToFreeMemory(bullet);
        }
    }
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
    auto data = m_CurrentStage.at(GVALUE->STAGE_LEVEL);
    if(data._changeTime < m_LevelTimer)
    {
        if(m_CurrentStage.size() > GVALUE->STAGE_LEVEL+1){
            GVALUE->STAGE_LEVEL++;
            GVALUE->NOTICE_LEVEL = data._noticeLevel;
            this->setGameStateByLevel();
    
            if(m_OriginPatternLevel != GVALUE->NOTICE_LEVEL){
                CUILayer::Instance()->LevelUPNotice();
                m_OriginPatternLevel        = GVALUE->NOTICE_LEVEL;
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
                          bool force,
                          bool sine)
{
    if(force) obj->stopActionByTag(100);
    if(obj->getActionByTag(100)) return;
    if(zoomAngle == -1) zoomAngle = obj->getRotation();
    auto scaleAction  = ScaleTo::create(duration,  zoomSize);
    auto moveAction   = MoveTo::create(duration,   zoomPos);
    auto rotateAction = RotateTo::create(duration, zoomAngle);
    auto spawnAction  = Spawn::create(moveAction, scaleAction , rotateAction, nullptr);
    FiniteTimeAction* easeAction = nullptr;
    
    if(sine) easeAction = EaseSineInOut::create(spawnAction);
    else     easeAction = EaseExponentialInOut::create(spawnAction);
    easeAction->setTag(100);
    obj->runAction(easeAction);
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
                                                      TRANSLATE("TUTORIAL_MENT_30"),
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
                       0, 0.8f, 2.5f, true);
        })
        ->addUpdateListener([=](float delta, CTutorialStep* sender){
            if(sender->getTime() > 2.2)
                CTutorialManager::Instance()->NextStep();
        })
        ->build(TUTORIAL_KEY::BEGINER)
        ->setBackgroundVisible(true)
        ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA);
        
        rotationEnable(false);
        
        createMessageBox(TRANSLATE("TUTORIAL_MENT_0"));
        createMessageBox(TRANSLATE("TUTORIAL_MENT_1"));
        createMessageBox(TRANSLATE("TUTORIAL_MENT_2"));
        
        
        createPattern(0);
        nextStep(2.5f);
        createMessageBox(TRANSLATE("TUTORIAL_MENT_3"));
        createMessageBox(TRANSLATE("TUTORIAL_MENT_4"));
        createMessageBox(TRANSLATE("TUTORIAL_MENT_5"));
        
        
        nextStep(3.f);
        createPattern(1);
        nextStep(1.f);
        rotationEnable(true);
        createMessageBox(TRANSLATE("TUTORIAL_MENT_6"));
        
        
        nextStep(4.f);
//        createMessageBox("test1");
        createPattern(2);
        
        
        nextStep(7.f);
//        createMessageBox("test2");
        createPattern(3);
        nextStep(2.f);
        createMessageBox(TRANSLATE("TUTORIAL_MENT_7"));
        createMessageBox(TRANSLATE("TUTORIAL_MENT_8"));

        
        nextStep(3.5f);
//        createMessageBox("test3");
        createPattern(4);
        nextStep(2.f);
        createMessageBox(TRANSLATE("TUTORIAL_MENT_9"));
        nextStep(1.0f);
        createMessageBox(TRANSLATE("TUTORIAL_MENT_10"));

        
        nextStep(4.f);
//        createMessageBox("test4");
        createPattern(5);
        nextStep(1.f);
        createMessageBox(TRANSLATE("TUTORIAL_MENT_11"));
        
        
        nextStep(5.f);
//        createMessageBox("test5");
        createPattern(6);
        nextStep(1.f);
        createMessageBox(TRANSLATE("TUTORIAL_MENT_12"));
        nextStep(3.5f);
        createMessageBox(TRANSLATE("TUTORIAL_MENT_13"),
                         Vec2(layerSize.width * 0.45f, layerSize.height * 0.91f),
                         Vec2::ANCHOR_TOP_LEFT);
        
        
        nextStep(2.5f);
        createMessageBox(TRANSLATE("TUTORIAL_MENT_14"),
                         Vec2(layerSize.width * 0.5f, layerSize.height * 0.8f));
        createMessageBox(TRANSLATE("TUTORIAL_MENT_15"),
                         Vec2(layerSize.width * 0.5f, layerSize.height * 0.8f));
        createMessageBox(TRANSLATE("TUTORIAL_MENT_16"),
                         Vec2(layerSize.width * 0.5f, layerSize.height * 0.8f));
        
//        createMessageBox("test6");
        createPattern(7);
        nextStep(0.5f);
        createMessageBox(TRANSLATE("TUTORIAL_MENT_17"));
        createMessageBox(TRANSLATE("TUTORIAL_MENT_18"));
        createMessageBox(TRANSLATE("TUTORIAL_MENT_19"));
        
        
        nextStep(4.5f);
//        createMessageBox("test7");
        createPattern(8);
        
        nextStep(9.0f);
//        createMessageBox("test8");
        createPattern(9);
        nextStep(1.f);
        createMessageBox(TRANSLATE("TUTORIAL_MENT_20"));
        createMessageBox(TRANSLATE("TUTORIAL_MENT_21"));
        
        nextStep(4.0f);
//        createMessageBox("test9");
        createPattern(10);
        
        nextStep(9.0f);
//        createMessageBox("test10");
        createPattern(11);
        nextStep(3.0f);
        createMessageBox(TRANSLATE("TUTORIAL_MENT_22"));
        nextStep(1.0f);
        createMessageBox(TRANSLATE("TUTORIAL_MENT_23"));
        createMessageBox(TRANSLATE("TUTORIAL_MENT_24"));
        
        
        nextStep(2.f);
//        createMessageBox("test11");
        createPattern(12);
        nextStep(5.f);
        createMessageBox(TRANSLATE("TUTORIAL_MENT_25"));
        nextStep(2.f);
        createMessageBox(TRANSLATE("TUTORIAL_MENT_26"));
        nextStep(6.f);
        
        
        createMessageBox(TRANSLATE("TUTORIAL_MENT_27"));
        createMessageBox(TRANSLATE("TUTORIAL_MENT_28"));
        createMessageBox(TRANSLATE("TUTORIAL_MENT_29"));
        CTutorialStep::create()
        ->addBeginListener([=](CTutorialStep* sender){
            GVALUE->Clear();
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

void CObjectManager::AddUIToCapturedNode(Node* captured){
    if(!captured) return;
    

    auto layerSize  = captured->getContentSize();
    auto createLabel = [=](std::string value, float size, Vec2 pos){
        auto label = Label::createWithTTF(value, FONT::MALGUNBD, size);
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        label->setPosition(pos);
        label->setOpacity(255 * 0.1f);
        label->enableOutline(COLOR::BRIGHT_WHITEGRAY_ALPHA, 3);
        captured->addChild(label);
        return label;
    };
    
    // score
    createLabel(StringUtility::toCommaString(GVALUE->TOTAL_SCORE), 80, Vec2(layerSize.width * 0.03f,
                                                                            layerSize.height * 0.96f));
    
    // logo
    auto logo = Sprite::create("background_0.png");
    logo->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    logo->setPosition(Vec2(layerSize.width * 0.8f, layerSize.height * 0.05f));
    logo->setScale(0.5f);
    captured->addChild(logo);
}

void CObjectManager::CaptureZoomLayer()
{
    if(!m_CaptureNode) return;

    // render texture
    auto renderTexture = RenderTexture::create(1080, 1920);
    renderTexture->begin();

    m_Background->cocos2d::Node::visit();
    CGameScene::getZoomLayer()->cocos2d::Node::visit();

    renderTexture->end();

    auto texture = renderTexture->getSprite()->getTexture();
    m_CaptureNode->removeAllChildren();
    m_CaptureNode->setTexture(texture);
}
