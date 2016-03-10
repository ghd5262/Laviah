#pragma once
#include "GameObject.h"

class CPlayer : public CGameObject {
public:
	static CPlayer* create(std::string textureName, float boundingRadius, float rotate, float rotateSpeed);
	virtual void Execute(float delta = 0.f) override;

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

	//getter & setter
	CC_SYNTHESIZE(float, m_fRotate, Rotate);
	CC_SYNTHESIZE(float, m_fRotateSpeed, RotateSpeed);

private:
	CPlayer(std::string textureName, float boundingRadius, float rotate, float rotateSpeed);
	virtual ~CPlayer(){}

private:
	std::string m_TextureName;
	Sprite* m_pTexture;
};