#pragma once
#include "Bullet.h"

class CGameObject;

class CScoreBullet : public CBullet {
public:
	/*create를 호출하면 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
	static CScoreBullet* create(int score = 0);
	virtual void Execute(float delta = 0.f) override {};
	virtual void Rotation(float dir, float delta) override {};

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CScoreBullet(int score);
	virtual ~CScoreBullet(){};

private:
	int m_nScore;
	std::string m_strScore;
	Label* m_labelScore;
};