#include "Player.h"
#include "Planet.h"
#include "ItemManager.h"
#include "ItemBarrier.h"
#include "ObjectManager.h"
#include "MagnetEffect.h"
#include "BulletCreator.h"
#include "../AI/States/PlayerStates.h"
#include "../AI/States/StageStates.h"
#include "../Particle/Particles.h"
#include "../Scene/GameScene.h"
#include "../MyUI/ScoreUI.h"
#include "../DataManager/UserDataManager.h"
#include "../MyUI/MultipleScore.h"

namespace PLAYER_DEFINE{
	static const float SCALE_SIZE = 1.5f;
	static const float NORMAL_BOUNDING_RADIUS = 25.f * SCALE_SIZE;
	static const float GIANT_BOUNDING_RADIUS = NORMAL_BOUNDING_RADIUS * 3.f;
	static const float GIANT_SIZE_PERCENT = 2.f;
	static const float INVINCIVILITY_TIME = 5.f;
	static const float NORMAL_ROTATION_SPEED = ((PLANET_DEFINE::BOUNDING_RADIUS / NORMAL_BOUNDING_RADIUS) * BULLETCREATOR::ROTATION_SPEED);
	static const float GIANT_ROTATION_SPEED = NORMAL_ROTATION_SPEED * 0.7f;
};

using namespace cocos2d;
using namespace PLAYER_DEFINE;

