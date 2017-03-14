#pragma once
#include "Bullet.h"

enum eLETTER{
    eLETTER_B = 0,
    eLETTER_O = 1,
    eLETTER_N = 2,
    eLETTER_U = 3,
    eLETTER_S = 4,
    eLETTER_T = 5,
    eLETTER_I = 6,
    eLETTER_M = 7,
    eLETTER_E = 8,
    
    eLETTER_MAX
};

class CBonusTimeUI;
class CPlayer;

class CBonusLetter : public CBullet {
public:
	/*create를 호출하면 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
	static CBonusLetter* create();

	virtual void CollisionWithPlayer() override;
	virtual void CollisionWithPlanet() override;

protected:
	virtual bool init() override;

private:
	CBonusLetter();
	virtual ~CBonusLetter(){};
	cocos2d::Vec2 m_TargetPos;
	eLETTER m_LetterNum;
};