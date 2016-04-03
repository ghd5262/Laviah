#pragma once
#include "Shooter.h"

struct sPATTERN_SHOOTER_PARAM
{
	int _index;
	std::string _patternName;
	int _height;
	int _width;
	float _widthAngleDistance;
	float _heightDistance;
	int _pattern[60 * 60]; // 패턴의 최대 크기 60 * 60
};

/* RandomShooter : 360도 사방에서 랜덤으로 총알을 쏘는 패턴*/
class CPatternShooter : public CShooter {

public:
	/* create를 호출하면 operator new가 호출되면서 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다. */
	static CPatternShooter* create(sSHOOTER_PARAM param);

	virtual void Execute(float delta) override;
	virtual void ShootOnce() override;

	// 초기 각도와 거리를 받아서 Shoot
	// 코인 아이템 사용 시 미사일의 코인 변환에 사용할 수 있음 (참고 normalMissile)
	void ShootWithPosition(std::string patternName, float angle, float distance);

private:
	// interval = Bullet 생성 간격
	CPatternShooter(sSHOOTER_PARAM param);
	virtual ~CPatternShooter(){}
};