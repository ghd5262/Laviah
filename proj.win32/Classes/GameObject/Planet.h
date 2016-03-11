#pragma once
#include "GameObject.h"

class CPlanet : public CGameObject {
public:
	static CPlanet* create(std::string textureName, float boundingRadius, float rotate, float rotateSpeed);
	virtual void Execute(float delta = 0.f) override;

	void setOriginPos(cocos2d::Vec2 pos) { m_OriginPos = pos; }
	cocos2d::Vec2 getOriginPos(){ return m_OriginPos; }
	void CPlanet::CrushShake();

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

	//getter & setter
	CC_SYNTHESIZE(float, m_fRotate, Rotate);
	CC_SYNTHESIZE(float, m_fRotateSpeed, RotateSpeed);

private:
	float CPlanet::noise(int x, int y);
	CPlanet(std::string textureName, float boundingRadius, float rotate, float rotateSpeed);
	virtual ~CPlanet(){}

private:
	cocos2d::Vec2 m_OriginPos;
	std::string m_TextureName;
	Sprite* m_pTexture;
	float m_fElapsed;
};
