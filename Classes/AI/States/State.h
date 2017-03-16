#pragma once
/*---------------------------CState----------------------------
 * Template 클래스
 * FSM을 소유하는 클래스의 상태를 정의하는 추상클래스
 * State의 인터페이스만을 제공한다.
 * Enter()은 상태가 Execute를 실행하기 전에 한번만 호출된다.
 * Exit()은 상태가 종료 혹은 변경될 때 한번만 호출된다.
 * Execute()은 상태의 메인 루프이다.
 * 각 함수는 모두 FSM의 주체 인스턴스를 인자로 받는다.
--------------------------------------------------------------*/

template <class T>
class CState
{
public:
	virtual ~CState(){}

	virtual void Enter(T* pOwner) = 0;

	virtual void Execute(T* pOwner, float delta) = 0;

	virtual void Exit(T* pOwner) = 0;
};