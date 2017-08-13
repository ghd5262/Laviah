#include "Player.h"
#include "ItemManager.h"
#include "ItemRange.h"
#include "ObjectManager.h"
#include "Rocket.h"
#include "../AI/States/PlayerStates.h"
#include "../AI/States/GameStates.h"
#include "../Particle/Particles.h"
#include "../Scene/GameScene.h"
#include "../MyUI/ScoreUI.h"
#include "../DataManager/UserDataManager.h"
#include "../DataManager/WorkshopItemDataManager.h"
#include "../DataManager/CostumeDataManager.hpp"

using namespace cocos2d;
using namespace PLAYER_DEFINE;

CPlayer* CPlayer::create()
{
	CPlayer *pRet = new(std::nothrow) CPlayer();
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

CPlayer::CPlayer()
: m_CharacterParam(nullptr)
, m_CostumeParam(nullptr)
, m_Texture(nullptr)
, m_Costume(nullptr)
, m_Angle(0.f)
, m_MaxLife(0)
, m_Life(0)
, m_GiantScale(0)
, m_EffectItemTypes(eITEM_FLAG_none)
, m_Particle(nullptr)
, m_Invincibility(false)
{}

bool CPlayer::init()
{
    if (!CGameObject::init()) return false;
      
    this->setItemEffect(eITEM_FLAG_giant);
    this->setCascadeOpacityEnabled(true);
    
    m_FSM = std::shared_ptr<CStateMachine<CPlayer>>(new CStateMachine<CPlayer>(this),
                                                    [=](CStateMachine<CPlayer>* fsm){
        delete fsm;
    });
    this->ChangeState(CPlayerNormal::Instance());
    
	m_CharacterParam = CCharacterDataManager::Instance()->getCurCharacter();
    m_Texture = Sprite::createWithSpriteFrameName(m_CharacterParam->_texture);
    if (m_Texture != nullptr){
        this->setContentSize(m_Texture->getContentSize());
        m_Texture->setPosition(this->getContentSize() / 2);
        m_Texture->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		addChild(m_Texture);
    }
    
    m_CostumeParam = CCostumeDataManager::Instance()->getCurCostumeByCharacter(m_CharacterParam->_index);
    m_Costume = Sprite::createWithSpriteFrameName(m_CostumeParam->_texture);
    if (m_Costume != nullptr){
        m_Costume->setPosition(m_Texture->getContentSize() / 2);
        m_Costume->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_Texture->addChild(m_Costume);
    }
    
    this->createRunParticle();
	this->setBoundingRadius(NORMAL_BOUNDING_RADIUS);
    return true;
}

void CPlayer::Execute(float delta)
{
	m_FSM->Execute(delta);
}

void CPlayer::Clear()
{
//    this->TakeOffRocket();
    m_MaxLife = 1;
    m_Texture->setRotation(0);
    m_Particle->setGravity(Vec2(0, -270));
    m_GiantScale = CUserDataManager::Instance()->getItemValueBySkillIndex(6);
    m_Particle->setPosition(this->getPosition());
}

void CPlayer::GameStart()
{
    m_Life = m_MaxLife;
    
    GVALUE->CURRENT_CHARACTER = m_CharacterParam->_index;
    GVALUE->CURRENT_COSTUME   = m_CostumeParam->_index;
    
    this->setVisible(true);
    this->setPlayerTexture(m_CharacterParam->_texture);
}
    
void CPlayer::PlayerAlive()
{
    m_Life = m_MaxLife;

    this->setVisible(false);
    this->createAliveParticle();
	this->setPlayerTexture(m_CharacterParam->_texture);
    this->InvincibilityMode(INVINCIVILITY_TIME); // 1초간 무적 카운트 끝나기 전부터 적용되기 때문에 실제로는 1.5초정도
    this->scheduleOnce([=](float delta){
        this->setVisible(true);
	}, 1.f, "PlayerAlive");

}

void CPlayer::PlayerDead(){
    CCLOG("Player dead");
    CObjectManager::Instance()->CaptureZoomLayer();
    m_Particle->setVisible(false);
    this->createDeadParticle();
    this->setVisible(false);
}

void CPlayer::GotSomeHealth(float health)
{
	if (0.f < m_Life) return;
    
	if (m_MaxLife > (m_Life + health))
	{
		m_Life += health;
	}
	else{
		m_Life = m_MaxLife;
	}
}

void CPlayer::LostSomeHealth(float loseHealth)
{
	if (m_Invincibility || !m_Life) return;
    
//    CAudioManager::Instance()->PlayEffectSound("sounds/hit.mp3", false);
	if (0.f < (m_Life - loseHealth))
	{
		m_Life -= loseHealth;
	}
	else{
        m_Life = 0.f;
		this->PlayerDead();
        CGameScene::getGameScene()->GameEnd();
    }
}

// Dir -1 == Left, 1 == Right
void CPlayer::Rotation(float speed)
{
	m_Angle = m_Texture->getRotation() + (speed * 5.5f);
//	m_Angle = static_cast<int>(m_Angle) % 360;
	m_Particle->setStartSpin(m_Angle);
	m_Particle->setEndSpin(m_Angle);
	m_Particle->setAngle(speed > 0 ? 180 : 0);
	m_Particle->setGravity(Vec2(-90 * (speed < 0 ? -1 : 1), 0));
	m_Texture->setRotation(m_Angle);

    GVALUE->RUN_SCORE += 1;
}

void CPlayer::GiantMode()
{
    CObjectManager::Instance()->GiantMode();
    
    auto action = Sequence::create(ScaleTo::create(0.5f, m_GiantScale),
                                   CallFunc::create([=](){
        this->setBoundingRadius(GIANT_BOUNDING_RADIUS * m_GiantScale);
        m_Particle->setStartSize(NORMAL_BOUNDING_RADIUS * m_GiantScale);
        m_Particle->setEndSize(40.f);
    }), nullptr);
    m_Texture->runAction(action);
}

void CPlayer::NormalMode()
{
    CObjectManager::Instance()->NormalMode();
    
    //1초간 무적
    InvincibilityMode(1.5f);
    auto action = Sequence::create(ScaleTo::create(0.5f, NORMAL_SCALE),
                                   CallFunc::create([=](){
        this->setBoundingRadius(NORMAL_BOUNDING_RADIUS);
        m_Particle->setStartSize(NORMAL_BOUNDING_RADIUS * 2.f);
        m_Particle->setEndSize(4.f);
    }), nullptr);
    m_Texture->runAction(action);
}

void CPlayer::TakeOnRocket()
{
	//this->retain();
	//auto rocket = CObjectManager::Instance()->getRocket();
	//this->removeFromParent();
	//rocket->addChild(this, -1);
	//this->setRotation(0);
	//this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	//this->setPosition(Vec2(rocket->getContentSize().width * 0.5f,
	//                       rocket->getContentSize().height * 0.6f));
	//m_Particle->setVisible(false);
	//    this->release();

	this->ChangeState(CPlayerBonusTime::Instance());
}

void CPlayer::TakeOffRocket()
{
	/*this->retain();
	this->removeFromParent();
	CGameScene::getZoomLayer()->addChild(this, ZORDER::PLAYER);*/
	//this->setPosition(this->getOriginPos());
	//m_Particle->setVisible(true);
	//this->release();
	this->ChangeState(CPlayerNormal::Instance());
}

void CPlayer::StartBonusTime()
{
    m_Particle->setVisible(false);
}

void CPlayer::EndBonusTime()
{
	this->setPosition(PLAYER_DEFINE::POSITION);
	m_Particle->setVisible(true);
}

// callback 평소에 적용되는 생명력 계산함수
float CPlayer::HealthCalculatorInNormal(float delta)
{
	// 5.0f == 가장 레벨이 낮을때 한 번에 빠지는 생명력의 양
	// 이후에 펫 효과나 버프 등과 함께 계산해야한다.
	//LostSomeHealth(1.0f * delta);
	return (m_Life / m_MaxLife) * 100;
}

// callback 보너스 타임에 적용되는 생명력 계산함수
float CPlayer::HealthCalculatorInBonusTime(float delta)
{
	// 5.0f == 가장 레벨이 낮을때 한 번에 빠지는 생명력의 양
	// 이후에 펫 효과나 버프 등과 함께 계산해야한다.
	// 보너스 타임이기 때문에 더느리게 줄어든다.
	//LostSomeHealth(0.5f);
	return (m_Life / m_MaxLife) * 100;
}

void CPlayer::Crushed()
{
    CObjectManager::Shake(0.01f, 0.2f, 0.1f, 3.0f, m_Texture, this->getContentSize() / 2);
}

void CPlayer::InvincibilityMode(float time)
{
    if(this->isScheduled("SetPlayerNormalMode"))
        this->unschedule("SetPlayerNormalMode");
        
	m_Texture->setOpacity(100);
	m_Invincibility = true;
	this->scheduleOnce([=](float delta){
		m_Texture->setOpacity(255);
		m_Invincibility = false;
	}, time, "SetPlayerNormalMode");
}

void CPlayer::setCharacterParam(const CHARACTER* data)
{
	m_CharacterParam = data;
    this->setPlayerTexture(m_CharacterParam->_texture);
}

void CPlayer::setCostumeParam(const COSTUME* data)
{
    m_CostumeParam = data;
    if(m_Costume) m_Costume->setSpriteFrame(m_CostumeParam->_texture);
}

void CPlayer::setPlayerTexture(std::string textureName)
{
	if (m_Texture != nullptr){
		m_Texture->setSpriteFrame(textureName);
	}

	if (m_Particle != nullptr){
		SpriteFrame* spriteFrame = nullptr;
		spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(textureName);

		if (spriteFrame != nullptr)
		{
			m_Particle->setTextureWithRect(spriteFrame->getTexture(), spriteFrame->getRect());
		}
	}
}

void CPlayer::createAliveParticle()
{
	auto particle = CParticle_Explosion_2::create(m_CharacterParam->_texture);
    if (particle != nullptr){
        particle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        particle->setPosition(this->getPosition());
        particle->setStartRadius(200);
        particle->setEndRadius(0);
        particle->setDuration(1.0f);
        CGameScene::getZoomLayer()->addChild(particle, ZORDER::PLAYER);
    }
}

void CPlayer::createDeadParticle()
{
	auto particle = CParticle_Explosion_2::create(m_CharacterParam->_texture);
    if (particle != nullptr){
        particle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        particle->setPosition(this->getPosition());
        CGameScene::getZoomLayer()->addChild(particle, ZORDER::PLAYER);
    }
}

void CPlayer::createRunParticle()
{
	m_Particle = CParticle_Flame::create(m_CharacterParam->_texture);
    if (m_Particle != nullptr){
        m_Particle->setPosition(this->getPosition());
        m_Particle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_Particle->setAngle(90);
        m_Particle->setGravity(Vec2(0, -270));
        m_Particle->setStartSize(NORMAL_BOUNDING_RADIUS * 2.f);
        m_Particle->setEndSize(4.f);
		CGameScene::getZoomLayer()->addChild(m_Particle, ZORDER::PLAYER-1);
    }
}
