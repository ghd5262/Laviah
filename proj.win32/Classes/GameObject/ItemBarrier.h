#pragma once
#include "GameObject.h"


class CItemBarrier : public CGameObject {
	const float BARRIER_TIMER = 1.5f;
	const float BARRIER_SIZE = 3.f;

public:
    static CItemBarrier* create(std::string textureName, float boundingRadius);
    inline void GotBarrierItem(){
		CAudioManager::Instance()->PlayEffectSound("sounds/barrier.mp3", false);
        m_pTexture->setVisible(true);
		m_bBarrierAlive = true;

		auto action = Sequence::create(
			ScaleTo::create(BARRIER_TIMER, BARRIER_SIZE),
			CallFunc::create([&](){
			FinishedBarrierItem();
		}), nullptr);
		m_pTexture->runAction(action);
    }
    virtual void Execute(float delta = 0.f) override;
    
protected:
    virtual bool init() override;
    virtual bool initVariable() override;
    
    //getter & setter
	CC_SYNTHESIZE(bool, m_bBarrierAlive, BarrierAlive);
    
private:
	inline void FinishedBarrierItem()
	{
		m_pTexture->setScale(0.f);
		m_pTexture->setVisible(false);
		setBRadius(0);
		m_bBarrierAlive = false;
	}
    CItemBarrier(std::string textureName, float boundingRadius);
    virtual ~CItemBarrier(){}
    
private:
    float m_OriginBoundingRadius;
    std::string m_TextureName;
    Sprite* m_pTexture;
   // ParticleSystemQuad* m_pParticle;
};