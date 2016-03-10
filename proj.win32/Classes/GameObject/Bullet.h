#pragma once

#include "Mover.h"
#include "../Common/HSHUtility.h"

class CBullet : public CMover {
public:
	virtual void ReturnToMemoryBlock() override;

protected:
	void* operator new (size_t size, const std::nothrow_t);
	CBullet(std::string textureName,
		float boundingRadius,
		float angle,
		float speed,
		CGameObject* target);
	virtual ~CBullet() = 0;

	//getter & setter
	CC_SYNTHESIZE(float, m_fAngle, Angle);
	CC_SYNTHESIZE(float, m_fBulletSpeed, BulletSpeed);

protected:
	std::string m_TextureName;
	Sprite* m_pTexture;
	CGameObject* m_Target;
};