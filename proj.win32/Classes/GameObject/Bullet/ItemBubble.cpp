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
	angle)
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
#if(!USE_MEMORY_POOLING)
        pRet->autorelease();
#endif
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
    this->calculateIntersectPos();
    
    return true;
}

void CItemBubble::calculateIntersectPos()
{
    float padding = 100;
    float paddingW = m_ScreenRect.size.width - padding;
    float paddingH = m_ScreenRect.size.height - padding;
	Vec2 resultPoint = Vec2::ZERO;
	Vec2 screenLT = Vec2(padding, paddingH);
	Vec2 screenRT = Vec2(paddingW, paddingH);
	Vec2 screenRB = Vec2(paddingW, padding);
    Vec2 screenLB = Vec2(padding, padding);
    
    auto getIntersect = [=, &resultPoint](Vec2 pointA, Vec2 pointB){
        if(Vec2::isSegmentIntersect(m_PlanetPos, m_OwnerBullet->getPosition(), pointA, pointB))
            resultPoint = Vec2::getIntersectPoint(m_PlanetPos, m_OwnerBullet->getPosition(), pointA, pointB);
    };

    getIntersect(screenLT, screenRT);
    getIntersect(screenRT, screenRB);
    getIntersect(screenLB, screenRB);
    getIntersect(screenLT, screenLB);
	
	setPosition(resultPoint);
}

void CItemBubble::Rotation(float dir, float delta)
{
	// 오브젝트 자체도 회전
	setRotation(getRotation() - (dir *(m_fRotationSpeed * delta)));
}

void CItemBubble::Execute(float delta)
{
    // Item삭제시 함께 삭제
    if (!m_OwnerBullet->IsAlive()){		// 이것 이외의 OwnerBullet을 사용하는 곳이 있으면 안된다.. 사실상 이 코드도 이미 메모리 블럭으로 되돌아간 bullet의 Alive이다.
        ReturnToMemoryBlock();			// OwnerBullet은 항상 Target보다 먼저 메모리 블럭으로 되돌아가기 때문이다.
        return;
    }
    // 화면 안에 들어왔을때에는 visible false
	if (m_ScreenRect.containsPoint(m_OwnerBullet->getPosition()))
	{
		this->setVisible(false);
	}
	else
	{
		this->setVisible(true);
		calculateIntersectPos();
	}

	
}
