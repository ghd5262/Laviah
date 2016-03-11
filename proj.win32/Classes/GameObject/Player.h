#pragma once
#include "GameObject.h"

class CPlayer : public CGameObject {
public:
	static CPlayer* create(std::string textureName, float boundingRadius, float rotate, float rotateSpeed);
	virtual void Execute(float delta = 0.f) override;

	void setOriginPos(cocos2d::Vec2 pos) { m_OriginPos = pos; }
	cocos2d::Vec2 getOriginPos(){ return m_OriginPos; }
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
	cocos2d::Vec2 m_OriginPos;
	std::string m_TextureName;
	Sprite* m_pTexture;
};