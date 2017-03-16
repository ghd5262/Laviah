#pragma once
#include "State.h"


class CBullet;
//------------------------------------------------------------------------
class CBulletNormal : public CState<CBullet>
{
public:

	static CBulletNormal* Instance();

	void Enter(CBullet* bullet) override;

	void Execute(CBullet* bullet, float delta) override;

	void Exit(CBullet* bullet) override;

private:
	CBulletNormal(){}
	virtual ~CBulletNormal(){}
};

//------------------------------------------------------------------------
class CBulletBonusTime : public CState<CBullet>
{
public:

	static CBulletBonusTime* Instance();

	void Enter(CBullet* bullet) override;

	void Execute(CBullet* bullet, float delta) override;

	void Exit(CBullet* bullet) override;

private:
	CBulletBonusTime(){}
	virtual ~CBulletBonusTime(){}
};

//------------------------------------------------------------------------
class CBulletMagnetItem : public CState<CBullet>
{
public:

	static CBulletMagnetItem* Instance();

	void Enter(CBullet* bullet) override;

	void Execute(CBullet* bullet, float delta) override;

	void Exit(CBullet* bullet) override;

private:
	CBulletMagnetItem(){}
	virtual ~CBulletMagnetItem(){}
};