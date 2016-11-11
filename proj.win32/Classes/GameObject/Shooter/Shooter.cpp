#include "Shooter.h"
#include "../../Task/PoolingManager.h"

CShooter::~CShooter(){}

void CShooter::ReturnToMemoryBlock()
{
	/*removeFromParent 의 이유 :
	이유는 모든 CMover의 파생 객체들은 메모리 블럭에서 메모리를 할당 받는다.
	그로인해 실행 중 addChild시 같은 메모리를 여러번 addChild할 수 있다.
	때문에 메모리 블럭으로 되돌릴때에는 부모관계를 제거하여야한다.
	또 ReferenceCount를 1 낮춰야 하는 이유도 있다.*/
	this->removeFromParent();
	this->removeAllChildren();
	this->setVisible(false);
	this->setAlive(false);
	CPoolingManager::Instance()->Shooter_ReturnToFreeMemory(this);
}

void* CShooter::operator new (size_t size, const std::nothrow_t)
{
	// PoolingManager에서 메모리를 할당 받는다.
	return CPoolingManager::Instance()->ShooterNew();
}

void CShooter::setRandomAngleIfRandom()
{
    // angle이 마이너스 이면 랜덤으로 사용한다.
    if (m_ShooterParam._isAngleRandom){
        if (random<int>(0, 100) < 80)
            m_ShooterParam._fAngle = random<float>(0.f, 180.f);
        else
            m_ShooterParam._fAngle = random<float>(180.f, 360.f);
        
    }
}