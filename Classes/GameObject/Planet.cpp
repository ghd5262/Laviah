#include "Planet.h"
#include "Player.h"
#include "ObjectManager.h"
#include "BulletCreator.h"
#include "../Scene/GameScene.h"

using namespace cocos2d;
using namespace PLANET_DEFINE;

CPlanet* CPlanet::create()
{
	CPlanet *pRet = new(std::nothrow) CPlanet();
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

CPlanet::CPlanet()
: m_Angle(0.f)
, m_fElapsed(0.0f)
{}

bool CPlanet::init()
{
	//this->DrawDebugBinding();   //for debug
    if (!Node::init()) return false;
    
	auto data = CObjectManager::Instance()->getCharacterParam();
	m_Texture = Sprite::createWithSpriteFrameName(data->_planetTextureName);
    if (m_Texture != nullptr){
		this->setContentSize(m_Texture->getContentSize());
		m_Texture->setPosition(this->getContentSize() / 2);
        m_Texture->setAnchorPoint(Vec2(0.5f, 0.5f));
        m_Texture->setCascadeColorEnabled(true);
        addChild(m_Texture);
    }
    
//    // blur test
//    auto getGLProgram = [=](std::string fileName){
//        auto fileUtiles = FileUtils::getInstance();
//        auto fragmentFullPath = fileUtiles->fullPathForFilename(fileName);
//        auto fragSource = fileUtiles->getStringFromFile(fragmentFullPath);
//        auto glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert,
//                                                         fragSource.c_str());
//    
//        return GLProgramState::getOrCreateWithGLProgram(glprogram);
//    };
//
//    m_Texture->setGLProgramState(getGLProgram("example_Blur.fsh"));
//    Size size = m_Texture->getTexture()->getContentSizeInPixels();
//    m_Texture->getGLProgramState()->setUniformVec2("resolution", size);
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
//    m_Texture->getGLProgramState()->setUniformFloat("blurRadius", 10);
//    m_Texture->getGLProgramState()->setUniformFloat("sampleNum", 100);
//#endif
    
//    this->setScale(PLANET_DEFINE::SCALE_SIZE);

    return true;
}

void CPlanet::Clear()
{
    this->setRotation(0);
}

void CPlanet::Crushed()
{
    CObjectManager::Shake(0.01f, 0.2f, 0.1f, 3.0f, m_Texture, this->getContentSize() / 2);
    CObjectManager::Instance()->getPlayer()->Crushed();
}

void CPlanet::Execute(float delta){
    this->setBoundingRadius(((getContentSize().width * this->getScale()) / 2));
}

// Dir -1 == Left, 1 == Right
void CPlanet::Rotation(float speed)
{
    m_Angle = this->getRotation() + speed;
    
//	m_Angle = static_cast<int>(m_Angle) % 360;
	this->setRotation(m_Angle);
}

void CPlanet::setPlanetTexture(std::string textureName)
{
    if(m_Texture != nullptr){
        this->m_Texture->setSpriteFrame(textureName);
    }
}