#include "Bullet.h"
#include "../ItemManager.h"
#include "../../Task/PoolingManager.h"
#include "../../GameObject/ObjectManager.h"
#include "../../GameObject/Planet.h"
#include "../../GameObject/Player.h"
#include "../../AI/States/BulletStates.h"
#include "../../MyUI/ScoreUI.h"
#include "../../MyUI/UIManager.h"
#include "../../Scene/GameScene.h"
#include "../../MyUI/MultipleScore.h"

CBullet::CBullet(
	sBULLET_PARAM bulletParam,
	float angle,				    //bullet 초기 각도 
	float speed)				    //bullet 초기 속도
	: CMover(bulletParam._fBouningRadius)
	, m_BulletParam(bulletParam)
	, m_fAngle(angle)
	, m_fBulletSpeed(speed)
	, m_pTexture(nullptr)
	, m_fRotationSpeed(100.f)
	, m_nReceivingEffectItemTypes(eITEM_FLAG_none)
	, m_pPlayer(CObjectManager::Instance()->getPlayer())
	, m_pPlanet(CObjectManager::Instance()->getPlanet())
	, m_TargetVec(CObjectManager::Instance()->getPlanet()->getPosition())
	, m_bIsPlayerGet(false)
	, m_fTime(0.f)
	, m_pUIScore(nullptr)
	, m_pMultipleScore(nullptr)
{
	// bullet이 초기화 될때마다 매번 생성하지 않는다.
	if (m_FSM == nullptr){
		m_FSM = new CStateMachine<CBullet>(this);
	}
	if (m_FSM != nullptr){
		m_FSM->ChangeState(CBulletNormal::Instance());
	}
    setCascadeOpacityEnabled(true);
}

CBullet::~CBullet(){
	if (m_FSM != nullptr)
		delete m_FSM;
}


/* poolingManager에서 FreeMemory Block을 하나 가져옴 */
void* CBullet::operator new(size_t size, const std::nothrow_t)
{
    // PoolingManager에서 메모리를 할당 받는다.
    return CPoolingManager::Instance()->BulletNew();
}


/* 오브젝트의 메모리를 FreeMemoryBlock으로 변환 == 오브젝트 삭제 */
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


/* 회전행렬을 이용하여 오브젝트 회전 및 이동 */
void CBullet::Rotation(float dir, float delta)
{
    
    // 회전 속도와 방향을 이용하여 각도를 구하고 라디안으로 변환
    float radian = CC_DEGREES_TO_RADIANS(dir * (m_fRotationSpeed * delta));
    
    // 현재의 Direction Vector를 저장한다.
    Vec2 beforeRotation = getPosition() - m_pPlanet->getPosition();
    
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
    setPosition(m_pPlanet->getPosition() + m_RotationVec);
    
    // 오브젝트 자체도 회전
    setRotation(getRotation() - (dir *( m_fRotationSpeed * delta)));
}


/* 일정 위치로 베지어 곡선을 그리며 이동한 후 커지면서 FadeOut */
void CBullet::R_BezierWithScale(Vec2 targetPos, Vec2 controlPoint_1, Vec2 controlPoint_2, float time, float scale)
{
    // 더이상 Execute 하지 않는다.
    setAlive(false);
    
    // 베지어 곡선 생성
    ccBezierConfig bezier;
    bezier.controlPoint_1 = Vec2(controlPoint_1);
    bezier.controlPoint_2 = Vec2(controlPoint_2);
    bezier.endPosition = Vec2(targetPos);
    
    // 베지어 액션 및 다른 액션 순서대로 실행
    auto bezierTo1 = BezierTo::create(time, bezier);
    this->setZOrder(101);
    auto action = Sequence::create(bezierTo1,
        Spawn::create(ScaleBy::create(0.5f, scale), FadeTo::create(2.0f, 1), NULL),
        // 두 액션이 끝난후 스케줄을 걸어 오브젝트 삭제
        CallFunc::create([&](){
        this->scheduleOnce([=](float dt){
            this->ReturnToMemoryBlock();
        }, 1.0f, "BezierWithScale");
        
    }), nullptr);
    this->runAction(action);
    
    // texture 페이드 아웃
//    auto textureAction = FadeOut::create(2.0f);
//    m_pTexture->runAction(textureAction);
}


