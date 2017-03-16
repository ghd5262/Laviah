#pragma once
#include "States/State.h"

/*------------------------CStateMachine------------------------
 * Template 클래스
 * FSM사용을 원하는 클래스에서 정의하여 사용할 수 있다.
 * 사용자는 States를 가지고 있어야 한다.
 * Global, Current, Previous 상태를 가지고 있다.
 * Global은 어떠한 상황에서도 가장 먼저 반영된다.
 * Previous는 직전의 상태를 보관한다.
--------------------------------------------------------------*/

template <class T>
class CStateMachine
{
public:
	void Execute(float delta) const;
	void ChangeState(CState<T>* pNewState);
	void setStateToPreState();

	// getter & setter
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
	{
		this->m_pOwner = Owner;
	}
	~CStateMachine(void){};

private:
	T*					m_pOwner;
	CState<T>*			m_pPreviousState;
	CState<T>*			m_pCurrentState;
	CState<T>*			m_pGlobalState;
};

/* 현재 상태를 이전상태로 되돌린다. */
template <class T>
void CStateMachine<T>::setStateToPreState()
{
	if (m_pCurrentState != nullptr)
		m_pCurrentState->Exit(m_pOwner);
	m_pCurrentState = m_pPreviousState;
	m_pCurrentState->Enter(m_pOwner);
}

/* 현재 상태를 전달받은 상태로 변경한다. */
template <class T>
void CStateMachine<T>::ChangeState(CState<T>* pNewState)
{
	if (m_pCurrentState != nullptr)
		m_pCurrentState->Exit(m_pOwner);
	m_pPreviousState = m_pCurrentState;
	m_pCurrentState = pNewState;
	m_pCurrentState->Enter(m_pOwner);
}

/* 현재 상태와 Global상태를 Execute */
template <class T>
void CStateMachine<T>::Execute(float delta) const
{
	if (m_pGlobalState != nullptr)
		m_pGlobalState->Execute(m_pOwner, delta);
	if (m_pCurrentState != nullptr)
		m_pCurrentState->Execute(m_pOwner, delta);
}

/* 현재 상태를 반환한다. */
template <class T>
const CState<T>* CStateMachine<T>::getCurState() const
{
	return m_pCurrentState;
}

/* 이전 상태를 반환한다. */
template <class T>
const CState<T>* CStateMachine<T>::getPreState() const
{
	return m_pPreviousState;
}

/* 전역 상태를 반환한다. */
template <class T>
const CState<T>* CStateMachine<T>::getGlobalState() const
{
	return m_pGlobalState;
}