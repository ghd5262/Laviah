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
    std::string _spriteName;
	float _boundingRadius;
	float _power;
    float _speed;
    float _angle;
    float _distance;
	float _delayTime;
    char  _symbol;
    bool  _isFly;

	sBULLET_PARAM()
    : _spriteName("")
    , _boundingRadius(0)
    , _power(0)
    , _speed(0)
    , _angle(0)
    , _distance(0)
	, _delayTime(0)
    , _symbol(-1)
    , _isFly(true){}
    
    sBULLET_PARAM(const sBULLET_PARAM& data)
    : _spriteName(data._spriteName)
    , _boundingRadius(data._boundingRadius)
    , _power(data._power)
    , _speed(data._speed)
    , _angle(data._angle)
    , _distance(data._distance)
	, _delayTime(data._delayTime)
    , _symbol(data._symbol)
    , _isFly(data._isFly){}
    
    sBULLET_PARAM(const sBULLET_PARAM* data)
    : _spriteName(data->_spriteName)
    , _boundingRadius(data->_boundingRadius)
    , _power(data->_power)
    , _speed(data->_speed)
    , _angle(data->_angle)
    , _distance(data->_distance)
	, _delayTime(data->_delayTime)
    , _symbol(data->_symbol)
    , _isFly(data->_isFly){}
};

class CScoreUI;
class CPlayer;
class CPlanet;
class CBullet : public CMover {
public:
	virtual void Rotation(float speed);
	virtual void CollisionWithPlayer(){}
	virtual void CollisionWithPlanet(){}
	virtual void CollisionWithBarrier(){}
	virtual void ChangeToCoinOrStar(){}
    virtual void ReturnToMemoryBlock() override;
    virtual void setBoundingRadius(float data) override;
    virtual float getBoundingRadius() const override;

    virtual void Execute(float delta) override;

    void ChangeState(CState<CBullet>* newState)
    { m_FSM->ChangeState(newState); };
    CBullet* setBulletInfo(sBULLET_PARAM data);
    virtual CBullet* build();
    
    void  setSpeed          (float data);
    void  setAngle          (float data);
    void  setDistance       (float data);
	void  setDelayTime		(float data);
    void  setPower          (float data);
    void  setSymbol         (float data);
    void  setIsFly          (float data);
    
    sBULLET_PARAM getInfo()   const;
    float getSpeed()          const;
    float getAngle()          const;
    float getDistance()       const;
	float getDelayTime()      const;
    float getPower()          const;
    char  getSymbol()         const;
    bool  getIsFly()          const;
    
    int getItemEffect(){ return m_ItemFlag; }
    
    void setItemEffect(int item){ m_ItemFlag |= item; }
    
    bool IsEffectWithItem(eITEM_FLAG itemType){
        if (itemType == eITEM_FLAG_none)
            return false;
        return on(itemType);
    }
    
    bool IsTimeUP(){
        return m_Time >= m_BulletInfo._delayTime;
    }
    
    static cocos2d::Vec2 getCirclePosition(float angle, float distance, cocos2d::Vec2 center);
    static cocos2d::Vec2 getSquarePosition(float angle, float distance);

protected:
    virtual bool init() override;
    
#if(USE_MEMORY_POOLING)
	void* operator new (size_t size, const std::nothrow_t);
	void operator delete(void* ptr){};
#endif

    CC_SYNTHESIZE(CPlayer*, m_Player, Player);
    CC_SYNTHESIZE(CPlanet*, m_Planet, Planet);
    CC_SYNTHESIZE(cocos2d::Vec2, m_TargetVec, TargetVec);
    CC_SYNTHESIZE(cocos2d::Vec2, m_RotationVec, RotationVec);
    CC_SYNTHESIZE(float, m_RotationSpeed, RotationSpeed);
	CC_SYNTHESIZE(float, m_Time, Time);
    CC_SYNTHESIZE(int, m_ItemFlag, ItemFlag);
	CC_SYNTHESIZE(bool, m_bIsPlayerGet, IsPlayerGet);


    void R_UpAndBezier(cocos2d::Vec2 targetPos,
                       cocos2d::Vec2 controlPoint_1,
                       cocos2d::Vec2 controlPoint_2,
                       float time,
                       float scale);
    
    
    void R_BezierWithScale(cocos2d::Vec2 targetPos,
                           cocos2d::Vec2 controlPoint_1,
                           cocos2d::Vec2 controlPoint_2,
                           float time,
                           float scale);
    
    
    void R_BezierWithRotation(cocos2d::Vec2 targetPos,
                              cocos2d::Vec2 controlPoint_1,
                              cocos2d::Vec2 controlPoint_2,
                              float time);
    
    
    void R_ScaleWithFadeOut(float scale,
                            float scaleTime, 
                            float fadeOutTime);
    
    
    void R_FadeOutWithCount(int repeat,
                            float removeTime);
    
    void StackedRL(float duration, float stackSize, int stackCount);

	void Seek(float delta);
    
    void Flee(float delta);

	void createScoreCurrentPos(int score);
    
    CBullet();
    virtual ~CBullet();

private:
	bool on(eITEM_FLAG itemType){ return (m_ItemFlag & itemType) == itemType; }
    
protected:
    CStateMachine<CBullet>* m_FSM;
    
private:
    sBULLET_PARAM m_BulletInfo;
};