void CBullet::R_BezierWithRotation(Vec2 targetPos, Vec2 controlPoint_1, Vec2 controlPoint_2, float time)
{
	// 더이상 Execute 하지 않는다.
	setAlive(false);

	// 이전의 Action들을 cancel
	this->stopAllActions();
	this->m_pTexture->stopAllActions();

	ccBezierConfig bezier;
	bezier.controlPoint_1 = Vec2(controlPoint_1);
	bezier.controlPoint_2 = Vec2(controlPoint_2);
	bezier.endPosition = Vec2(targetPos);

	auto bezierTo1 = BezierTo::create(time, bezier);
	auto action = Sequence::create(
        Spawn::create(RotateBy::create(0.5f, 720.f), bezierTo1, NULL),
		CallFunc::create([&](){

		this->scheduleOnce([=](float dt){
			this->ReturnToMemoryBlock();
		}, 1.0f, "BezierWithRotation");
	}), nullptr);
	this->runAction(action);
//	auto textureAction = RotateBy::create(0.5f, 720.f);
//	m_pTexture->runAction(textureAction);
}


void CBullet::R_ScaleWithFadeOut(float scale, float scaleTime, float fadeOutTime)
{
	// 더이상 Execute 하지 않는다.
	setAlive(false);

	// 이전의 Action들을 cancel
	this->stopAllActions();
//	this->m_pTexture->stopAllActions(); 

	auto action = Sequence::create(
        Spawn::create(FadeTo::create(fadeOutTime, 1), ScaleBy::create(scaleTime, scale), NULL),
		CallFunc::create([&](){

		this->scheduleOnce([=](float dt){
			this->ReturnToMemoryBlock();
		}, 1.0f, MakeString("ScaleWithFadeOut_%d", random<int>(1, 100)));
	}), nullptr);
	this->runAction(action);
//	auto textureAction = FadeOut::create(fadeOutTime);
//	this->runAction(textureAction);
}


void CBullet::R_FadeOutWithCount(int intervalCount, float removeTime)
{
	this->scheduleOnce([=](float dt){
		this->ReturnToMemoryBlock();
	}, removeTime + 0.1f, MakeString("FadeOutWithCount_%d", random<int>(1, 100)));

	auto textureAction1 = FadeTo::create((removeTime / intervalCount) - 0.1f, 1);
	auto textureAction2 = FadeTo::create(0.1f, 255);
	auto repeatAction = Repeat::create(Sequence::create(textureAction2, textureAction1, nullptr), intervalCount);
	this->runAction(repeatAction);
}


void CBullet::StackedRL(float duration, float stackSize, int stackCount)
{
	this->runAction(
		Repeat::create(
		Sequence::create(
		MoveBy::create(duration / stackCount, Vec2(stackSize, 0)),
		MoveBy::create(duration / stackCount, Vec2(-stackSize, 0)),
		nullptr), stackCount));

}


/* 조종행동 Seek - 오브젝트를 목표지점까지 이동 */
void CBullet::Seek(float delta)
{
    // 방향벡터 구하기
    Vec2 dir = m_TargetVec - getPosition();
    dir.normalize();
    
    // 속력벡터 계산
    dir *= (m_fBulletSpeed * delta);
    
    // 현재 좌표에 적용
    setPosition(getPosition() + dir);
}


void CBullet::createScoreCurrentPos(int score)
{
	if (m_pMultipleScore != nullptr){
		m_pMultipleScore->AddScore(score);
		auto scoreBullet = CScoreBullet::create(score);
		scoreBullet->setPosition(getPosition());
		scoreBullet->setAnchorPoint(Vec2::ZERO);
		CGameScene::getGridWorld()->addChild(scoreBullet);
	}
}
