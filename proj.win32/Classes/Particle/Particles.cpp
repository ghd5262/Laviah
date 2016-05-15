#include "Particles.h"



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
		this->_life = 0.4f;
		this->_lifeVar = 1;

		// emits per frame
		this->_emissionRate = _totalParticles / _life;

		// color of particles
		_startColor.r = 0.5f;
		_startColor.g = 0.5f;
		_startColor.b = 0.5f;
		_startColor.a = 1.0f;
		_startColorVar.r = 0.5f;
		_startColorVar.g = 0.5f;
		_startColorVar.b = 0.5f;
		_startColorVar.a = 0.1f;
		_endColor.r = 0.1f;
		_endColor.g = 0.1f;
		_endColor.b = 0.1f;
		_endColor.a = 0.2f;
		_endColorVar.r = 0.1f;
		_endColorVar.g = 0.1f;
		_endColorVar.b = 0.1f;
		_endColorVar.a = 0.2f;

		// size, in pixels
		_startSize = 30.0f;
		_startSizeVar = 4.0f;
		_endSize = 5;

		Texture2D* texture = nullptr;
		texture = Director::getInstance()->getTextureCache()->getTextureForKey(m_strTextureName);

		if (texture != nullptr)
		{
			setTexture(texture);
		}
		else
		{
			setTexture(Director::getInstance()->getTextureCache()->addImage(m_strTextureName));
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
		this->modeA.speed = 150;
		this->modeA.speedVar = 200;

		// emitter position
		Size winSize = Director::getInstance()->getWinSize();
		this->setPosition(winSize.width / 2, winSize.height / 2);

		// angle
		this->_angle = 0;
		this->_angleVar = 80;
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
		_startColorVar.r = 0.5f;
		_startColorVar.g = 0.5f;
		_startColorVar.b = 0.5f;
		_startColorVar.a = 0.1f;
		_endColor.r = 0.0f;
		_endColor.g = 0.0f;
		_endColor.b = 0.0f;
		_endColor.a = 0.1f;
		_endColorVar.r = 0.0f;
		_endColorVar.g = 0.0f;
		_endColorVar.b = 0.0f;
		_endColorVar.a = 0.2f;

		// size, in pixels
		_startSize = 50.f;
		_startSizeVar = 4.f;
		_endSize = 30.f;

		Texture2D* texture = nullptr;
		texture = Director::getInstance()->getTextureCache()->getTextureForKey(m_strTextureName);
		
		if (texture != nullptr)
		{
			setTexture(texture);
		}
		else
		{
			setTexture(Director::getInstance()->getTextureCache()->addImage(m_strTextureName));
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

		Texture2D* texture = nullptr;
		texture = Director::getInstance()->getTextureCache()->getTextureForKey(m_strTextureName);

		if (texture != nullptr)
		{
			setTexture(texture);
		}
		else
		{
			setTexture(Director::getInstance()->getTextureCache()->addImage(m_strTextureName));
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
		this->setPosVar(Vec2(350,5));
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
		_startSize = 10.f;
		_endSize = 5.f;

		Texture2D* texture = nullptr;
		texture = Director::getInstance()->getTextureCache()->getTextureForKey(m_strTextureName);

		if (texture != nullptr)
		{
			setTexture(texture);
		}
		else
		{
			setTexture(Director::getInstance()->getTextureCache()->addImage(m_strTextureName));
		}
		// additive
		this->setBlendAdditive(false);
		return true;
	}
	return false;
}