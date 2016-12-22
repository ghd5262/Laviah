#pragma once
#include "GameObject.h"
#include "Bullet/Bullet.h"
#include "../AI/StateMachine.h"
#include "../DataManager/CharacterDataManager.h"

/* ----------------HealthCalculatorInNormal에 대해서----------------
* Player클래스에 virtual로 계산 함수를 구현할까 했지만
* 이후 똑같은 캐릭터라도 상황, 버프, 특성 등 
* 변경요소 마다 다르게 적용시키고 싶어서 콜백함수로 구현함
*-----------------------------------------------------------------*/

class CScoreUI;
class CItemBarrier;
class CMagnetEffect;
class CMultipleScore;
class CPlayer : public CGameObject {
public:
	static CPlayer* create();

	virtual void Execute(float delta = 0.f) override;
    void Clear();
    
	void GotSomeHealth(float health);
	void LostSomeHealth(float loseHealth);
    void GotBarrierItem();
    void GotMagnetItem();
	void GiantMode();
	void NormalMode();
    void TakeOnRocket();
    void TakeOffRocket();
	void Rotation(float speed);
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
	void StackedRL(float duration, float stackSizeLR, float stackSizeTB, int stackCount);

	// 게임 시작할 때
	void PlayerAlive();

	// 게임 끝났을 때
	void PlayerDead();

	// time 동안무적
	void InvincibilityMode(float time);

	void ChangeDataByCharacter();

	//getter & setter
	void setCharacterParam(sCHARACTER_PARAM data);
	void setOriginPos(cocos2d::Vec2 pos) { m_OriginPos = pos; }
	cocos2d::Vec2 getOriginPos(){ return m_OriginPos; }
    void setParticlePos(cocos2d::Vec2 pos);

protected:
	virtual bool init() override;

	//getter & setter
	CC_SYNTHESIZE(float, m_Angle, Angle);
	CC_SYNTHESIZE(float, m_fRotateSpeed, RotateSpeed);
	CC_SYNTHESIZE(float, m_fMaxLife, MaxLife);
	CC_SYNTHESIZE(float, m_fLife, Life);
	CC_SYNTHESIZE(float, m_fLevel, Level);
	CC_SYNTHESIZE(float, m_fMagnetLimitTime, MagnetLimitTime);
	CC_SYNTHESIZE(float, m_fMagnetLimitRadius, MagnetLimitRadius);
	CC_SYNTHESIZE(float, m_fCoinLimitTime, CoinLimitTime);
	CC_SYNTHESIZE(float, m_fStarLimitTime, StarLimitTime);
	CC_SYNTHESIZE(float, m_fGiantLimitTime, GiantLimitTime);
	CC_SYNTHESIZE(float, m_fBonusTimeLimitTime, BonusTimeLimitTime);
	CC_SYNTHESIZE(CItemBarrier*, m_pItemBarrier, ItemBarrier);
    
private:
	bool on(eITEM_FLAG itemType){ return (m_EffectItemTypes & itemType) == itemType; }

    void createAliveParticle();
    void createDeadParticle();
    void createRunParticle();
	void setPlayerTexture(std::string textureName);
    
	CPlayer();
	virtual ~CPlayer(){}

private:
	std::shared_ptr<CStateMachine<CPlayer>> m_FSM;
	CMultipleScore* m_MultipleScore;
	cocos2d::Vec2 m_OriginPos;
	cocos2d::Sprite* m_pTexture;
	cocos2d::ParticleSystemQuad* m_Particle;
    CMagnetEffect* m_MagnetEffect;
	sCHARACTER_PARAM m_CharacterParam;
	bool m_Invincibility;

	// 영향을 받는 아이템 타입 
	// ex) m_EffectItemType == eITEM_TYPE_magnet 이면 자석아이템에게 영향력을 받는다.
	int m_EffectItemTypes;
};