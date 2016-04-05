#pragma once
#include "Bullet.h"

class CGameObject;
class CBonusTimeUI;
class CPlayer;
enum eLETTER;
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