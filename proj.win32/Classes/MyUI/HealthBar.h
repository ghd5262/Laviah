#pragma once
#include "../GameObject/GameObject.h"


class CHealthBar : public CGameObject
{
public:
	static CHealthBar* create(float lifeMax);
	virtual void Execute(float delta = 0.f);

	void Hit(float life);
	void AddLife(float life);

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CHealthBar(float maxLife) 
		: m_HealthBar(nullptr)
		, m_Health(nullptr)
		, m_HealthBarImg(nullptr)
		, m_fMaxLife(maxLife)
		, m_fLife(m_fMaxLife)
		, m_bIsAlive(true){}
	virtual ~CHealthBar(){}

private:
	ProgressTimer* m_HealthBar;
	Sprite* m_Health;
	Sprite* m_HealthBarImg;
	float m_fMaxLife;
	float m_fLife;
	bool m_bIsAlive;
};

