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

	// PoolingManager���� �޸𸮸� �Ҵ� �޴´�.
	void* operator new (size_t size, const std::nothrow_t);

	// ���� �޸� ������ memorypooling���� ���������
	// �Ҹ��ڸ� ȣ���Ͽ� Node�迭�� �ƴ� �޸𸮵��� �����ϱ� ����
	void operator delete(void* ptr){};

	CAlien(sALIEN_PARAM alienParam, float walkingSpeed, float distance);
	virtual ~CAlien();

	//getter & setter
	CC_SYNTHESIZE(float, m_fWalkingSpeed, WalkingSpeed);
	CC_SYNTHESIZE(float, m_fDistance, Distance);    // 0 > Distance �̸� bullet�� ���� distance���� �����
	CC_SYNTHESIZE(float, m_fActionTime, ActionTime);
	CC_SYNTHESIZE(int, m_Direction, Direction);     // -1 : left     1 : right
	CC_SYNTHESIZE(int, m_CurrentAction, CurrentAction);
	CC_SYNTHESIZE(cocos2d::Vec2, m_RotationVec, RotationVec);
	CC_SYNTHESIZE(sALIEN_PARAM, m_AlienParam, AlienParam);
	CC_SYNTHESIZE(CStateMachine<CAlien>*, m_FSM, FSM);
	CC_SYNTHESIZE(CPlanet*, m_Planet, Planet);

private:
	cocos2d::Sprite* m_Texture;
};