#pragma once

template <class T>
class CState
{
public:
	virtual ~CState(){}

	virtual void Enter(T* pOwner) = 0;

	virtual void Execute(T* pOwner, float delta) = 0;

	virtual void Exit(T* pOwner) = 0;
};