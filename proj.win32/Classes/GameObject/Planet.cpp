#include "Planet.h"
#include "ObjectManager.h"
#include "Player.h"

CPlanet* CPlanet::create(
	std::string textureName,
	float boundingRadius,
	float rotate,
	float rotateSpeed)
{
	CPlanet *pRet = new(std::nothrow) CPlanet(
		 textureName
		, boundingRadius
		, rotate
		, rotateSpeed);
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

CPlanet::CPlanet(std::string textureName, float boundingRadius, float rotate, float rotateSpeed)
	: CGameObject(boundingRadius)
	, m_TextureName(textureName)
	, m_fRotate(rotate)
	, m_fRotateSpeed(rotateSpeed)
	, m_fElapsed(0.0f)
{
}

bool CPlanet::init()
{
	//this->DrawDebugBinding();
	if (!initVariable())
		return false;
	return true;
}

bool CPlanet::initVariable()
{
	try{
		m_pTexture = Sprite::create(m_TextureName);
		if (m_pTexture != nullptr){
			m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
			addChild(m_pTexture);
		}
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTIONW__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

float CPlanet::noise(int x, int y) {
	int n = x + y * 57;
	n = (n << 13) ^ n;
	return (1.0 - ((n * ((n * n * 15731) + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

void CPlanet::CrushShake() {

	//experiment more with these four values to get a rough or smooth effect!
	float interval = 0.01f;
	float duration = 0.5f;
	float speed = 0.1f;
	float magnitude = 5.0f;
	m_fElapsed = 0.0f;
	
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
		setPosition(m_OriginPos.x + x, m_OriginPos.y + y);
		CObjectManager::Instance()->getM_Player()->setPosition(
			CObjectManager::Instance()->getM_Player()->getOriginPos().x + x, 
			CObjectManager::Instance()->getM_Player()->getOriginPos().y + y);
		if (m_fElapsed >= duration)
		{
			m_fElapsed = 0;
			this->unschedule("Shake");

			setPosition(m_OriginPos);
			CObjectManager::Instance()->getM_Player()->setPosition(
				CObjectManager::Instance()->getM_Player()->getOriginPos() );
		}

	}, interval, CC_REPEAT_FOREVER, 0.f, "Shake");
}

void CPlanet::Execute(float delta)
{
	// 青己雀傈
	//CCLOG("青己雀傈\n");
}
