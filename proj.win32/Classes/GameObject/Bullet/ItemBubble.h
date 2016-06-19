#pragma once
#include "Bullet.h"

class CGameObject;
class CSpeechBubble;

class CItemBubble : public CBullet {
public:
	/*create�� ȣ���ϸ� CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�.*/
	static CItemBubble* create(
		sBULLET_PARAM bulletParam,
		std::string bubbleIconName,
		float angle,				//TargetMark �ʱ� ���� 
		CBullet* owner = nullptr);	//owner missile (nullptr �� ������ �����ð����� �����Ѵ�.)
	virtual void Execute(float delta = 0.f) override;
	virtual void Rotation(float dir, float delta) override;

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	void calculateIntersectPos();

	CItemBubble(
		sBULLET_PARAM bulletParam,
		std::string bubbleIconName,
		float angle,
		CBullet* owner);

	virtual ~CItemBubble(){};

private:
	CBullet* m_OwnerBullet;			// owner missile
	CSpeechBubble* m_Bubble;
	std::string m_BubbleIconName;
	Rect m_ScreenRect;
	Vec2 m_IntersectionPos;
	Vec2 m_PlanetPos;
	
};