#pragma once
#include "GameObject.h"

namespace ITEMRANGE{
	const float STAY_DISTANCE = 700.f;
	const float STAY_DURATION = 0.5f;
	const float TARGET_DISTANCE = 1600.f;
	const float TARGET_DURATION = 0.5f;
};

class CItemRange : public CGameObject {
public:
    static CItemRange* create();
	CItemRange* setTextureName(std::string name);
	CItemRange* show(cocos2d::Node* parent, int zOrder = 0);

	void ItemGet(float time);
	void Clear();

    virtual void update(float delta) override;

protected:
    
    //getter & setter
	CC_SYNTHESIZE(bool, m_IsStayStatus, IsStayStatus);
    
private:
	void stayTimeUP();

    CItemRange();
    virtual ~CItemRange(){}
    
private:
	cocos2d::Sprite* m_Texture;
    std::string m_TextureName;
	float m_StayLimitTime;
	float m_StayTime;
	float m_TargetDuration;
};