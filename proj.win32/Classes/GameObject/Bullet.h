#pragma once

#include "Mover.h"
#include "../Common/HSHUtility.h"

class CBullet : public CMover {
public:
	CBullet(){}
	static CBullet* create(
		std::string textureName,	//bullet 이미지
		float boundingRadius,		//bullet 충돌 범위
		float angle,				//bullet 초기 각도 
		float speed,				//bullet 초기 속도
		const CMover* target);			//bullet 타겟

	virtual void Execute(float delta = 0.f) override;

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

	//getter & setter
	CC_SYNTHESIZE(float, m_fAngle, Angle);
	CC_SYNTHESIZE(float, m_fBulletSpeed, BulletSpeed);

private:
	CBullet(std::string textureName, float boundingRadius, float angle, float speed, const CMover* target);
	virtual ~CBullet(){};

	void* operator new (size_t size, const std::nothrow_t);

private:
	std::string m_TextureName;
	Sprite* m_pTexture;
	const CMover* m_Target;
};