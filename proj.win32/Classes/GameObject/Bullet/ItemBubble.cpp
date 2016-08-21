#include "ItemBubble.h"
#include "../Planet.h"
#include "../../MyUI/SpeechBubble.h"
#include "../../Scene/GameScene.h"

CItemBubble::CItemBubble(
	sBULLET_PARAM bulletParam,
	std::string bubbleIconName,
	float angle,					//ItemBubble 초기 각도 
	CBullet* owner/* = nullptr*/)	//owner missile (nullptr 일 때에는 도착시간으로 삭제한다.)
	: CBullet(
	bulletParam,
	angle,
	0.0f)
	, m_OwnerBullet(owner)
	, m_BubbleIconName(bubbleIconName)
	, m_Bubble(nullptr)
{}

CItemBubble* CItemBubble::create(
	sBULLET_PARAM bulletParam,
	std::string bubbleIconName,
	float angle,					//ItemBubble 초기 각도 
	CBullet* owner)					//owner missile
{
	CItemBubble* pRet =
		(CItemBubble*)new(std::nothrow)CItemBubble(
		bulletParam, bubbleIconName, angle, owner);

	if (pRet && pRet->init())
	{
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool CItemBubble::init()
{
	if (!initVariable())
		return false;
	return true;
}

bool CItemBubble::initVariable()
{
	try{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		m_ScreenRect = Rect(0, 0, visibleSize.width, visibleSize.height);

		m_Bubble = CSpeechBubble::create("bubble_1.png", m_BubbleIconName.c_str());
		if (m_Bubble != nullptr)
		{
			m_Bubble->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_Bubble->setPosition(Vec2::ZERO);
			addChild(m_Bubble);
		}
		setRotation(-m_fAngle + 90);
		m_PlanetPos = m_pPlanet->getOriginPos();
		calculateIntersectPos();
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void CItemBubble::calculateIntersectPos()
{
	Vec2 resultPoint = Vec2::ZERO;
	Vec2 intersectPoint = Vec2::ZERO;
	Vec2 screenLT = Vec2(m_ScreenRect.origin.x + 100, m_ScreenRect.size.height - 100);
	Vec2 screenRT = Vec2(m_ScreenRect.size.width - 100, m_ScreenRect.size.height - 100);
	Vec2 screenLB = Vec2(m_ScreenRect.origin.x + 100, m_ScreenRect.origin.y + 100);
	Vec2 screenRB = Vec2(m_ScreenRect.size.width - 100, m_ScreenRect.origin.y + 100);
	Vec2 OwnerBulletPos = m_OwnerBullet->getPosition();
	// top

	if (Vec2::isSegmentIntersect(m_PlanetPos, OwnerBulletPos, screenLT, screenRT)){
		resultPoint = Vec2::getIntersectPoint(m_PlanetPos, OwnerBulletPos, screenLT, screenRT);
	}
	if (Vec2::isSegmentIntersect(m_PlanetPos, OwnerBulletPos, screenRT, screenRB)){
		resultPoint = Vec2::getIntersectPoint(m_PlanetPos, OwnerBulletPos, screenRT, screenRB);
	}
	if (Vec2::isSegmentIntersect(m_PlanetPos, OwnerBulletPos, screenLB, screenRB)){
		resultPoint = Vec2::getIntersectPoint(m_PlanetPos, OwnerBulletPos, screenLB, screenRB);
	}
	if (Vec2::isSegmentIntersect(m_PlanetPos, OwnerBulletPos, screenLT, screenLB)){
		resultPoint = Vec2::getIntersectPoint(m_PlanetPos, OwnerBulletPos, screenLT, screenLB);
	}

	setPosition(resultPoint);
}

void CItemBubble::Rotation(float dir, float delta)
{
	// 오브젝트 자체도 회전
	setRotation(getRotation() - (dir *(m_fRotationSpeed * delta)));
}

void CItemBubble::Execute(float delta)
{
	// 화면 안에 들어왔을때에는 visible false
	if (m_OwnerBullet->IsAlive() && m_ScreenRect.containsPoint(m_OwnerBullet->getPosition()))
	{
		this->setVisible(false);
	}
	else
	{
		this->setVisible(true);
		calculateIntersectPos();
	}

	// Item삭제시 함께 삭제
	if (!m_OwnerBullet->IsAlive())		// 이것 이외의 OwnerBullet을 사용하는 곳이 있으면 안된다.. 사실상 이 코드도 이미 메모리 블럭으로 되돌아간 bullet의 Alive이다.
		ReturnToMemoryBlock();			// OwnerBullet은 항상 Target보다 먼저 메모리 블럭으로 되돌아가기 때문이다.
}
