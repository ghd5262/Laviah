#pragma once
/*---------------------------CState----------------------------
 * Template Ŭ����
 * FSM�� �����ϴ� Ŭ������ ���¸� �����ϴ� �߻�Ŭ����
 * State�� �������̽����� �����Ѵ�.
 * Enter()�� ���°� Execute�� �����ϱ� ���� �ѹ��� ȣ��ȴ�.
 * Exit()�� ���°� ���� Ȥ�� ����� �� �ѹ��� ȣ��ȴ�.
 * Execute()�� ������ ���� �����̴�.
 * �� �Լ��� ��� FSM�� ��ü �ν��Ͻ��� ���ڷ� �޴´�.
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