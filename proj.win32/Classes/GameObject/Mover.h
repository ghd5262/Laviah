#pragma once
#include "GameObject.h"

class CMover : public CGameObject
{
public:
	CMover(float boundingRadius);
	virtual ~CMover() = 0;

	bool IsHit(CGameObject* mover);

	/* ���� �Ļ�Ŭ������ �ݵ�� ������ ���־���Ѵ�.
	 �޸� ������ �ǵ����� �Լ� �̴�. 
	 Alive = false, this�� parent�κ��� ����, visible = false�� �����Ѵ�.  */
	virtual void ReturnToMemoryBlock() = 0;	

	//getter & setter
	bool IsAlive()const { return m_bAlive; }		
	void setAlive(bool alive){ m_bAlive = alive; }
	bool HasChild()const { return m_bHasChild; }

private:
	bool m_bAlive;// �޸� Ǯ�� Alive�� ���Ͻ� �Ǿ�� �Ѵ�.
	bool m_bHasChild;// �ѹ��̶� create������ �ִٸ� child�� ������ �ֱ� ������ ������ removeAllChild�� ���־�� �Ѵ�.
};

