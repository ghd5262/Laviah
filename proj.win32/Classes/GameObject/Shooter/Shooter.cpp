#include "Shooter.h"
#include "../../Task/PoolingManager.h"
#include "../ObjectManager.h"
#include "../Planet.h"

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


/* 회전행렬을 이용하여 오브젝트 회전 및 이동 */
void CShooter::Rotation(float dir, float delta)
{
    
    // 회전 속도와 방향을 이용하여 각도를 구하고 라디안으로 변환
    float radian = CC_DEGREES_TO_RADIANS(dir * (100 * delta));
    
    // 현재의 Direction Vector를 저장한다.
    Vec2 beforeRotation = getPosition() - CObjectManager::Instance()->getPlanet()->getPosition();
    
    // 거리도 저장
    float length = beforeRotation.length();
    
    /* 회전행렬을 구함
     * rotate x = ((x_ * cos(angle)) - (y_ * sin(angle)))
     * rotate y = ((x_ * sin(angle)) + (y_ * cos(angle))) */
    m_RotationVec = Vec2((float)((beforeRotation.x * cos(radian)) - (beforeRotation.y * sin(radian))),
                         (float)((beforeRotation.x * sin(radian)) + (beforeRotation.y * cos(radian))));
    
    // 노말라이즈
    m_RotationVec.normalize();
    m_RotationVec *= length;
    
    // 기존의 좌표에 새로운 좌표를 더해준다.
    setPosition(CObjectManager::Instance()->getPlanet()->getPosition() + m_RotationVec);
    
    // 오브젝트 자체도 회전
    setRotation(getRotation() - (dir *( 100 * delta)));
}