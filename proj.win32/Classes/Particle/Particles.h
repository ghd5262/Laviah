#pragma once
#include "../Common/HSHUtility.h"
#include "2d/CCParticleSystemQuad.h"
#include "cocos2d.h"

class CParticle : public cocos2d::ParticleSystemQuad
{
public:
    std::string getTextureName() const { return m_TextureName; }
    void setTextureName(std::string name) { m_TextureName = name; }
    
    CParticle() : m_TextureName(""){}
    virtual ~CParticle(){}
    
protected:
    std::string m_TextureName;
};

class CParticle_Flame : public CParticle
{
public:
    static CParticle_Flame* create();
    
    virtual bool init() override;
    
private:
    CParticle_Flame(){}
    virtual ~CParticle_Flame(){}
};

class CParticle_Explosion : public CParticle
{
public:
	static CParticle_Explosion* create();

    virtual bool init() override;
    
protected:
    CParticle_Explosion(){}
	virtual ~CParticle_Explosion(){}
};

//300
class CParticle_Explosion_2 : public CParticle
{
public:
    static CParticle_Explosion_2* create();
    
    virtual bool init() override;
    
protected:
    CParticle_Explosion_2(){}
    virtual ~CParticle_Explosion_2(){}
};

//120
class CParticle_Line : public CParticle
{
public:
    static CParticle_Line* create();
    
    virtual bool init() override;
    
protected:
    CParticle_Line(){}
    virtual ~CParticle_Line(){}
};

//60
class CParticle_BackGround : public CParticle
{
public:
    static CParticle_BackGround* create();
    
    virtual bool init() override;
    
protected:
    CParticle_BackGround(){}
    virtual ~CParticle_BackGround(){}
};