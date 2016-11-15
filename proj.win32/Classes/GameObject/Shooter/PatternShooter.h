#pragma once
#include "ShooterLegacy.h"

struct sPATTERN_SHOOTER_PARAM
{
	int _index;
	std::string _patternName;
	int _height;
	int _width;
	float _widthAngleDistance;
	float _heightDistance;
	char _pattern[60 * 1200]; // 패턴의 최대 크기 60 * 60
};

/* RandomShooter : 360도 사방에서 랜덤으로 총알을 쏘는 패턴*/
class CPatternShooter : public CShooterLegacy {

public:
    CPatternShooter() : CShooterLegacy(sSHOOTER_PARAM(), 0){};
    virtual ~CPatternShooter(){}
    
	/* create를 호출하면 operator new가 호출되면서 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다. */
	static CPatternShooter* create(sSHOOTER_PARAM param, float distance = -1.f);
	virtual void Execute(float delta) override;

private:
    virtual bool init() override;
    void shootOnceByHeight(int height);
    
	// interval = Bullet 생성 간격
	CPatternShooter(sSHOOTER_PARAM param, float distance);
    
private:
    int m_PatternHeightMax;
    int m_PatternCurrentHeight;
    
    sPATTERN_SHOOTER_PARAM m_PatternParam;
    
    float m_Angle;
};