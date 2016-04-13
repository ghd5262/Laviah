#pragma once
#include "GameObject.h"
#include "Bullet/Bullet.h"
#include "../AI/StateMachine.h"

/* ----------------HealthCalculatorInNormal에 대해서----------------
* Player클래스에 virtual로 계산 함수를 구현할까 했지만
* 이후 똑같은 캐릭터라도 상황, 버프, 특성 등 
* 변경요소 마다 다르게 적용시키고 싶어서 콜백함수로 구현함
*-----------------------------------------------------------------*/

class CScoreUI;
class CItemBarrier;
class CPlayer : public CGameObject {
public:
	static CPlayer* create(
		std::string normalTextureName,
		std::string giantTextureName, 
		float boundingRadius, 
		float angle, 
		float rotateSpeed,
		float maxLife);

	virtual void Execute(float delta = 0.f) override;

	void GotSomeHealth(float health);
	void LostSomeHealth(float loseHealth);
    void GotBarrierItem();
	void GiantMode();
	void NormalMode();
	void Rotation(float dir, float delta);
	
	//인자로 전달된 아이템의 영향을 받는다
	void setItemEffect(int item){ m_EffectItemTypes |= item; }

	//인자로 전달된 아이템의 영향을 받는 bullet인지 검사한다.
	bool isEffectWithItem(eITEM_FLAG itemType){ return on(itemType); }

	//현재 bullet이 영향을 받는 모든 플래그를 반환함
	int getItemEffect(){ return m_EffectItemTypes; }

	//callback
	float HealthCalculatorInNormal(float delta);	// 평소에 적용되는 생명력 계산함수
	float HealthCalculatorInBonusTime(float delta); // 보너스 타임에 적용되는 생명력 계산함수
	
	// 좌우로 흔들림
	void StackedRL(float duration, float stackSizeLR, float stackSizeTB, int stackCount);

	//getter & setter
	CStateMachine<CPlayer>* getFSM(){ return m_FSM.get(); }
	void setOriginPos(cocos2d::Vec2 pos) { m_OriginPos = pos; }
	cocos2d::Vec2 getOriginPos(){ return m_OriginPos; }
	void setParticlePos(Vec2 pos){ m_pParticle->setPosition(pos); }

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

	//getter & setter
	CC_SYNTHESIZE(float, m_fAngle, Angle);
	CC_SYNTHESIZE(float, m_fRotateSpeed, RotateSpeed);
	CC_SYNTHESIZE(float, m_fLife, Life);
	CC_SYNTHESIZE(float, m_fMaxLife, MaxLife);
	CC_SYNTHESIZE(float, m_fLevel, Level);
	CC_SYNTHESIZE(float, m_fMagnetLimitRadius, MagnetLimitRadius);
	CC_SYNTHESIZE(CItemBarrier*, m_pItemBarrier, ItemBarrier);

private:
	bool on(eITEM_FLAG itemType){ return (m_EffectItemTypes & itemType) == itemType; }

	CPlayer(
		std::string normalTextureName,
		std::string giantTextureName,
		float boundingRadius,
		float rotate,
		float rotateSpeed,
		float maxLife);
	virtual ~CPlayer(){}

private:
	std::shared_ptr<CStateMachine<CPlayer>> m_FSM;
	cocos2d::Vec2 m_OriginPos;
	std::string m_NormalTextureName;
	std::string m_GiantTextureName;
	Sprite* m_pTexture;
	ParticleSystemQuad* m_pParticle;
	bool m_isRoatating;
    CScoreUI* m_pUIRunScore;
    
    
	// 영향을 받는 아이템 타입 
	// ex) m_EffectItemType == eITEM_TYPE_magnet 이면 자석아이템에게 영향력을 받는다.
	int m_EffectItemTypes;
};