#pragma once

struct Telegram;

template <class T>
class CState
{
public:
	virtual ~CState(){}

	virtual void Enter(T* pOwner) = 0;

	virtual void Execute(T* pOwner) = 0;

	virtual void Exit(T* pOwner) = 0;

	virtual bool OnMessage(T* pOwner, const Telegram&) = 0;
};