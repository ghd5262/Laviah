#pragma once
#include "States/State.h"

template <class T>
class CStateMachine
{
public:
	void Execute() const;
	void ChangeState(CState<T>* pNewState);
	
	///getter & setter
	const CState<T>* getM_curState() const;
	const CState<T>* getM_preState() const;
	const CState<T>* getM_globalState() const;

	CStateMachine(T* Owner) : m_pOwner(nullptr),
		m_pCurrentState(nullptr),
		m_pPreviousState(nullptr),
		m_pGlobalState(nullptr)
	{this->m_pOwner = Owner;}
	~CStateMachine(void){};

private:
	T*					m_pOwner;
	CState<T>*			m_pPreviousState;
	CState<T>*			m_pCurrentState;
	CState<T>*			m_pGlobalState;
};

template <class T>
void CStateMachine<T>::ChangeState(CState<T>* pNewState)
{
	if (m_pCurrentState != nullptr)
		m_pCurrentState->Exit(m_pOwner);
	m_pCurrentState = pNewState;
	m_pCurrentState->Enter(m_pOwner);
}

template <class T>
void CStateMachine<T>::Execute() const
{
	if (m_pGlobalState != nullptr)
		m_pGlobalState->Execute(m_pOwner);
	if (m_pCurrentState != nullptr)
		m_pCurrentState->Execute(m_pOwner);
}

template <class T>
const CState<T>* CStateMachine<T>::getM_curState() const
{
	return m_pCurrentState;
}

template <class T>
const CState<T>* CStateMachine<T>::getM_preState() const
{
	return m_pPreviousState;
}

template <class T>
const CState<T>* CStateMachine<T>::getM_globalState() const
{
	return m_pGlobalState;
}