#pragma once
#include "../Common/HSHUtility.h"
#include "2d/CCParticleSystemQuad.h"

class CParticle_Flame : public ParticleSystemQuad
{
public:
	static CParticle_Flame* create(std::string textureName);
	static CParticle_Flame* createWithTotalParticles(int numberOfParticles, std::string textureName);

protected:
	CParticle_Flame(std::string textureName)
		: m_strTextureName(textureName){}
	virtual ~CParticle_Flame(){}

	bool init(){ return initWithTotalParticles(50); }
	virtual bool initWithTotalParticles(int numberOfParticles) override;

private:
	CC_DISALLOW_COPY_AND_ASSIGN(CParticle_Flame);
	std::string m_strTextureName;
};




class CParticle_Explosion : public ParticleSystemQuad
{
public:
	static CParticle_Explosion* create(std::string textureName);
	static CParticle_Explosion* createWithTotalParticles(int numberOfParticles, std::string textureName);

protected:
	CParticle_Explosion(std::string textureName)
		: m_strTextureName(textureName){}
	virtual ~CParticle_Explosion(){}

	bool init(){ return initWithTotalParticles(30); }
	virtual bool initWithTotalParticles(int numberOfParticles) override;

private:
	CC_DISALLOW_COPY_AND_ASSIGN(CParticle_Explosion);
	std::string m_strTextureName;
};




class CParticle_Explosion_2 : public ParticleSystemQuad
{
public:
	static CParticle_Explosion_2* create(std::string textureName);
	static CParticle_Explosion_2* createWithTotalParticles(int numberOfParticles, std::string textureName);

protected:
	CParticle_Explosion_2(std::string textureName)
		: m_strTextureName(textureName){}
	virtual ~CParticle_Explosion_2(){}

	bool init(){ return initWithTotalParticles(300); }
	virtual bool initWithTotalParticles(int numberOfParticles) override;

private:
	CC_DISALLOW_COPY_AND_ASSIGN(CParticle_Explosion_2);
	std::string m_strTextureName;
};




class CParticle_Line : public ParticleSystemQuad
{
public:
	static CParticle_Line* create(std::string textureName);
	static CParticle_Line* createWithTotalParticles(int numberOfParticles, std::string textureName);

protected:
	CParticle_Line(std::string textureName)
		: m_strTextureName(textureName){}
	virtual ~CParticle_Line(){}

	bool init(){ return initWithTotalParticles(120); }
	virtual bool initWithTotalParticles(int numberOfParticles) override;

private:
	CC_DISALLOW_COPY_AND_ASSIGN(CParticle_Line);
	std::string m_strTextureName;
};