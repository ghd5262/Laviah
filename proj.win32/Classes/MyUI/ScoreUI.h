#pragma once
#include "../GameObject/GameObject.h"

class CScoreUI : public CGameObject
{
public:
	static CScoreUI* create();

	virtual void Execute(float delta = 0.f);

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CScoreUI(){};
	virtual ~CScoreUI(){};

private:
	std::string m_Score;
};

