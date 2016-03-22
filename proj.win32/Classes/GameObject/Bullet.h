#pragma once

#include "Mover.h"
#include "SteeringBehaviors.h"
#include "ObjectManager.h"
#include "Planet.h"
#include "Player.h"
#include "../Common/HSHUtility.h"
#include "../AI/StateMachine.h"

enum eITEM_TYPE;
class CBullet : public CMover {
public:
	virtual void ReturnToMemoryBlock() override;
	virtual void Rotation(int dir);

	CSteeringBehaviors* getSteeringBehavior(){ return m_SteeringBehavior.get(); }
	CStateMachine<CBullet>* getFSM(){ return m_FSM.get(); }

	virtual void CollisionWithPlayer(){}
	virtual void CollisionWithPlanet(){}

	//인자로 전달된 아이템의 영향을 받는다
	void setItemEffect(int item){ m_EffectItemTypes |= item; }

	//인자로 전달된 아이템의 영향을 받는 bullet인지 검사한다.
	bool isEffectWithItem(eITEM_FLAG itemType){ return on(itemType); }

	int getItemEffect(){ return m_EffectItemTypes; }

protected:
	void* operator new (size_t size, const std::nothrow_t);
	void operator delete(void* ptr){};

	CBullet(std::string textureName,
		float boundingRadius,
		float angle,
		float speed);
	virtual ~CBullet();

	//getter & setter
	CC_SYNTHESIZE(float, m_fAngle, Angle);
	CC_SYNTHESIZE(float, m_fBulletSpeed, BulletSpeed);
	CC_SYNTHESIZE(float, m_fMaxForce, MaxForce);
	CC_SYNTHESIZE(float, m_fRotationSpeed, RotationSpeed);
	CC_SYNTHESIZE(Vec2, m_VelocityVec, VelocityVec);
	CC_SYNTHESIZE(Vec2, m_RotationVec, RotationVec);
	CC_SYNTHESIZE(CPlayer*, m_pPlayer, Player);
	CC_SYNTHESIZE(CPlanet*, m_pPlanet, Planet);

	// 목표지점으로 이동 후 커지면서 FadeOut 
	// 이펙트가 끝나면 오브젝트가 삭제됨
	void BezierWithScale(Vec2 targetPos, Vec2 controlPoint_1, Vec2 controlPoint_2, float time, float scale);

private:
	bool on(eITEM_FLAG itemType){ return (m_EffectItemTypes & itemType) == itemType; }

protected:
	std::shared_ptr<CSteeringBehaviors> m_SteeringBehavior;
	std::shared_ptr<CStateMachine<CBullet>> m_FSM;
	std::string m_TextureName;
	Sprite* m_pTexture;

	// 영향을 받는 아이템 타입 
	// ex) m_EffectItemType == eITEM_TYPE_magnet 이면 자석아이템에게 영향력을 받는다.
	int m_EffectItemTypes;
};