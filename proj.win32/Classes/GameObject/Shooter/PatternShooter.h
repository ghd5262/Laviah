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
	int _pattern[60 * 60]; // ������ �ִ� ũ�� 60 * 60
};

/* RandomShooter : 360�� ��濡�� �������� �Ѿ��� ��� ����*/
class CPatternShooter : public CShooter {

public:
	/* create�� ȣ���ϸ� operator new�� ȣ��Ǹ鼭 CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�. */
	static CPatternShooter* create(sSHOOTER_PARAM param);

	virtual void Execute(float delta) override;
	virtual void ShootOnce() override;

	// �ʱ� ������ �Ÿ��� �޾Ƽ� Shoot
	// ���� ������ ��� �� �̻����� ���� ��ȯ�� ����� �� ���� (���� normalMissile)
	void ShootWithPosition(std::string patternName, float angle, float distance);

private:
	// interval = Bullet ���� ����
	CPatternShooter(sSHOOTER_PARAM param);
	virtual ~CPatternShooter(){}
};