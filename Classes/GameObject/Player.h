#pragma once
#include "GameObject.h"
#include "Planet.h"
#include "BulletCreator.h"
#include "Bullet/Bullet.h"
#include "../AI/StateMachine.h"
#include "../DataManager/CharacterDataManager.h"

/* ----------------HealthCalculatorInNormal에 대해서----------------
* Player클래스에 virtual로 계산 함수를 구현할까 했지만
* 이후 똑같은 캐릭터라도 상황, 버프, 특성 등 
* 변경요소 마다 다르게 적용시키고 싶어서 콜백함수로 구현함
*-----------------------------------------------------------------*/

namespace PLAYER_DEFINE{
    static const float NORMAL_BOUNDING_RADIUS = 35;
    static const float GIANT_BOUNDING_RADIUS  = 100;
    static const float NORMAL_SCALE = 1.f;
    static const float INVINCIVILITY_TIME = 5.f;
    static const float NORMAL_ROTATION_SPEED = ((PLANET_DEFINE::BOUNDING_RADIUS / NORMAL_BOUNDING_RADIUS) * BULLETCREATOR::ROTATION_SPEED);
    static const float GIANT_ROTATION_SPEED = NORMAL_ROTATION_SPEED * 0.7f;
    static const cocos2d::Vec2 POSITION
    = cocos2d::Vec2(540, 960 + (PLANET_DEFINE::BOUNDING_RADIUS) + (PLAYER_DEFINE::NORMAL_BOUNDING_RADIUS * 0.8f));
};

struct COSTUME;
class CScoreUI;
class CItemRange;
class CPlayer : public CGameObject {
public:
	static CPlayer* create();

	virtual void Execute(float delta = 0.f) override;
    void Clear();
    
	void GotSomeHealth(float health);
	void LostSomeHealth(CBullet* data);
	void GiantMode();
	void NormalMode();
	void TakeOnRocket();
	void TakeOffRocket();
	void StartBonusTime();
	void EndBonusTime();
	void Rotation(float speed);
    void CrownEnable(bool enable);
    void ParticleVisible(bool visible)
    { m_Particle->setVisible(visible); };
    void ChangeState(CState<CPlayer>* newState)
    { m_FSM->ChangeState(newState); };
    
	//인자로 전달된 아이템의 영향을 받는다
	void setItemEffect(int item){ m_EffectItemTypes |= item; }

	//인자로 전달된 아이템의 영향을 받는 bullet인지 검사한다.
	bool IsEffectWithItem(eITEM_FLAG itemType){ return on(itemType); }

	//현재 bullet이 영향을 받는 모든 플래그를 반환함
	int getItemEffect(){ return m_EffectItemTypes; }

	//callback
	float HealthCalculatorInNormal(float delta);	// 평소에 적용되는 생명력 계산함수
	float HealthCalculatorInBonusTime(float delta); // 보너스 타임에 적용되는 생명력 계산함수
	
	// 좌우로 흔들림
	void Crushed();
    
	// 게임 시작할 때
    void GameStart();
    
    // Game resume
	void PlayerAlive();

	// 게임 끝났을 때
	void PlayerDead();

	// time 동안무적
	void InvincibilityMode(float time);
    
	//getter & setter
	void setCharacterParam(const CHARACTER* data);
    void setCostumeParam(const COSTUME* data);
protected:
	virtual bool init() override;

	//getter & setter
	CC_SYNTHESIZE(float, m_Angle, Angle);
	CC_SYNTHESIZE(float, m_MaxLife, MaxLife);
	CC_SYNTHESIZE(float, m_Life, Life);
    CC_SYNTHESIZE(float, m_GiantScale, GiantScale);

private:
	bool on(eITEM_FLAG itemType){ return (m_EffectItemTypes & itemType) == itemType; }

    void createAliveParticle();
    void createDeadParticle();
    void createRunParticle();
	void setPlayerTexture(std::string textureName);
    void setParticleTexture(std::string name);
    void setParticleGiantMode();
    void setParticleNormalMode();
    
	CPlayer();
	virtual ~CPlayer(){}

private:
	std::shared_ptr<CStateMachine<CPlayer>> m_FSM;
	cocos2d::ParticleSystemQuad* m_Particle;
	const CHARACTER* m_CharacterParam;
    const COSTUME* m_CostumeParam;
    cocos2d::Sprite* m_Texture;
    cocos2d::Sprite* m_Costume;
    cocos2d::Sprite* m_Crown;
    
	bool m_Invincibility;

	// 영향을 받는 아이템 타입 
	// ex) m_EffectItemType == eITEM_TYPE_magnet 이면 자석아이템에게 영향력을 받는다.
	int m_EffectItemTypes;
};
