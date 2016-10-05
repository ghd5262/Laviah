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

	bool init() override { return initWithTotalParticles(50); }
	virtual bool initWithTotalParticles(int numberOfParticles) override;

private:
	CC_DISALLOW_COPY_AND_ASSIGN(CParticle_Flame);
	std::string m_strTextureName;
};



class CParticle_Character : public ParticleSystemQuad
{
public:
    static CParticle_Character* create(std::string textureName);
    static CParticle_Character* createWithTotalParticles(int numberOfParticles, std::string textureName);
    
protected:
    CParticle_Character(std::string textureName)
    : m_strTextureName(textureName){}
    virtual ~CParticle_Character(){}
    
    bool init() override { return initWithTotalParticles(50); }
    virtual bool initWithTotalParticles(int numberOfParticles) override;
    
private:
    CC_DISALLOW_COPY_AND_ASSIGN(CParticle_Character);
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

	bool init() override { return initWithTotalParticles(50); }
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

	bool init() override { return initWithTotalParticles(300); }
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

	bool init() override { return initWithTotalParticles(120); }
	virtual bool initWithTotalParticles(int numberOfParticles) override;

private:
	CC_DISALLOW_COPY_AND_ASSIGN(CParticle_Line);
	std::string m_strTextureName;
};



class CParticle_BackGround : public ParticleSystemQuad
{
public:
    static CParticle_BackGround* create(std::string textureName);
    static CParticle_BackGround* createWithTotalParticles(int numberOfParticles, std::string textureName);
    
protected:
    CParticle_BackGround(std::string textureName)
    : m_strTextureName(textureName){}
    virtual ~CParticle_BackGround(){}
    
    bool init() override { return initWithTotalParticles(60); }
    virtual bool initWithTotalParticles(int numberOfParticles) override;
    
private:
    CC_DISALLOW_COPY_AND_ASSIGN(CParticle_BackGround);
    std::string m_strTextureName;
};