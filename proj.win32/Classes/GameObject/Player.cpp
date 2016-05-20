#include "Player.h"
#include "ItemManager.h"
#include "ItemBarrier.h"
#include "ObjectManager.h"
#include "MagnetEffect.h"
#include "../AI/States/PlayerStates.h"
#include "../AI/States/StageStates.h"
#include "../Particle/Particles.h"
#include "../Scene/GameScene.h"
#include "../MyUI/ScoreUI.h"
#include "../MyUI/UIManager.h"

CPlayer* CPlayer::create(
	std::string normalTextureName,
	std::string giantTextureName,
	float boundingRadius,
	float angle,
	float rotateSpeed, 
	float maxLife)
{
	CPlayer *pRet = new(std::nothrow) CPlayer(
		normalTextureName
		, giantTextureName
		, boundingRadius
		, angle
		, rotateSpeed
		, maxLife);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

CPlayer::CPlayer(
	std::string normalTextureName,
	std::string giantTextureName,
	float boundingRadius,
	float angle,
	float rotateSpeed,
	float maxLife)

	: CGameObject(boundingRadius)
	, m_NormalTextureName(normalTextureName)
	, m_GiantTextureName(giantTextureName)
	, m_fAngle(angle)
	, m_fRotateSpeed(rotateSpeed)
	, m_fMaxLife(maxLife)
	, m_fLife(maxLife)
	, m_fMagnetLimitRadius(200.f)
	, m_EffectItemTypes(eITEM_FLAG_none)
	, m_pParticle(nullptr)
	, m_pParticleDead(nullptr)
	, m_pParticleAlive(nullptr)
	, m_isRoatating(false)
	, m_pUIRunScore(nullptr)
	, m_pItemBarrier(nullptr)
	, m_isPlayerDead(true)
    , m_MagnetEffect(nullptr)
{
}

bool CPlayer::init()
{
	//this->DrawDebugBinding();
	if (!initVariable())
		return false;
	return true;
}

bool CPlayer::initVariable()
{
	try{
		setItemEffect(eITEM_FLAG_giant);
        setCascadeOpacityEnabled(true);
        m_FSM = std::shared_ptr<CStateMachine<CPlayer>>(
            new CStateMachine<CPlayer>(this), [](CStateMachine<CPlayer>* fsm)
            {
                delete fsm;
            });
        
		if (m_FSM.get() != nullptr){
            m_FSM.get()->ChangeState(CPlayerNormal::Instance());
		}
        
        m_pItemBarrier = CItemBarrier::create("barrier2.png", 800.f);
        if(m_pItemBarrier != nullptr){
            m_pItemBarrier->setPosition(Vec2(0, 0));
            m_pItemBarrier->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            addChild(m_pItemBarrier);
        }
        
		m_pTexture = Sprite::create(m_NormalTextureName);
		if (m_pTexture != nullptr){
			m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
			m_pTexture->setScale(0.5f);
			addChild(m_pTexture);
			m_pTexture->setVisible(false);
		}

		m_pParticle = CParticle_Flame::create(m_NormalTextureName);
		if (m_pParticle != nullptr){
			m_pParticle->retain();
			m_pParticle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_pParticle->setAngle(90);
			m_pParticle->setGravity(Vec2(0, -270));
			CGameScene::getGridWorld()->addChild(m_pParticle, 10);
			m_pParticle->setVisible(false);
		}
        
        m_MagnetEffect = CMagnetEffect::create("barrier.png", m_fMagnetLimitRadius, 8.f);
        if(m_MagnetEffect != nullptr)
        {
            m_MagnetEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            CGameScene::getGridWorld()->addChild(m_MagnetEffect);
        }
        
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void CPlayer::Execute(float delta)
{
	if (m_isPlayerDead == true)
		return;
	m_FSM->Execute(delta);
    m_pItemBarrier->Execute(delta);
    m_MagnetEffect->Execute(delta);
	if (!m_isRoatating)
	{
		m_pParticle->setAngle(90);
		m_pParticle->setGravity(Vec2(0, -270));
	}
	m_isRoatating = false;
}


void CPlayer::PlayerAlive(){

	m_pParticleAlive = CParticle_Explosion_2::create("whiteSquare.png");
	if (m_pParticleAlive != nullptr){
		m_pParticleAlive->retain();
		m_pParticleAlive->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_pParticleAlive->setPosition(getPosition());
		m_pParticleAlive->setStartRadius(160);
		m_pParticleAlive->setEndRadius(0);
		m_pParticleAlive->setDuration(0.5f);
		CGameScene::getGridWorld()->addChild(m_pParticleAlive, 100);
	}

	this->scheduleOnce([this](float delta){
		m_isPlayerDead = false;
		m_pParticle->setVisible(true);
		m_pTexture->setVisible(true);
	}, 1.5f, "PlayerAlive");

}

void CPlayer::PlayerDead(){
	m_pParticleDead = CParticle_Explosion_2::create("whiteSquare.png");
	if (m_pParticleDead != nullptr){
		m_pParticleDead->retain();
		m_pParticleDead->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_pParticleDead->setPosition(getPosition());
		CGameScene::getGridWorld()->addChild(m_pParticleDead, 100);
	}
    m_isPlayerDead = true;
	m_pParticle->setVisible(false);
	m_pTexture->setVisible(false);
}

void CPlayer::GotSomeHealth(float health)
{
	if (m_isPlayerDead == true)
		return;
	if (m_fMaxLife > (m_fLife + health))
	{
		m_fLife += health;
	}
	else{
		m_fLife = m_fMaxLife;
	}
}

void CPlayer::LostSomeHealth(float loseHealth)
{
	if (m_isPlayerDead == true)
		return;
	if (0.f < (m_fLife - loseHealth))
	{
		m_fLife -= loseHealth;
	}
	else{
		CGameScene::getGameScene()->GameEnd();
		m_fLife = 0.f;
	}
}

// Dir -1 == Left, 1 == Right
void CPlayer::Rotation(float dir, float delta)
{
	m_isRoatating = true;
	m_fAngle = this->getRotation() + (dir * m_fRotateSpeed * delta);
	m_fAngle = static_cast<int>(m_fAngle) % 360;
	m_pParticle->setStartSpin(m_fAngle);
	m_pParticle->setEndSpin(m_fAngle);
	m_pParticle->setAngle(dir == 1 ? 180 : 0);
	m_pParticle->setGravity(Vec2(-90 * dir, 0));
	this->setRotation(m_fAngle);
    if(m_pUIRunScore == nullptr)
        m_pUIRunScore = static_cast<CScoreUI*>(CUIManager::Instance()->FindUIWithName("RunScoreUI"));
    m_pUIRunScore->UpdateValue(1);
}

void CPlayer::GiantMode()
{
	auto action = Sequence::create(
		ScaleTo::create(0.5f, 3.0f),
		CallFunc::create([&](){
		this->m_pTexture->setTexture(m_GiantTextureName); 
		this->m_pTexture->setAnchorPoint(Vec2(0.5f, 0.3f));
		this->setBRadius(80.f);
		m_pParticle->setStartSize(80.f);
		m_pParticle->setEndSize(40.f);
	}), nullptr);
	this->runAction(action);
}

void CPlayer::NormalMode()
{
	auto action = Sequence::create(
		ScaleTo::create(0.5f, 1.0f),
		CallFunc::create([&](){
		this->m_pTexture->setTexture(m_NormalTextureName);
		this->m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
		this->setBRadius(6.f);
		m_pParticle->setStartSize(30.f);
		m_pParticle->setEndSize(4.f);
	}), nullptr);
	this->runAction(action);
}

// callback 평소에 적용되는 생명력 계산함수
float CPlayer::HealthCalculatorInNormal(float delta)
{
	// 5.0f == 가장 레벨이 낮을때 한 번에 빠지는 생명력의 양
	// 이후에 펫 효과나 버프 등과 함께 계산해야한다.
	//LostSomeHealth(1.0f * delta);
	return (m_fLife / m_fMaxLife) * 100;
}

// callback 보너스 타임에 적용되는 생명력 계산함수
float CPlayer::HealthCalculatorInBonusTime(float delta)
{
	// 5.0f == 가장 레벨이 낮을때 한 번에 빠지는 생명력의 양
	// 이후에 펫 효과나 버프 등과 함께 계산해야한다.
	// 보너스 타임이기 때문에 더느리게 줄어든다.
	//LostSomeHealth(0.5f);
	return (m_fLife / m_fMaxLife) * 100;
}

void CPlayer::setParticlePos(Vec2 pos){
    m_pParticle->setPosition(pos);
    m_MagnetEffect->setPosition(pos);
}

void CPlayer::StackedRL(float duration, float stackSizeLR, float stackSizeTB, int stackCount)
{
	this->runAction(
        Sequence::create(
        Repeat::create(
        Sequence::create(
		MoveBy::create(duration / stackCount, Vec2(stackSizeLR, -stackSizeTB)),
		MoveBy::create(duration / stackCount, Vec2(-stackSizeLR, stackSizeTB)), nullptr), stackCount),
        CallFunc::create([this](){this->setPosition(m_OriginPos);}), nullptr));
}

void CPlayer::GotBarrierItem()
{
    CCLOG("Player GotBerrierItem");
    m_pItemBarrier->GotBarrierItem();
}

void CPlayer::GotMagnetItem()
{
    m_MagnetEffect->GotMagnetItem();
}
