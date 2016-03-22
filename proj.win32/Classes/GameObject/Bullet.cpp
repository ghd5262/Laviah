#include "Bullet.h"
#include "Bullet/PlayItem.h"
#include "../Task/PoolingManager.h"
#include "../AI/States/BulletStates.h"
CBullet::CBullet(
	std::string textureName,	    //bullet 이미지
	float boundingRadius,		    //bullet 충돌 범위
	float angle,				    //bullet 초기 각도 
	float speed)				    //bullet 초기 속도
	: CMover(boundingRadius)
	, m_TextureName(textureName)
	, m_fAngle(angle)
	, m_fBulletSpeed(speed)
	, m_pTexture(nullptr)
	, m_fRotationSpeed(2.0f)
	, m_fMaxForce(1.0f)
	, m_EffectItemType(eITEM_FLAG_none)
	, m_pPlayer(CObjectManager::Instance()->getM_Player())
	, m_pPlanet(CObjectManager::Instance()->getM_Planet())
{
	if (m_SteeringBehavior == nullptr){
		m_SteeringBehavior = std::shared_ptr<CSteeringBehaviors>(new CSteeringBehaviors(this), [](CSteeringBehaviors* steeringbehavior)
		{
			delete steeringbehavior;
		});
	}

	if (m_FSM == nullptr){
		m_FSM = std::shared_ptr<CStateMachine<CBullet>>(new CStateMachine<CBullet>(this), [](CStateMachine<CBullet>* fsm)
		{
			delete fsm;
		});
	}
	if (m_FSM != nullptr){
		m_FSM->ChangeState(CBulletNormal::Instance());
	}
}

CBullet::~CBullet(){

}

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

void CBullet::Rotation(int dir)
{
	float radian = CC_DEGREES_TO_RADIANS(dir * m_fRotationSpeed);
	Vec2 beforeRotation = getPosition() - m_pPlanet->getPosition();
	float length = beforeRotation.length();

	m_RotationVec = Vec2((float)((beforeRotation.x * cos(radian)) - (beforeRotation.y * sin(radian))),
		(float)((beforeRotation.x * sin(radian)) + (beforeRotation.y * cos(radian))));
	m_RotationVec.normalize();

	m_RotationVec *= length;
	setPosition(m_pPlanet->getPosition() + m_RotationVec);
	setRotation(getRotation() - (dir * m_fRotationSpeed));
}

void CBullet::BezierWithScale(Vec2 targetPos, Vec2 controlPoint_1, Vec2 controlPoint_2, float time, float scale)
{
	ccBezierConfig bezier;
	bezier.controlPoint_1 = Vec2(controlPoint_1);
	bezier.controlPoint_2 = Vec2(controlPoint_2);
	bezier.endPosition = Vec2(targetPos);

	auto bezierTo1 = BezierTo::create(time, bezier);
	this->setZOrder(101);
	auto action = Sequence::create(
		bezierTo1,
		ScaleBy::create(0.5f, scale),
		CallFunc::create([&](){

		this->scheduleOnce([=](float dt){
			this->ReturnToMemoryBlock();
		}, 1.0f, "Sequence");
	}), nullptr);
	this->runAction(action);
	auto textureAction = FadeOut::create(2.0f);
	m_pTexture->runAction(textureAction);
}