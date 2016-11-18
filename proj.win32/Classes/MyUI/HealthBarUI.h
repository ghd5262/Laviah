#pragma once
#include "../GameObject/GameObject.h"
/*--------------------------CHealthBarUI--------------------------
* strategy pattern
* 같은 캐릭터라도 다른 체력계산함수를 사용할 수 있도록 구현
* 때문에 캐릭터에서 virtual 함수로 구현하지 않고 strategy pattern을 이용
* 차후에 다른캐릭터로 이어달리기 등의 기능을 구현할때 용이함
* (자세한 것은 player클래스 참조)
* Lambda혹은 함수포인터, 함수객체를 인자로 받는다.
* CHealthBarUI는 철저히 UI만 표현한다.
--------------------------------------------------------------*/

class CPlayer;
class CHealthBarUI : public CGameObject
{
public: 
	// 캐릭터마다의 체력계산함수 포인터 전달
	typedef std::function<float(float delta)> HealthCalculatorFunc;

public:
	static CHealthBarUI* create(HealthCalculatorFunc healthCalFunc);
	// 플레이중 버프나 보너스 타임때 계산 함수 변경가능위해
	void ChangeHealthCalFunc(HealthCalculatorFunc healthCalFunc);	

	virtual void Execute(float delta = 0.f);

protected:
	virtual bool init() override;

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
	cocos2d::ProgressTimer* m_HealthBar;
	cocos2d::Sprite* m_HealthBarBG;
	cocos2d::Sprite* m_HealthBarImg;
};

