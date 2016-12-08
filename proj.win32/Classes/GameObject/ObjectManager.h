#pragma once
#include <vector>
#include "../Common/HSHUtility.h"
#include "../AI/StateMachine.h"

/*------------------------------ObjectManager 클래스설명----------------------------------
*
* CMover를 상속받는 모든 클래스를 Execute및 Remove하는 함수이다.
* 현재 주의해야 할 사항은 Remove함수가 Delete함수를 호출하는 구조이다.
* 이유는 Delete에서 removeFromParent()와 operator delete를 호출하여 소멸자를 호출하기 위함
*
*----------------------------------------------------------------------------------------*/

class CBullet;
class CPlanet;
class CPlayer;
class CBackGround;
class CBulletCreator;

class CObjectManager
{
    // 회전 가속도 
    const float ROTATE_ACCEL_MAX = 0.5f;
public:
	static CObjectManager* Instance();

	/* AddBullet(), AddShooter() => Bullet과 Shooter를 따로 관리하는 이유는
	첫째로 Bullet이 생성되는 루틴이 Shooter에 있다.
	때문에 하나의 리스트에서 둘 다 관리하면 리스트 순회 중 Bullet이 리스트에 추가될 수 있다.
	즉, 리스트를 사용하고 있는 중에 원하지 않는 변형을 가지고 올 수 있다.
	
	또한 Bullet에는 존재하지만 Shooter에는 존재하지 않는 함수들이 있다.*/
    
#if(USE_MEMORY_POOLING)
    void AddBullet(void* bullet);
#else
    void AddBullet(CBullet* bullet);
#endif
    
    // 게임 종료 시점에 호출된다. RemoveAllBullet(), RemoveAllShooter() 호출함
	void RemoveAllObject();
    void removeBulletFromList(CBullet* bullet);

	//callback
	void RotationObject(float dir);
    void RotateAccelerationUpdate(float value);
	
    void Execute(float delta);
    
	// 초기화
	void Clear();

	//getter & setter
	CC_SYNTHESIZE(CPlanet*, m_Planet, Planet);
	CC_SYNTHESIZE(CPlayer*, m_Player, Player);
    CC_SYNTHESIZE(CBulletCreator*, m_BulletCreator, BulletCreator);
	CC_SYNTHESIZE(CBackGround*, m_Background, Background);
	CC_SYNTHESIZE(float, m_fStageTime, StageTime);
	CC_SYNTHESIZE(float, m_fDelta, Delta);
	CC_SYNTHESIZE(bool, m_IsGamePause, IsGamePause);

private:
	void createBulletByTimer(float delta);
	void removeAllBullet();				// Delete함수 호출! 이유는 구현부에~
	CObjectManager();
	~CObjectManager(){};

private:
	std::vector<CBullet*> m_BulletList;
    float m_fRotateAcceleration;
};