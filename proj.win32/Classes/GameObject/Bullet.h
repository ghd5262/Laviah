﻿#pragma once

#include "Mover.h"
#include "../Common/HSHUtility.h"

class CBullet : public CMover {
public:
	/*create를 호출하면 CObjectManager에서 메모리를 받는다.
	  받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
	static CBullet* create(			
		std::string textureName,	//bullet 이미지
		float boundingRadius,		//bullet 충돌 범위
		float angle,				//bullet 초기 각도 
		float speed,				//bullet 초기 속도
		const CMover* target);		//bullet 타겟

	virtual void Execute(float delta = 0.f) override;
	void* operator new (size_t size, const std::nothrow_t);
	CBullet() : m_pTexture(nullptr){}
	virtual ~CBullet(){};

protected:
	//getter & setter
	CC_SYNTHESIZE(float, m_fAngle, Angle);
	CC_SYNTHESIZE(float, m_fBulletSpeed, BulletSpeed);

private:
	bool initVariable(std::string textureName,
		float boundingRadius,
		float angle,
		float speed,
		const CMover* target);

private:
	std::string m_TextureName;
	Sprite* m_pTexture;
	const CMover* m_Target;
};