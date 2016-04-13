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

class CGameObject;
class CBonusTimeUI;
class CPlayer;


class CBonusLetter : public CBullet {
public:
	/*create�� ȣ���ϸ� CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�.*/
	static CBonusLetter* create(
		sBULLET_PARAM bulletParam,
		float angle,				//bullet �ʱ� ���� 
		float speed);				//bullet �ʱ� �ӵ�

	virtual void Execute(float delta = 0.f) override;
	virtual void CollisionWithPlayer();
	virtual void CollisionWithPlanet();

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CBonusLetter(
		sBULLET_PARAM bulletParam,
		float angle,
		float speed);
	virtual ~CBonusLetter(){};

	CBonusTimeUI* m_pUIBonusTime;
	CPlayer* m_Player;
	Vec2 m_TargetPos;
	eLETTER m_LetterNum;
};