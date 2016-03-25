#include "Bullet.h"
#include "../ItemManager.h"
#include "../../Task/PoolingManager.h"
#include "../../GameObject/ObjectManager.h"
#include "../../GameObject/Planet.h"
#include "../../GameObject/Player.h"
#include "../../AI/States/BulletStates.h"

CBullet::CBullet(
	std::string textureName,	    //bullet 이미지
	float boundingRadius,		    //bullet 충돌 범위
	float angle,				    //bullet 초기 각도 
	float speed,				    //bullet 초기 속도
	bool isFly/* = true*/)			//Fly Bullet 인지여부
	: CMover(boundingRadius)
	, m_TextureName(textureName)
	, m_fAngle(angle)
	, m_fBulletSpeed(speed)
	, m_pTexture(nullptr)
	, m_fRotationSpeed(2.0f)
	, m_EffectItemTypes(eITEM_FLAG_none)
	, m_bIsFlyItem(isFly)
	, m_pPlayer(CObjectManager::Instance()->getM_Player())
	, m_pPlanet(CObjectManager::Instance()->getM_Planet())
	, m_TargetVec(CObjectManager::Instance()->getM_Planet()->getPosition())
{
	// bullet이 초기화 될때마다 매번 생성하지 않는다.
	if (m_FSM == nullptr){
		m_FSM = new CStateMachine<CBullet>(this);
	}
	if (m_FSM != nullptr){
		m_FSM->ChangeState(CBulletNormal::Instance());
	}
}

CBullet::~CBullet(){
	if (m_FSM != nullptr)
		delete m_FSM;
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

	// 회전행렬
	m_RotationVec = Vec2((float)((beforeRotation.x * cos(radian)) - (beforeRotation.y * sin(radian))),
		(float)((beforeRotation.x * sin(radian)) + (beforeRotation.y * cos(radian))));
	m_RotationVec.normalize();

	m_RotationVec *= length;
	setPosition(m_pPlanet->getPosition() + m_RotationVec);
	setRotation(getRotation() - (dir * m_fRotationSpeed));
}

void CBullet::R_BezierWithScale(Vec2 targetPos, Vec2 controlPoint_1, Vec2 controlPoint_2, float time, float scale)
{
	setAlive(false);
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
		}, 1.0f, "BezierWithScale");
	}), nullptr);
	this->runAction(action);
	auto textureAction = FadeOut::create(2.0f);
	m_pTexture->runAction(textureAction);
}

void CBullet::R_BezierWithRotation(Vec2 targetPos, Vec2 controlPoint_1, Vec2 controlPoint_2, float time)
{
	setAlive(false);
	ccBezierConfig bezier;
	bezier.controlPoint_1 = Vec2(controlPoint_1);
	bezier.controlPoint_2 = Vec2(controlPoint_2);
	bezier.endPosition = Vec2(targetPos);

	auto bezierTo1 = BezierTo::create(time, bezier);
	auto action = Sequence::create(
		bezierTo1,
		CallFunc::create([&](){

		this->scheduleOnce([=](float dt){
			this->ReturnToMemoryBlock();
		}, 1.0f, "BezierWithRotation");
	}), nullptr);
	this->runAction(action);
	auto textureAction = RotateBy::create(0.5f, 720.f);
	m_pTexture->runAction(textureAction);
}

void CBullet::R_ScaleWithFadeOut(float scale, float scaleTime, float fadeOutTime)
{
	setAlive(false);
	auto action = Sequence::create(
		ScaleBy::create(scaleTime, scale),
		CallFunc::create([&](){

		this->scheduleOnce([=](float dt){
			this->ReturnToMemoryBlock();
		}, 1.0f, "BezierWithScale");
	}), nullptr);
	this->runAction(action);
	auto textureAction = FadeOut::create(fadeOutTime);
	m_pTexture->runAction(textureAction);
}

void CBullet::Seek(float delta)
{
	Vec2 dir = m_TargetVec - getPosition();
	dir.normalize();
	dir *= (m_fBulletSpeed * delta);
	setPosition(getPosition() + dir);
}