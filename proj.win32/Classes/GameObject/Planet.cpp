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

float CPlanet::noise(int x, int y) {
	int n = x + y * 57;
	n = (n << 13) ^ n;
	return (1.0 - ((n * ((n * n * 15731) + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

void CPlanet::CrushShake(float interval, float duration, float speed, float magnitude) {

	//experiment more with these four values to get a rough or smooth effect!
	m_fElapsed = 0.0f;
    auto player = CObjectManager::Instance()->getPlayer();
    if(!player) return;
    
    if(this->isScheduled("Shake")) return;
    
	this->schedule([=](float dt) {
		float randomStart = random(-1000.0f, 1000.0f);
		m_fElapsed += dt;

		float percentComplete = m_fElapsed / duration;

		// We want to reduce the shake from full power to 0 starting half way through
		float damper = 1.0f - clampf(2.0f * percentComplete - 1.0f, 0.0f, 1.0f);

		// Calculate the noise parameter starting randomly and going as fast as speed allows
		float alpha = randomStart + speed * percentComplete;

		// map noise to [-1, 1]
		float x = noise(alpha, 0.0f) * 2.0f - 1.0f;
		float y = noise(0.0f, alpha) * 2.0f - 1.0f;

		x *= magnitude * damper;
		y *= magnitude * damper;
        
        
        m_Texture->setPosition(Vec2(getContentSize() * 0.5f) + Vec2(x, y));
        player->setTexturePos(Vec2(player->getContentSize() * 0.5f) + Vec2(x, y));
		if (m_fElapsed >= duration)
		{
			m_fElapsed = 0;
			this->unschedule("Shake");
            m_Texture->setPosition(this->getContentSize() / 2);
            player->setTexturePos(player->getContentSize() / 2);
		}

	}, interval, CC_REPEAT_FOREVER, 0.f, "Shake");
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