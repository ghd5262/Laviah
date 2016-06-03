#pragma once

#include "Mover.h"
#include "../AI/StateMachine.h"

struct sALIEN_PARAM{
    sALIEN_PARAM(){}
};

class CPlanet;

class CAlien : public CMover {
public:
    static CAlien* create(sALIEN_PARAM alienParam,
                                 float walkingSpeed,
                                 float speed);
    
    virtual void Execute(float delta = 0.f) override;
    
    virtual void ReturnToMemoryBlock() override;
    
    void Walk(float delta);
    
    void Break(){};
protected:
    virtual bool init() override;
    virtual bool initVariable() override;
    
    // PoolingManager에서 메모리를 할당 받는다.
    void* operator new (size_t size, const std::nothrow_t);
    
    // 실제 메모리 해제는 memorypooling에서 담당하지만
    // 소멸자를 호출하여 Node계열이 아닌 메모리들을 삭제하기 위함
    void operator delete(void* ptr){};
    
    CAlien(sALIEN_PARAM alienParam, float walkingSpeed, float distance);
    virtual ~CAlien();
    
    //getter & setter
    CC_SYNTHESIZE(float, m_fWalkingSpeed, WalkingSpeed);
    CC_SYNTHESIZE(float, m_fDistance, Distance);    // 0 > Distance 이면 bullet의 원래 distance값을 사용함
    CC_SYNTHESIZE(float, m_fActionTime, ActionTime);
    CC_SYNTHESIZE(int, m_Direction, Direction);     // -1 : left     1 : right
    CC_SYNTHESIZE(int, m_CurrentAction, CurrentAction);
    CC_SYNTHESIZE(Vec2, m_RotationVec, RotationVec);
    CC_SYNTHESIZE(sALIEN_PARAM, m_AlienParam, AlienParam);
    CC_SYNTHESIZE(CStateMachine<CAlien>*, m_FSM, FSM);
    CC_SYNTHESIZE(CPlanet*, m_pPlanet, Planet);
};