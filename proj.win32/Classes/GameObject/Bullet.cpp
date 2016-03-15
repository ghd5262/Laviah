#include "Bullet.h"
#include "Planet.h"
#include "../Task/PoolingManager.h"

CBullet::CBullet(
	std::string textureName,	    //bullet 이미지
	float boundingRadius,		    //bullet 충돌 범위
	float angle,				    //bullet 초기 각도 
	float speed,				    //bullet 초기 속도
	CGameObject* target)		    //bullet 타겟 위치
	: CMover(boundingRadius)
	, m_TextureName(textureName)
	, m_fAngle(angle)
	, m_fBulletSpeed(speed)
	, m_Target(target)
	, m_pTexture(nullptr)
{}

CBullet::~CBullet(){}

void* CBullet::operator new(size_t size, const std::nothrow_t)
{
	// PoolingManager에서 메모리를 할당 받는다.
	return CPoolingManager::Instance()->BulletNew();
}

void CBullet::ReturnToMemoryBlock()
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
	CPoolingManager::Instance()->Bullet_ReturnToFreeMemory(this);
}