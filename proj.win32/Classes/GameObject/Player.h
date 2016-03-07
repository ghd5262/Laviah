#pragma once
#include "Mover.h"

class CPlayer : public CMover {

public:
	static CPlayer* create(float boundingRadius, float runSpeed, float scale);
	virtual void Execute(float delta = 0.f) override;

protected:
	bool init();
	bool initVariable();

	//getter & setter
	CC_SYNTHESIZE(float, m_fRunSpeed, RunSpeed);
	CC_SYNTHESIZE(float, m_fScale, Scale);

private:
	CPlayer(float boundingRadius, float runSpeed, float scale);
	virtual ~CPlayer();
};