#pragma once
#include "GameObject.h"

class CMover : public CGameObject
{
public:
	CMover(float boundingRadius);
	virtual ~CMover() = 0;

	bool IsHit(CGameObject* mover);
	bool IsHit(Vec2 pos, float radius);

	/* ReturnToMemoryBlock() - 각자 파생클래스는 반드시 재정의 해주어야한다.
	 메모리 블럭으로 되돌리는 함수 이다. 
	 Alive = false, this를 parent로부터 제거, visible = false를 수행한다.  */
	virtual void ReturnToMemoryBlock() = 0;	


	/* Delete() 
	- this를 부모로부터 제거한다.
	이유는 gameScene에서 한번더 제거하지 않도록 하기 위함
	
	- 각 오브젝트의 operator delete를 호출함 
	실제 메모리 해제는 memorypooling에서 담당하지만 
	소멸자를 호출하여 Node계열이 아닌 메모리들을 삭제하기 위함	*/
	void Delete();


	//getter & setter
	bool IsAlive()const { return m_bAlive; }		
	void setAlive(bool alive){ m_bAlive = alive; }
	bool HasPointer()const { return m_bHasPointer; }

private:
	bool m_bAlive;// 메모리 풀의 Alive와 동일시 되어야 한다.
	bool m_bHasPointer;// 한번이라도 create된적이 있다면 true
};

