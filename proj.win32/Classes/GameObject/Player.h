#pragma once
#include "GameObject.h"
#include "Bullet/Bullet.h"
#include "../AI/StateMachine.h"

/* ----------------HealthCalculatorInNormal�� ���ؼ�----------------
* PlayerŬ������ virtual�� ��� �Լ��� �����ұ� ������
* ���� �Ȱ��� ĳ���Ͷ� ��Ȳ, ����, Ư�� �� 
* ������ ���� �ٸ��� �����Ű�� �; �ݹ��Լ��� ������
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
	
	//���ڷ� ���޵� �������� ������ �޴´�
	void setItemEffect(int item){ m_EffectItemTypes |= item; }

	//���ڷ� ���޵� �������� ������ �޴� bullet���� �˻��Ѵ�.
	bool isEffectWithItem(eITEM_FLAG itemType){ return on(itemType); }

	//���� bullet�� ������ �޴� ��� �÷��׸� ��ȯ��
	int getItemEffect(){ return m_EffectItemTypes; }

	//callback
	float HealthCalculatorInNormal(float delta);	// ��ҿ� ����Ǵ� ����� ����Լ�
	float HealthCalculatorInBonusTime(float delta); // ���ʽ� Ÿ�ӿ� ����Ǵ� ����� ����Լ�
	
	// �¿�� ��鸲
	void StackedRL(float duration, float stackSizeLR, float stackSizeTB, int stackCount);

	// ���� ������ ��
	void PlayerAlive();

	// ���� ������ ��
	void PlayerDead();

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
	CC_SYNTHESIZE(bool, m_isPlayerDead, IsDead);
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
	ParticleSystemQuad* m_pParticleDead;
	ParticleSystemQuad* m_pParticleAlive;
	bool m_isRoatating;
    CScoreUI* m_pUIRunScore;
    
	// ������ �޴� ������ Ÿ�� 
	// ex) m_EffectItemType == eITEM_TYPE_magnet �̸� �ڼ������ۿ��� ������� �޴´�.
	int m_EffectItemTypes;
};