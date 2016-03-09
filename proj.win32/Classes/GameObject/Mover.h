#pragma once
#include "GameObject.h"

class CMover : public CGameObject
{
public:
	CMover(float boundingRadius);
	virtual ~CMover() = 0;

	bool IsHit(CGameObject* mover);

	/* 각자 파생클래스는 반드시 재정의 해주어야한다.
	 메모리 블럭으로 되돌리는 함수 이다. 
	 Alive = false, this를 parent로부터 제거, visible = false를 수행한다.  */
	virtual void ReturnToMemoryBlock() = 0;	

	//getter & setter
	bool IsAlive()const { return m_bAlive; }		
	void setAlive(bool alive){ m_bAlive = alive; }
	bool HasChild()const { return m_bHasChild; }

private:
	bool m_bAlive;// 메모리 풀의 Alive와 동일시 되어야 한다.
	bool m_bHasChild;// 한번이라도 create된적이 있다면 child를 가지고 있기 때문에 삭제시 removeAllChild를 해주어야 한다.
};

