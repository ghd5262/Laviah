#pragma once
#include "GameObject.h"


class CItemRange : public CGameObject {
public:
    static CItemRange* create();
	CItemRange* setTextureName(std::string name);
    CItemRange* setTargetDuration(float duration);
    CItemRange* setTargetDistance(float distance);
	CItemRange* show(cocos2d::Node* parent, int zOrder = 0);

	void ItemGet(float time);
	void Clear();

    virtual void update(float delta) override;

private:
    CItemRange();
    virtual ~CItemRange(){}
    
private:
	cocos2d::Sprite* m_Texture;
    std::string m_TextureName;
	float m_StayLimitTime;
	float m_StayTime;
	float m_TargetDuration;
    float m_TargetDistance;
};
