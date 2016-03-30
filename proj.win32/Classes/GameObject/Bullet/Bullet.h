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
	virtual void CollisionWithPlayer(){}
	virtual void CollisionWithPlanet(){}
	virtual void ChangeToCoin(){}
	virtual void ChangeToStar(){}

	//인자로 전달된 아이템의 영향을 받는다
	void setItemEffect(int item){ m_nReceivingEffectItemTypes  |= item; }

	//인자로 전달된 아이템의 영향을 받는 bullet인지 검사한다.
	bool isEffectWithItem(eITEM_FLAG itemType){ return on(itemType); }

	//현재 bullet이 영향을 받는 모든 플래그를 반환함
	int getItemEffect(){ return m_nReceivingEffectItemTypes; }

protected:
	void* operator new (size_t size, const std::nothrow_t);
	void operator delete(void* ptr){};

	CBullet(
		std::string textureName,
		float boundingRadius,
		float angle,
		float speed, 
		bool isFly = true);
	virtual ~CBullet();

	//getter & setter
	CC_SYNTHESIZE(int, m_nReceivingEffectItemTypes, ReceivingEffectItemTypes)
	CC_SYNTHESIZE(float, m_fAngle, Angle);
	CC_SYNTHESIZE(float, m_fBulletSpeed, BulletSpeed);
	CC_SYNTHESIZE(float, m_fRotationSpeed, RotationSpeed);
	CC_SYNTHESIZE(Vec2, m_TargetVec, TargetVec);
	CC_SYNTHESIZE(Vec2, m_RotationVec, RotationVec);
	CC_SYNTHESIZE(CPlayer*, m_pPlayer, Player);
	CC_SYNTHESIZE(CPlanet*, m_pPlanet, Planet);
	CC_SYNTHESIZE(CStateMachine<CBullet>*, m_FSM, FSM);
	CC_SYNTHESIZE(Sprite*, m_pTexture, BulletTexture);
	CC_SYNTHESIZE(std::string, m_TextureName, TextureName);
	CC_SYNTHESIZE(bool, m_bIsFlyItem, IsFlyItem);
	CC_SYNTHESIZE(bool, m_bIsPlayerGet, IsPlayerGet);

	/* "R_"로 시작하는 함수는 이펙트가 끝나면 ReturnToMemoryBlock 호출됨*/

	// 목표지점으로 이동 후 커지면서 FadeOut 
	void R_BezierWithScale(
		Vec2 targetPos, 
		Vec2 controlPoint_1, 
		Vec2 controlPoint_2, 
		float time,
		float scale);
	

	// 목표지점으로 이동 하면서 회전 
	void R_BezierWithRotation(
		Vec2 targetPos, 
		Vec2 controlPoint_1, 
		Vec2 controlPoint_2,
		float time);


	// 현재지점에서 커지면서 FadeOut
	void R_ScaleWithFadeOut(
		float scale, 
		float scaleTime, 
		float fadeOutTime);


	// 조종행동 - 찾기
	void Seek(float delta);

private:
	bool on(eITEM_FLAG itemType){ return (m_nReceivingEffectItemTypes & itemType) == itemType; }
};