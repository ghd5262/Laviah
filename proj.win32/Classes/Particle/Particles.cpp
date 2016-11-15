#include "Particles.h"

using namespace cocos2d;


CParticle_Flame* CParticle_Flame::create(std::string textureName)
{
	CParticle_Flame* ret = new (std::nothrow) CParticle_Flame(textureName);
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

CParticle_Flame* CParticle_Flame::createWithTotalParticles(int numberOfParticles, std::string textureName)
{
	CParticle_Flame* ret = new (std::nothrow) CParticle_Flame(textureName);
	if (ret && ret->initWithTotalParticles(numberOfParticles))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool CParticle_Flame::initWithTotalParticles(int numberOfParticles)
{
	if (ParticleSystemQuad::initWithTotalParticles(numberOfParticles))
	{
		// duration
		_duration = DURATION_INFINITY;

		// Gravity Mode
		this->_emitterMode = Mode::GRAVITY;

		// Gravity Mode: gravity
		this->modeA.gravity = Vec2(90, 0);

		// Gravity Mode:  radial
		this->modeA.radialAccel = 0;
		this->modeA.radialAccelVar = 0;

		//  Gravity Mode: speed of particles
		this->modeA.speed = 50;
		this->modeA.speedVar = 50;

		// emitter position
		Size winSize = Director::getInstance()->getWinSize();
		this->setPosition(winSize.width / 2, winSize.height / 2);

		// angle
		this->_angle = 0;
		this->_angleVar = 20;

		// life of particles
		this->_life = 0.8f;
		this->_lifeVar = 1.5;

		// emits per frame
		this->_emissionRate = _totalParticles / _life;

		// color of particles
		_startColor.r = 1.0f;
		_startColor.g = 1.0f;
		_startColor.b = 1.0f;
		_startColor.a = 0.8f;
		_startColorVar.r = 0.8f;
		_startColorVar.g = 0.8f;
		_startColorVar.b = 0.8f;
		_startColorVar.a = 0.5f;
		_endColor.r = 0.8f;
		_endColor.g = 0.8f;
		_endColor.b = 0.8f;
		_endColor.a = 0.1f;
		_endColorVar.r = 0.8f;
		_endColorVar.g = 0.8f;
		_endColorVar.b = 0.8f;
		_endColorVar.a = 0.1f;

		// size, in pixels
		_startSize = 30.0f;
		_startSizeVar = 4.0f;
		_endSize = 5;

        SpriteFrame* spriteFrame = nullptr;
        spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(m_strTextureName);
        
        if (spriteFrame != nullptr)
        {
            this->setTextureWithRect(spriteFrame->getTexture(), spriteFrame->getRect());
        }
        else{
            Texture2D* texture = nullptr;
            texture = Director::getInstance()->getTextureCache()->getTextureForKey(m_strTextureName);
            
            if (texture != nullptr)
            {
                setTexture(texture);
            }
            else
            {
                texture = Director::getInstance()->getTextureCache()->addImage(m_strTextureName);
                if(texture != nullptr){
                    setTexture(texture);
                }
                else
                    return false;
            }
        }
		// additive
		this->setBlendAdditive(false);
		return true;
	}
	return false;
}


CParticle_Explosion* CParticle_Explosion::create(std::string textureName)
{
    CParticle_Explosion* ret = new (std::nothrow) CParticle_Explosion(textureName);
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

CParticle_Explosion* CParticle_Explosion::createWithTotalParticles(int numberOfParticles, std::string textureName)
{
	CParticle_Explosion* ret = new (std::nothrow) CParticle_Explosion(textureName);
	if (ret && ret->initWithTotalParticles(numberOfParticles))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool CParticle_Explosion::initWithTotalParticles(int numberOfParticles)
{
	if (ParticleSystemQuad::initWithTotalParticles(numberOfParticles))
	{
		this->setAutoRemoveOnFinish(true);

		// duration
		_duration = 0.1f;

		// Gravity Mode
		this->_emitterMode = Mode::GRAVITY;

		// Gravity Mode: gravity
		this->modeA.gravity = Vec2(90, 0);

		// Gravity Mode:  radial
		this->modeA.radialAccel = 0;
		this->modeA.radialAccelVar = 0;

		//  Gravity Mode: speed of particles
		this->modeA.speed = 200;
		this->modeA.speedVar = 100;

		// emitter position
		Size winSize = Director::getInstance()->getWinSize();
		this->setPosition(winSize.width / 2, winSize.height / 2);

		// angle
		this->_angle = 0;
		this->_angleVar = 50;
		// life of particles
		this->_life = 0.15f;
		this->_lifeVar = 0.2f;

		// emits per frame
		this->_emissionRate = _totalParticles / _life;

		// color of particles
		_startColor.r = 1.f;
		_startColor.g = 1.f;
		_startColor.b = 1.f;
		_startColor.a = 1.0f;
		_endColor.r = 1.f;
		_endColor.g = 1.f;
		_endColor.b = 1.f;
		_endColor.a = 0.0f;

		// size, in pixels
		_startSize = 30.f;
		_startSizeVar = 4.f;
		_endSize = 10.f;

        SpriteFrame* spriteFrame = nullptr;
        spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(m_strTextureName);
        
        if (spriteFrame != nullptr)
        {
            this->setTextureWithRect(spriteFrame->getTexture(), spriteFrame->getRect());
        }
        else{
            Texture2D* texture = nullptr;
            texture = Director::getInstance()->getTextureCache()->getTextureForKey(m_strTextureName);
            
            if (texture != nullptr)
            {
                setTexture(texture);
            }
            else
            {
                texture = Director::getInstance()->getTextureCache()->addImage(m_strTextureName);
                if(texture != nullptr){
                    setTexture(texture);
                }
                else
                    return false;
            }
        }
		// additive
		this->setBlendAdditive(false);
		return true;
	}
	return false;
}




CParticle_Explosion_2* CParticle_Explosion_2::create(std::string textureName)
{
	CParticle_Explosion_2* ret = new (std::nothrow) CParticle_Explosion_2(textureName);
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

CParticle_Explosion_2* CParticle_Explosion_2::createWithTotalParticles(int numberOfParticles, std::string textureName)
{
	CParticle_Explosion_2* ret = new (std::nothrow) CParticle_Explosion_2(textureName);
	if (ret && ret->initWithTotalParticles(numberOfParticles))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool CParticle_Explosion_2::initWithTotalParticles(int numberOfParticles)
{
	if (ParticleSystemQuad::initWithTotalParticles(numberOfParticles))
	{
		this->setAutoRemoveOnFinish(true);

		// duration
		_duration = 0.2f;

		// Gravity Mode
		this->_emitterMode = Mode::RADIUS;

		// Gravity Mode: gravity
		//this->modeA.gravity = Vec2(0, -900);
		// radius mode: start and end radius in pixels
		this->setStartRadius(0);
		this->setStartRadiusVar(0);
		this->setEndRadius(160);
		this->setEndRadiusVar(0);

		// Gravity Mode:  radial
		this->modeA.radialAccel = 0;
		this->modeA.radialAccelVar = 0;

		//  Gravity Mode: speed of particles
		this->modeA.speed = 400;
		this->modeA.speedVar = 100;

		// emitter position
		Size winSize = Director::getInstance()->getWinSize();
		this->setPosition(winSize.width / 2, winSize.height / 2);

		// angle
		this->_angle = 360;
		this->_angleVar = 360;
		// life of particles
		this->_life = 1.5f;
		this->_lifeVar = 0;

		// emits per frame
		this->_emissionRate = _totalParticles / _life;

		// color of particles
		_startColor.r = 1.f;
		_startColor.g = 1.f;
		_startColor.b = 1.f;
		_startColor.a = 1.f;
		_startColorVar.r = 0.0f;
		_startColorVar.g = 0.0f;
		_startColorVar.b = 0.0f;
		_startColorVar.a = 0.0f;
		_endColor.r = 1.0f;
		_endColor.g = 1.0f;
		_endColor.b = 1.0f;
		_endColor.a = 1.0f;
		_endColorVar.r = 0.0f;
		_endColorVar.g = 0.0f;
		_endColorVar.b = 0.0f;
		_endColorVar.a = 0.0f;

		// size, in pixels
		_startSize = 20.f;
		_startSizeVar = 0.f;
		_endSize = 0.f;

        SpriteFrame* spriteFrame = nullptr;
        spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(m_strTextureName);
        
        if (spriteFrame != nullptr)
        {
            this->setTextureWithRect(spriteFrame->getTexture(), spriteFrame->getRect());
        }
        else{
            Texture2D* texture = nullptr;
            texture = Director::getInstance()->getTextureCache()->getTextureForKey(m_strTextureName);
            
            if (texture != nullptr)
            {
                setTexture(texture);
            }
            else
            {
                texture = Director::getInstance()->getTextureCache()->addImage(m_strTextureName);
                if(texture != nullptr){
                    setTexture(texture);
                }
                else
                    return false;
            }
        }
        
        // additive
        this->setBlendAdditive(false);
        return true;
    }
    return false;
}


#pragma mark - Particle_Line
CParticle_Line* CParticle_Line::create(std::string textureName)
{
	CParticle_Line* ret = new (std::nothrow) CParticle_Line(textureName);
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

CParticle_Line* CParticle_Line::createWithTotalParticles(int numberOfParticles, std::string textureName)
{
	CParticle_Line* ret = new (std::nothrow) CParticle_Line(textureName);
	if (ret && ret->initWithTotalParticles(numberOfParticles))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool CParticle_Line::initWithTotalParticles(int numberOfParticles)
{
	if (ParticleSystemQuad::initWithTotalParticles(numberOfParticles))
	{
		this->setAutoRemoveOnFinish(true);

		// duration
		_duration = DURATION_INFINITY;

		// Gravity Mode
		this->_emitterMode = Mode::GRAVITY;

		// Gravity Mode: gravity
		this->modeA.gravity = Vec2(90, 0);

		// Gravity Mode:  radial
		this->modeA.radialAccel = 0;
		this->modeA.radialAccelVar = 0;

		//  Gravity Mode: speed of particles
		this->modeA.speed = 3;
		this->modeA.speedVar = 3;

		// emitter position
		Size winSize = Director::getInstance()->getWinSize();
		this->setPosition(winSize.width / 2, winSize.height / 2);
		this->setPosVar(Vec2(450,10));
		this->setPositionType(ParticleSystem::PositionType::GROUPED);

		// angle
		this->_angle = 0;
		this->_angleVar = 80;
		// life of particles
		this->_life = 2.0f;
		this->_lifeVar = 0.5f;

		// emits per frame
		this->_emissionRate = _totalParticles / _life;

		// color of particles
		_startColor.r = 1.f;
		_startColor.g = 1.f;
		_startColor.b = 1.f;
		_startColor.a = 1.0f;
		_endColor.r = 1.0f;
		_endColor.g = 1.0f;
		_endColor.b = 1.0f;
		_endColor.a = 0.1f;

		// size, in pixels
		_startSize = 20.f;
		_endSize = 10.f;

        SpriteFrame* spriteFrame = nullptr;
        spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(m_strTextureName);
        
        if (spriteFrame != nullptr)
        {
            this->setTextureWithRect(spriteFrame->getTexture(), spriteFrame->getRect());
        }
        else{
            Texture2D* texture = nullptr;
            texture = Director::getInstance()->getTextureCache()->getTextureForKey(m_strTextureName);
            
            if (texture != nullptr)
            {
                setTexture(texture);
            }
            else
            {
                texture = Director::getInstance()->getTextureCache()->addImage(m_strTextureName);
                if(texture != nullptr){
                    setTexture(texture);
                }
                else
                    return false;
            }
        }
		// additive
		this->setBlendAdditive(false);
		return true;
	}
	return false;
}


#pragma mark - Particle_BackGround
CParticle_BackGround* CParticle_BackGround::create(std::string textureName)
{
    CParticle_BackGround* ret = new (std::nothrow) CParticle_BackGround(textureName);
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

CParticle_BackGround* CParticle_BackGround::createWithTotalParticles(int numberOfParticles, std::string textureName)
{
    CParticle_BackGround* ret = new (std::nothrow) CParticle_BackGround(textureName);
    if (ret && ret->initWithTotalParticles(numberOfParticles))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool CParticle_BackGround::initWithTotalParticles(int numberOfParticles)
{
    if (ParticleSystemQuad::initWithTotalParticles(numberOfParticles))
    {
        this->setAutoRemoveOnFinish(true);
        
        // duration
        _duration = DURATION_INFINITY;
        
        // Gravity Mode
        this->_emitterMode = Mode::GRAVITY;
        
        // Gravity Mode: gravity
        this->modeA.gravity = Vec2(0, 0);
        
        // Gravity Mode:  radial
        this->modeA.radialAccel = 0;
        this->modeA.radialAccelVar = 0;
        
        //  Gravity Mode: speed of particles
        this->modeA.speed = 3;
        this->modeA.speedVar = 3;
        
        // emitter position
        Size winSize = Director::getInstance()->getWinSize();
        this->setPosVar(Vec2(winSize.width, winSize.height * 0.25f));
        this->setPositionType(ParticleSystem::PositionType::GROUPED);
        
        // angle
        this->_angle = 90;
        this->_angleVar = 360;
        // life of particles
        this->_life = 2.0f;
        this->_lifeVar = 0.5f;
        
        this->_startSpin = 0.f;
        this->_startSpinVar = 360.f;
        
        this->_endSpin = 360.f;
        this->_endSpinVar = 360.f;
        
        this->modeA.speed = 10;
        this->modeA.speedVar = 3;
        
        // emits per frame
        this->_emissionRate = _totalParticles / _life;
        
        // color of particles
        _startColor.r = 1.f;
        _startColor.g = 1.f;
        _startColor.b = 1.f;
        _startColor.a = 1.0f;
        _startColorVar.a = 0.4f;
        
        _endColor.r = 1.0f;
        _endColor.g = 1.0f;
        _endColor.b = 1.0f;
        _endColor.a = 0.0f;
        
        // size, in pixels
        _startSize = 10.f;
        _startSizeVar = 5.f;
        _endSize = 0.f;
        
        SpriteFrame* spriteFrame = nullptr;
        spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(m_strTextureName);
        
        if (spriteFrame != nullptr)
        {
            this->setTextureWithRect(spriteFrame->getTexture(), spriteFrame->getRect());
        }
        else{
            Texture2D* texture = nullptr;
            texture = Director::getInstance()->getTextureCache()->getTextureForKey(m_strTextureName);
            
            if (texture != nullptr)
            {
                setTexture(texture);
            }
            else
            {
                texture = Director::getInstance()->getTextureCache()->addImage(m_strTextureName);
                if(texture != nullptr){
                    setTexture(texture);
                }
                else
                    return false;
            }
        }
        // additive
        this->setBlendAdditive(false);
        return true;
    }
    return false;
}