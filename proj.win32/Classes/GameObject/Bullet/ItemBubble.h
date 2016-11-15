#pragma once
#include "Bullet.h"

class CGameObject;
class CSpeechBubble;

class CItemBubble : public CBullet {
public:
	/*create를 호출하면 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
	static CItemBubble* create(
		sBULLET_PARAM bulletParam,
		std::string bubbleIconName,
		float angle,				//TargetMark 초기 각도 
		CBullet* owner = nullptr);	//owner missile (nullptr 일 때에는 도착시간으로 삭제한다.)
	virtual void Execute(float delta = 0.f) override;
	virtual void Rotation(float dir, float delta) override;

protected:
	virtual bool init() override;

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
	cocos2d::Rect m_ScreenRect;
	cocos2d::Vec2 m_IntersectionPos;
	cocos2d::Vec2 m_PlanetPos;
	
};