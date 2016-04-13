#include "Player.h"
#include "ItemManager.h"
#include "ItemBarrier.h"
#include "../AI/States/PlayerStates.h"
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
	, m_isRoatating(false)
    , m_pUIRunScore(nullptr)
    , m_pItemBarrier(nullptr)
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
		}

		m_pParticle = CParticle_Flame::create(m_NormalTextureName);
		if (m_pParticle != nullptr){
			m_pParticle->retain();
			m_pParticle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			CGameScene::getGameScene()->addChild(m_pParticle, 10);
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
	m_FSM->Execute(delta);
    m_pItemBarrier->Execute(delta);
	if (!m_isRoatating)
	{
		m_pParticle->setAngle(90);
		m_pParticle->setGravity(Vec2(0, -270));
	}
	m_isRoatating = false;
}

void CPlayer::GotSomeHealth(float health)
{
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
	if (0.f < (m_fLife - loseHealth))
	{
		m_fLife -= loseHealth;
	}
	else{
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
	LostSomeHealth(1.0f * delta);
	return (m_fLife / m_fMaxLife) * 100;
}

// callback 보너스 타임에 적용되는 생명력 계산함수
float CPlayer::HealthCalculatorInBonusTime(float delta)
{
	// 5.0f == 가장 레벨이 낮을때 한 번에 빠지는 생명력의 양
	// 이후에 펫 효과나 버프 등과 함께 계산해야한다.
	// 보너스 타임이기 때문에 더느리게 줄어든다.
	LostSomeHealth(0.5f);
	return (m_fLife / m_fMaxLife) * 100;
}

void CPlayer::StackedRL(float duration, float stackSizeLR, float stackSizeTB, int stackCount)
{
	m_pTexture->runAction(
		Repeat::create(
		Sequence::create(
		MoveBy::create(duration / stackCount, Vec2(stackSizeLR, -stackSizeTB)),
		MoveBy::create(duration / stackCount, Vec2(-stackSizeLR, stackSizeTB)),
		nullptr), stackCount));

}

void CPlayer::GotBarrierItem()
{
    CCLOG("Player GotBerrierItem");
    m_pItemBarrier->GotBarrierItem();
}
