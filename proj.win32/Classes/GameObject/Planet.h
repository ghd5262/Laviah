#pragma once
#include "GameObject.h"

namespace PLANET_DEFINE {
    static const float RADIUS = 180.f;
    static const float SCALE_SIZE = 1.5f;
    static const float ZOOMOUT_SIZE = 1.5f;
    static const float ZOOMIN_SIZE = 2.f;
    static const float ZOOMIN_2_SIZE = 4.f;
    static const float BOUNDING_RADIUS = RADIUS * SCALE_SIZE;
    static const cocos2d::Vec2 ZOOMOUT_POS  = cocos2d::Vec2(540.f, 672.f);
    static const cocos2d::Vec2 ZOOMIN_POS   = cocos2d::Vec2(540.f, 900.f);
    static const cocos2d::Vec2 ZOOMIN_2_POS = cocos2d::Vec2(540.f, 410.f);
}

class CPlanet : public CGameObject {
public:
	static CPlanet* create();
	
    void Clear();
    
	void CrushShake(float interval, float duration, float speed, float magnitude);
	virtual void Execute(float delta = 0.f) override;

	void Rotation(float speed);

	//getter & setter
    void setPlanetTexture(std::string textureName);

protected:
	virtual bool init() override;

	//getter & setter
	CC_SYNTHESIZE(float, m_Angle, Angle);

private:
	float noise(int x, int y);
	CPlanet();
	virtual ~CPlanet(){}

private:
	cocos2d::Sprite* m_Texture;
	float m_fElapsed;
};
