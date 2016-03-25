#pragma once

#include "../Mover.h"
#include "../../Common/HSHUtility.h"
#include "../../AI/StateMachine.h"

class CPlayer;
class CPlanet;
enum eITEM_FLAG;
class CBullet : public CMover {
public:
	virtual void ReturnToMemoryBlock() override;
	virtual void Rotation(int dir);

	CStateMachine<CBullet>* getFSM(){ return m_FSM; }

	virtual void CollisionWithPlayer(){}
	virtual void CollisionWithPlanet(){}
	virtual void ChangeToCoin(){}
	virtual void ChangeToStar(){}

	//인자로 전달된 아이템의 영향을 받는다
	void setItemEffect(int item){ m_EffectItemTypes |= item; }

	//인자로 전달된 아이템의 영향을 받는 bullet인지 검사한다.
	bool isEffectWithItem(eITEM_FLAG itemType){ return on(itemType); }

	//현재 bullet이 영향을 받는 모든 플래그를 반환함
	int getItemEffect(){ return m_EffectItemTypes; }

protected:
	void* operator new (size_t size, const std::nothrow_t);
	void operator delete(void* ptr){};

	CBullet(std::string textureName,
		float boundingRadius,
		float angle,
		float speed,
		bool isFly = true);
	virtual ~CBullet();

	//getter & setter
	CC_SYNTHESIZE(float, m_fAngle, Angle);
	CC_SYNTHESIZE(float, m_fBulletSpeed, BulletSpeed);
	CC_SYNTHESIZE(float, m_fRotationSpeed, RotationSpeed);
	CC_SYNTHESIZE(Vec2, m_TargetVec, TargetVec);
	CC_SYNTHESIZE(Vec2, m_RotationVec, RotationVec);
	CC_SYNTHESIZE(CPlayer*, m_pPlayer, Player);
	CC_SYNTHESIZE(CPlanet*, m_pPlanet, Planet);
	CC_SYNTHESIZE(bool, m_bIsFlyItem, IsFlyItem);		//fly coin 인지 ground coin 인지)


	/* "R_"로 시작하는 함수는 이펙트가 끝나면 ReturnToMemoryBlock 호출됨*/
	// 목표지점으로 이동 후 커지면서 FadeOut 
	void R_BezierWithScale(Vec2 targetPos, Vec2 controlPoint_1, Vec2 controlPoint_2, float time, float scale);
	
	// 목표지점으로 이동 하면서 회전 
	void R_BezierWithRotation(Vec2 targetPos, Vec2 controlPoint_1, Vec2 controlPoint_2, float time);

	// 현재지점에서 커지면서 FadeOut
	void R_ScaleWithFadeOut(float scale, float scaleTime, float fadeOutTime);

	// 조종행동 - 찾기
	void Seek(float delta);

private:
	bool on(eITEM_FLAG itemType){ return (m_EffectItemTypes & itemType) == itemType; }

protected:
	CStateMachine<CBullet>* m_FSM;
	std::string m_TextureName;
	Sprite* m_pTexture;

	// 영향을 받는 아이템 타입 
	// ex) m_EffectItemType == eITEM_TYPE_magnet 이면 자석아이템에게 영향력을 받는다.
	int m_EffectItemTypes;
};