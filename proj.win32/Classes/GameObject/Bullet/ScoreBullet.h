#pragma once
#include "Bullet.h"

class CGameObject;

class CScoreBullet : public CBullet {
public:
	/*create�� ȣ���ϸ� CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�.*/
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