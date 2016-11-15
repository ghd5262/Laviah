#include "ItemBarrier.h"
#include "../Particle/Particles.h"

using namespace cocos2d;

CItemBarrier* CItemBarrier::create(std::string textureName, float boundingRadius)
{
    CItemBarrier *pRet = new(std::nothrow) CItemBarrier(textureName, boundingRadius);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

CItemBarrier::CItemBarrier(std::string textureName, float boundingRadius)
	: CGameObject(0.f)
, m_TextureName(textureName)
, m_OriginBoundingRadius(boundingRadius)
, m_bBarrierAlive(false)
{
}

bool CItemBarrier::init()
{
    //this->DrawDebugBinding();
    if (!initVariable())
        return false;
    return true;
}

bool CItemBarrier::initVariable()
{
	m_pTexture = Sprite::create(m_TextureName);
	if (m_pTexture != nullptr){
		m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
		addChild(m_pTexture);
	}

	/*m_pParticle = CParticle_Flame::create(m_TextureName);
	if (m_pParticle != nullptr){
	m_pParticle->retain();
	m_pParticle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	addChild(m_pParticle, 101);
	}*/

	m_pTexture->setScale(0.f);
	m_pTexture->setVisible(false);

    return true;
}

void CItemBarrier::Execute(float delta/* = 0.f*/)
{
	if (m_bBarrierAlive)
	{
		setBRadius((m_OriginBoundingRadius * m_pTexture->getScale()) * 0.5f);
	}
}


