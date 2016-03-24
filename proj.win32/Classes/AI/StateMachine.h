#pragma once
#include "States/State.h"

template <class T>
class CStateMachine
{
public:
	void Execute(float delta) const;
	void ChangeState(CState<T>* pNewState);
	void setStateToPreState();

	///getter & setter
	void setCurState(CState<T>* newState){ m_pCurrentState = newState; }
	void setPreState(CState<T>* newState){ m_pPreviousState = newState; }
	void setGlobalState(CState<T>* newState){ m_pGlobalState = newState; }

	const CState<T>* getCurState() const;
	const CState<T>* getPreState() const;
	const CState<T>* getGlobalState() const;

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
void CStateMachine<T>::setStateToPreState()
{
	if (m_pCurrentState != nullptr)
		m_pCurrentState->Exit(m_pOwner);
	m_pCurrentState = m_pPreviousState;
	m_pCurrentState->Enter(m_pOwner);
}

template <class T>
void CStateMachine<T>::ChangeState(CState<T>* pNewState)
{
	if (m_pCurrentState != nullptr)
		m_pCurrentState->Exit(m_pOwner);
	m_pPreviousState = m_pCurrentState;
	m_pCurrentState = pNewState;
	m_pCurrentState->Enter(m_pOwner);
}

template <class T>
void CStateMachine<T>::Execute(float delta) const
{
	if (m_pGlobalState != nullptr)
		m_pGlobalState->Execute(m_pOwner, delta);
	if (m_pCurrentState != nullptr)
		m_pCurrentState->Execute(m_pOwner, delta);
}

template <class T>
const CState<T>* CStateMachine<T>::getCurState() const
{
	return m_pCurrentState;
}

template <class T>
const CState<T>* CStateMachine<T>::getPreState() const
{
	return m_pPreviousState;
}

template <class T>
const CState<T>* CStateMachine<T>::getGlobalState() const
{
	return m_pGlobalState;
}