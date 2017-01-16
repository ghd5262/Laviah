#pragma once
#include "GameObject.h"


class CItemRange : public CGameObject {
	const float BARRIER_TIMER = 1.5f;
	const float BARRIER_SIZE = 3.f;

public:
    static CItemRange* create(std::string textureName,
                              float delayTime/* = 0*/);
    void ItemGet();
    virtual void Execute(float delta) override;
    
protected:
    virtual bool init() override;
    
    //getter & setter
	CC_SYNTHESIZE(bool, m_IsItemTimeUP, IsItemTimeUP);
    
private:
    void itemTimeUP();
    CItemRange(std::string textureName,
               float delayTime);
    virtual ~CItemRange(){}
    
private:
    std::string m_TextureName;
    float m_OriginBoundingRadius;
    float m_DelayTime;
};