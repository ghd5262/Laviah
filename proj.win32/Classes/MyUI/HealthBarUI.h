#pragma once
#include "../GameObject/GameObject.h"
/*--------------------------CHealthBarUI--------------------------
* stratege pattern
* ���� ĳ���Ͷ� �ٸ� ü�°���Լ��� ����� �� �ֵ��� ����
* ������ ĳ���Ϳ��� virtual �Լ��� �������� �ʰ� stratege pattern�� �̿�
* (�ڼ��� ���� playerŬ���� ����)
* LambdaȤ�� �Լ�������, �Լ���ü�� ���ڷ� �޴´�.
* CHealthBarUI�� ö���� UI�� ǥ���Ѵ�.
--------------------------------------------------------------*/

class CPlayer;
class CHealthBarUI : public CGameObject
{
public: 
	// ĳ���͸����� ü�°���Լ� ������ ����
	typedef std::function<float(float delta)> HealthCalculatorFunc;

public:
	static CHealthBarUI* create(HealthCalculatorFunc healthCalFunc);
	// �÷����� ������ ���ʽ� Ÿ�Ӷ� ��� �Լ� ���氡������
	void ChangeHealthCalFunc(HealthCalculatorFunc healthCalFunc);	

	virtual void Execute(float delta = 0.f);

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	// ProgressBar Update
	void UpdateHealthUI(float delta);

	CHealthBarUI(HealthCalculatorFunc healthCalFunc)
		: m_HealthCalculatorFunction(healthCalFunc)
		, m_HealthBar(nullptr)
		, m_HealthBarBG(nullptr)
		, m_HealthBarImg(nullptr)
		{}
	virtual ~CHealthBarUI(){}

private:
	HealthCalculatorFunc m_HealthCalculatorFunction;
	ProgressTimer* m_HealthBar;
	Sprite* m_HealthBarBG;
	Sprite* m_HealthBarImg;
};

