#pragma once
#include "GameObject.h"

namespace PLANET_DEFINE {
    static const float BOUNDING_RADIUS    = 393.5f;
    static const float MENU_SIZE          = 1.0f;
    static const float GAME_SIZE          = 0.66f;
    static const float SELECT_SIZE        = 2.0f;
    static const float RANK_SIZE          = 50.0f;
    static const cocos2d::Vec2 MENU_POS   = cocos2d::Vec2(540.f, 900.f);
    static const cocos2d::Vec2 GAME_POS   = cocos2d::Vec2(540.f, 672.f);
    static const cocos2d::Vec2 SELECT_POS = cocos2d::Vec2(540.f, 310.f);
    static const cocos2d::Vec2 RANK_POS   = cocos2d::Vec2(270.f, 1440.f);
}

class CPlanet : public CGameObject {
public:
	static CPlanet* create();
	
    void Clear();
	void Crushed();
    
	virtual void Execute(float delta = 0.f) override;

	void Rotation(float speed);

	//getter & setter
    void setPlanetTexture(std::string textureName);

protected:
	virtual bool init() override;

	//getter & setter
	CC_SYNTHESIZE(float, m_Angle, Angle);

private:
	CPlanet();
	virtual ~CPlanet(){}

private:
	cocos2d::Sprite* m_Texture;
	float m_fElapsed;
};