CPlayer* CPlayer::create(sCHARACTER_PARAM characterParam)
{
	CPlayer *pRet = new(std::nothrow) CPlayer(characterParam);
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

CPlayer::CPlayer(sCHARACTER_PARAM characterParam)
: m_CharacterParam(characterParam)
, m_Angle(0.f)
, m_fRotateSpeed(0.f)
, m_fMaxLife(characterParam._health)
, m_fLife(characterParam._health)
, m_fMagnetLimitTime(0)
, m_fMagnetLimitRadius(0)
, m_EffectItemTypes(eITEM_FLAG_none)
, m_pParticle(nullptr)
, m_pItemBarrier(nullptr)
, m_MagnetEffect(nullptr)
, m_Invincibility(false)
, m_MultipleScore(nullptr)
{}

bool CPlayer::init()
{
    if (!CGameObject::init()) return false;
    
    m_fMagnetLimitTime = m_CharacterParam._magnetItemTime + CUserDataManager::Instance()->getItemCurrentValue(USERDATA_KEY::ITEM_TIME_MAGNET);
    m_fMagnetLimitRadius = m_CharacterParam._magnetItemSize + CUserDataManager::Instance()->getItemCurrentValue(USERDATA_KEY::ITEM_SIZE_MAGNET);
    m_fCoinLimitTime = m_CharacterParam._coinItemTime + CUserDataManager::Instance()->getItemCurrentValue(USERDATA_KEY::ITEM_TIME_COIN);
    m_fStarLimitTime = m_CharacterParam._starItemTime + CUserDataManager::Instance()->getItemCurrentValue(USERDATA_KEY::ITEM_TIME_STAR);
    m_fBonusTimeLimitTime = m_CharacterParam._bonusItemTime + CUserDataManager::Instance()->getItemCurrentValue(USERDATA_KEY::ITEM_TIME_BOUNS);
    m_fGiantLimitTime = m_CharacterParam._giantItemTime + CUserDataManager::Instance()->getItemCurrentValue(USERDATA_KEY::ITEM_TIME_GIANT);
    
    this->setItemEffect(eITEM_FLAG_giant);
    this->setCascadeOpacityEnabled(true);
    
    m_FSM = std::shared_ptr<CStateMachine<CPlayer>>( new CStateMachine<CPlayer>(this),
                                                    [](CStateMachine<CPlayer>* fsm){
        delete fsm;
    });
    
    if (m_FSM.get() != nullptr){
        m_FSM.get()->ChangeState(CPlayerNormal::Instance());
    }
    
    m_pItemBarrier = CItemBarrier::create("barrier2.png", 800.f);
    if (m_pItemBarrier != nullptr){
        m_pItemBarrier->setPosition(this->getContentSize() / 2);
        m_pItemBarrier->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        addChild(m_pItemBarrier);
    }
    
    m_pTexture = Sprite::createWithSpriteFrameName(m_CharacterParam._normalTextureName);
    if (m_pTexture != nullptr){
        this->setContentSize(m_pTexture->getContentSize());
        m_pTexture->setPosition(this->getContentSize() / 2);
        m_pTexture->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		addChild(m_pTexture);
    }
    
    m_MagnetEffect = CMagnetEffect::create("barrier.png", m_fMagnetLimitRadius, m_fMagnetLimitTime);
    if (m_MagnetEffect != nullptr)
    {
        m_MagnetEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        CGameScene::getGameScene()->addChild(m_MagnetEffect);
    }
    
    this->createRunParticle();
    this->setScale(SCALE_SIZE);
	this->setBoundingRadius(NORMAL_BOUNDING_RADIUS);
    return true;
}

void CPlayer::Execute(float delta)
{
	m_FSM->Execute(delta);
    m_pItemBarrier->Execute(delta);
    m_MagnetEffect->Execute(delta);
}

void CPlayer::Clear()
{
    this->setRotation(0);
    m_pParticle->setVisible(false);
    m_pParticle->setAngle(90);
    m_pParticle->setRotation(0);
    m_pParticle->setGravity(Vec2(0, -270));
    CMultipleScore::Instance()->UpdateScore();
}

void CPlayer::PlayerAlive(){
    this->setVisible(false);
    this->createAliveParticle();
	this->setPlayerTexture(m_CharacterParam._aliveTextureName);

	this->scheduleOnce([=](float delta){
		this->setVisible(true);
		this->setPlayerTexture(m_CharacterParam._normalTextureName);
		this->InvincibilityMode(INVINCIVILITY_TIME); // 1초간 무적 카운트 끝나기 전부터 적용되기 때문에 실제로는 1.5초정도
	}, 1.5f, "PlayerAlive");

}

void CPlayer::PlayerDead(){
    m_pParticle->setVisible(false);
    this->createDeadParticle();
	this->setVisible(false);
}

void CPlayer::GotSomeHealth(float health)
{
	if (0.f < m_fLife) return;
    
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
	if (m_Invincibility) return;
    
    CAudioManager::Instance()->PlayEffectSound("sounds/hit.mp3", false);
	if (0.f < (m_fLife - loseHealth))
	{
		m_fLife -= loseHealth;
	}
	else{
		this->PlayerDead();
        CMultipleScore::Instance()->UpdateScore();
		CGameScene::getGameScene()->WatchVideo();
		m_fLife = 0.f;
	}
}

// Dir -1 == Left, 1 == Right
void CPlayer::Rotation(float dir, float delta)
{
    if(!m_pParticle->isVisible())
        m_pParticle->setVisible(true);
    
	m_Angle = this->getRotation() + (dir * m_fRotateSpeed * delta);
//	m_Angle = static_cast<int>(m_Angle) % 360;
	m_pParticle->setStartSpin(m_Angle);
	m_pParticle->setEndSpin(m_Angle);
	m_pParticle->setAngle(dir == 1 ? 180 : 0);
	m_pParticle->setGravity(Vec2(-90 * dir, 0));
	this->setRotation(m_Angle);

    GLOBAL->RUNSCORE += 1;
}

void CPlayer::GiantMode()
{
	auto action = Sequence::create(
		ScaleTo::create(0.5f, GIANT_SIZE_PERCENT),
		CallFunc::create([=](){
		this->setPlayerTexture(m_CharacterParam._giantTextureName);
		this->m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
		this->setBoundingRadius(GIANT_BOUNDING_RADIUS);
		this->setRotateSpeed(PLAYER_DEFINE::GIANT_ROTATION_SPEED);
		m_pParticle->setStartSize(NORMAL_BOUNDING_RADIUS * 4.f);
		m_pParticle->setEndSize(40.f);
	}), nullptr);
	this->runAction(action);
}

void CPlayer::NormalMode()
{
    //1초간 무적
    InvincibilityMode(1.5f);
	auto action = Sequence::create(
		ScaleTo::create(0.5f, SCALE_SIZE),
		CallFunc::create([=](){
        this->setPlayerTexture(m_CharacterParam._normalTextureName);
		this->m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
		this->setBoundingRadius(NORMAL_BOUNDING_RADIUS);
		this->setRotateSpeed(PLAYER_DEFINE::NORMAL_ROTATION_SPEED);
		m_pParticle->setStartSize(NORMAL_BOUNDING_RADIUS * 2.f);
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
	if (!m_Invincibility){
		this->runAction(
			Sequence::create(
			Repeat::create(
			Sequence::create(
			MoveBy::create(duration / stackCount, Vec2(stackSizeLR, -stackSizeTB)),
			MoveBy::create(duration / stackCount, Vec2(-stackSizeLR, stackSizeTB)), nullptr), stackCount),
			CallFunc::create([=](){this->setPosition(m_OriginPos); }), nullptr));
	}
}

void CPlayer::GotBarrierItem()
{
    m_pItemBarrier->GotBarrierItem();
}

void CPlayer::GotMagnetItem()
{
    m_MagnetEffect->GotMagnetItem();
}

void CPlayer::InvincibilityMode(float time)
{
	this->m_pTexture->setOpacity(100);
	m_Invincibility = true;
	this->scheduleOnce([=](float delta){
		this->m_pTexture->setOpacity(255);
		m_Invincibility = false;
	}, time, "SetPlayerNormalMode");
}

void CPlayer::setPlayerTexture(std::string textureName)
{
	if (m_pTexture != nullptr){
		m_pTexture->setSpriteFrame(textureName);
	}
}

void CPlayer::createAliveParticle()
{
    auto particle = CParticle_Explosion_2::create();
    if (particle != nullptr){
        particle->setTextureName(m_CharacterParam._deadParticleTextureName);
        particle->setTotalParticles(300);
        particle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        particle->setPosition(getPosition());
        particle->setStartRadius(160);
        particle->setEndRadius(0);
        particle->setDuration(0.5f);
        CGameScene::getGameScene()->addChild(particle, ZORDER::PLAYER);
    }
}

void CPlayer::createDeadParticle()
{
    auto particle = CParticle_Explosion_2::create();
    if (particle != nullptr){
        particle->setTextureName(m_CharacterParam._deadParticleTextureName);
        particle->setTotalParticles(300);
        particle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        particle->setPosition(this->getPosition());
        CGameScene::getGameScene()->addChild(particle, ZORDER::PLAYER);
    }
}

void CPlayer::createRunParticle()
{
    m_pParticle = CParticle_Flame::create();
    if (m_pParticle != nullptr){
        m_pParticle->setTextureName(m_CharacterParam._normalTextureName);
        m_pParticle->setTotalParticles(50);
        m_pParticle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_pParticle->setAngle(90);
        m_pParticle->setGravity(Vec2(0, -270));
        m_pParticle->setStartSize(NORMAL_BOUNDING_RADIUS * 2.f);
        m_pParticle->setEndSize(4.f);
        CGameScene::getGameScene()->addChild(m_pParticle);
        m_pParticle->setVisible(false);
    }
}