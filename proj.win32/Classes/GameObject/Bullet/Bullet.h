#pragma once
#include "../Mover.h"
#include "../../Common/HSHUtility.h"
#include "../../AI/StateMachine.h"

enum eITEM_TYPE{
	eITEM_TYPE_none = 0,
	eITEM_TYPE_health = 1,
	eITEM_TYPE_shield = 2,
	eITEM_TYPE_magnet = 3,
	eITEM_TYPE_coin = 4,
	eITEM_TYPE_star = 5,
	eITEM_TYPE_giant = 6,
	eITEM_TYPE_bonustime = 7,

	eITEM_TYPE_MAX
};

enum eITEM_FLAG{					//변환식 : 1 << eITEM_TYPE == eITEM_FLAG
	eITEM_FLAG_none = 0x0000,		//eITEM_TYPE에서 eITEM_FLAG으로 변환하면 아래와 같다.
	eITEM_FLAG_shield = 0x0004,		//== eITEM_TYPE_shield = 2,
	eITEM_FLAG_magnet = 0x0008,		//== eITEM_TYPE_magnet = 3,
	eITEM_FLAG_coin = 0x00010,		//== eITEM_TYPE_coin = 4,
	eITEM_FLAG_star = 0x00020,		//== eITEM_TYPE_star = 5,
	eITEM_FLAG_giant = 0x0040,		//== eITEM_TYPE_giant = 6,
	eITEM_FLAG_bonustime = 0x0080,	//== eITEM_TYPE_bonustime = 7,
};

enum eCOIN_TYPE{
	eCOIN_TYPE_none = 0,
	eCOIN_TYPE_bronze = 1,
	eCOIN_TYPE_silver = 2,
	eCOIN_TYPE_gold = 3,
	eCOIN_TYPE_bigSilver = 4,
	eCOIN_TYPE_bigGold = 5,

	eCOIN_TYPE_MAX
};

enum eSTAR_TYPE{
	eSTAR_TYPE_none = 0,
	eSTAR_TYPE_bronze = 1,
	eSTAR_TYPE_silver = 2,
	eSTAR_TYPE_gold = 3,
	eSTAR_TYPE_bigSilver = 4,
	eSTAR_TYPE_bigGold = 5,

	eSTAR_TYPE_MAX
};

struct sBULLET_PARAM{
	float _fBouningRadius;
	float _fDistance;
	float _fPower;
	bool  _isFly;
	bool  _isAimingMissile;
	eCOIN_TYPE _coinType;
	eSTAR_TYPE _starType;
	eITEM_TYPE _itemType;
	char _symbol;

	sBULLET_PARAM(
		float boundingRadius,
		float distance,
		float power,
		bool isFly = 1,
		bool isAimingMissile = 0,
		eCOIN_TYPE coinType = eCOIN_TYPE_none,
		eSTAR_TYPE starType = eSTAR_TYPE_none,
		eITEM_TYPE itemType = eITEM_TYPE_none)
		: _fBouningRadius(boundingRadius)
		, _fDistance(distance)
		, _fPower(power)
		, _isFly(isFly)
		, _isAimingMissile(isAimingMissile)
		, _coinType(coinType)
		, _starType(starType)
		, _itemType(itemType){}
	sBULLET_PARAM(){}
};

class CScoreUI;
class CPlayer;
class CPlanet;
class CMultipleScore;
class CBullet : public CMover {
public:
	virtual void ReturnToMemoryBlock() override;
	virtual void Rotation(float dir, float delta);
	virtual void CollisionWithPlayer(){}
	virtual void CollisionWithPlanet(){}
	virtual void CollisionWithBarrier(){}
	virtual void ChangeToCoinOrStar(){}

	//인자로 전달된 아이템의 영향을 받는다
	void setItemEffect(int item){ m_nReceivingEffectItemTypes  |= item; }

	//인자로 전달된 아이템의 영향을 받는 bullet인지 검사한다.
	bool isEffectWithItem(eITEM_FLAG itemType){
		if (itemType == eITEM_FLAG_none)
			return false;
		return on(itemType); 
	}

	//현재 bullet이 영향을 받는 모든 플래그를 반환함
	int getItemEffect(){ return m_nReceivingEffectItemTypes; }

protected:
	virtual void update(float delta) override;

	//void* operator new (size_t size, const std::nothrow_t);
	//void operator delete(void* ptr){};

	CBullet(
		sBULLET_PARAM bulletParam,
		float angle,
		float speed);
	virtual ~CBullet();

	//getter & setter
	//중요 - 멤버변수로 포인터를 넣을때는 꼭 초기화 및 nullptr을 이용하자 (크래시 유발)
	CC_SYNTHESIZE(int, m_nReceivingEffectItemTypes, ReceivingEffectItemTypes)
	CC_SYNTHESIZE(float, m_fAngle, Angle);
	CC_SYNTHESIZE(float, m_fBulletSpeed, BulletSpeed);
	CC_SYNTHESIZE(float, m_fRotationSpeed, RotationSpeed);
	CC_SYNTHESIZE(float, m_fTime, Time);
	CC_SYNTHESIZE(Vec2, m_TargetVec, TargetVec);
	CC_SYNTHESIZE(Vec2, m_RotationVec, RotationVec);
	CC_SYNTHESIZE(CPlayer*, m_pPlayer, Player);
	CC_SYNTHESIZE(CPlanet*, m_pPlanet, Planet);
	CC_SYNTHESIZE(CStateMachine<CBullet>*, m_FSM, FSM);
	CC_SYNTHESIZE(Sprite*, m_pTexture, BulletTexture);
	CC_SYNTHESIZE(bool, m_bIsPlayerGet, IsPlayerGet);
	CC_SYNTHESIZE(sBULLET_PARAM, m_BulletParam, BulletParam);


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


	// 현재지점에서 removeTime 후 FadeOut count만큼 반복 후 삭제
	void R_FadeOutWithCount(
		int intervalCount,
		float removeTime);
    
	// 좌우로 흔들림
	void StackedRL(float duration, float stackSize, int stackCount);

	// 조종행동 - 찾기
	void Seek(float delta);

	// 점수bullet 생성 및 점수 반영
	void createScoreCurrentPos(int score);

private:
	bool on(eITEM_FLAG itemType){ return (m_nReceivingEffectItemTypes & itemType) == itemType; }

protected:
	//중요 - 멤버변수로 포인터를 넣을때는 꼭 초기화 및 nullptr을 이용하자 (크래시 유발)
	CScoreUI* m_pUIScore;

	CMultipleScore* m_pMultipleScore;
};