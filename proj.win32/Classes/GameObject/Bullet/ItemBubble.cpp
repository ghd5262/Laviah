#include "ItemBubble.h"
#include "../Planet.h"
#include "../../MyUI/SpeechBubble.h"
#include "../../Scene/GameScene.h"

CItemBubble::CItemBubble(
	sBULLET_PARAM bulletParam,
	std::string bubbleIconName,
	float angle,					//ItemBubble �ʱ� ���� 
	CBullet* owner/* = nullptr*/)	//owner missile (nullptr �� ������ �����ð����� �����Ѵ�.)
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
	float angle,					//ItemBubble �ʱ� ���� 
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
	// ������Ʈ ��ü�� ȸ��
	setRotation(getRotation() - (dir *(m_fRotationSpeed * delta)));
}

void CItemBubble::Execute(float delta)
{
	// ȭ�� �ȿ� ������������ visible false
	if (m_OwnerBullet->IsAlive() && m_ScreenRect.containsPoint(m_OwnerBullet->getPosition()))
	{
		this->setVisible(false);
	}
	else
	{
		this->setVisible(true);
		calculateIntersectPos();
	}

	// Item������ �Բ� ����
	if (!m_OwnerBullet->IsAlive())		// �̰� �̿��� OwnerBullet�� ����ϴ� ���� ������ �ȵȴ�.. ��ǻ� �� �ڵ嵵 �̹� �޸� ������ �ǵ��ư� bullet�� Alive�̴�.
		ReturnToMemoryBlock();			// OwnerBullet�� �׻� Target���� ���� �޸� ������ �ǵ��ư��� �����̴�.
}
