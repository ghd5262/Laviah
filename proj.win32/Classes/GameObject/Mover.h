#pragma once
#include "GameObject.h"

class CMover : public CGameObject
{
public:
	CMover(float boundingRadius);
	virtual ~CMover() = 0;

	bool IsHit(CGameObject* mover);
	bool IsHit(Vec2 pos, float radius);

	/* ReturnToMemoryBlock() - ���� �Ļ�Ŭ������ �ݵ�� ������ ���־���Ѵ�.
	 �޸� ������ �ǵ����� �Լ� �̴�. 
	 Alive = false, this�� parent�κ��� ����, visible = false�� �����Ѵ�.  */
	virtual void ReturnToMemoryBlock() = 0;	


	/* Delete() 
	- this�� �θ�κ��� �����Ѵ�.
	������ gameScene���� �ѹ��� �������� �ʵ��� �ϱ� ����
	
	- �� ������Ʈ�� operator delete�� ȣ���� 
	���� �޸� ������ memorypooling���� ��������� 
	�Ҹ��ڸ� ȣ���Ͽ� Node�迭�� �ƴ� �޸𸮵��� �����ϱ� ����	*/
	void Delete();


	//getter & setter
	bool IsAlive()const { return m_bAlive; }		
	void setAlive(bool alive){ m_bAlive = alive; }
	bool HasPointer()const { return m_bHasPointer; }

private:
	bool m_bAlive;// �޸� Ǯ�� Alive�� ���Ͻ� �Ǿ�� �Ѵ�.
	bool m_bHasPointer;// �ѹ��̶� create������ �ִٸ� true
};

