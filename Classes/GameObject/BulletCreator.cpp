#include "BulletCreator.h"
#include "Bullet/BulletHeaders.h"
#include "../Scene/GameScene.h"
#include "../GameObject/ObjectManager.h"
#include "../GameObject/ItemManager.h"
#include "../MyUI/Tutorial/TutorialManager.hpp"
#include "../DataManager/PlanetDataManager.hpp"
#include "../DataManager/UserDataManager.h"
#include "../DataManager/BulletPatternDataManager.h"
#include "../DataManager/BulletDataManager.h"

using namespace cocos2d;
using namespace BULLETCREATOR;
using namespace PLANET_DEFINE;

#define PERFORMANCETEST 0

CBulletCreator::CBulletCreator()
: m_CurrentPattern(nullptr)
, m_PlanetInfo(nullptr)
, m_BulletDataManager(CBulletDataManager::Instance())
, m_RotationAngle(0.f)
, m_CurrentHeight(0)
, m_Time(0.f)
, m_CoinTimer(0.f)
, m_LineIntervalLimit(0.f)
, m_CreateDistance(0.f)
, m_StandardDelay(0.f)
, m_Running(false)
, m_IsFlip(false)
{}

CBulletCreator::~CBulletCreator(){}

CBulletCreator* CBulletCreator::create()
{
    CBulletCreator* pRet = new(std::nothrow)CBulletCreator();
    
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

bool CBulletCreator::init()
{
    if(!Node::init()) return false;
    
    this->Clear();
    
    auto index = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::CHARACTER);
    m_PlanetInfo = CPlanetDataManager::Instance()->getPlanetByIndex(index);
    
    return true;
}

void CBulletCreator::Update(float delta)
{
	m_Time += delta;
	m_CoinTimer += delta;
	if (m_Time < m_LineIntervalLimit) return;
	if (m_CurrentHeight <= 0) this->Clear();
    if (m_CurrentPattern == nullptr) return;
	if (m_CoinTimer > BULLETCREATOR::COIN_CREATE_LIMIT_TIME)
	{
		BULLETCREATOR::COIN_CREATE = true;
		m_CoinTimer = 0.f;
	}
	this->createOneLine(m_CurrentPattern, --m_CurrentHeight, m_CreateDistance, m_RotationAngle, true);
	m_Time = 0.0f;
}

void CBulletCreator::Clear()
{
    m_CurrentHeight     = 0;
    //m_RotationAngle = 0.f;
    m_CreateDistance    = 0.f;
    m_StandardDelay     = 0.f;
    m_CurrentPattern    = nullptr;
    m_Running           = false;
    m_IsFlip            = false;
}

void CBulletCreator::Rotation(float speed)
{
    m_RotationAngle -= speed;
}

void CBulletCreator::setPattern(int index)
{	
	auto data = CBulletPatternDataManager::Instance()->getNormalPatternByIndex(index);
	this->setData(data);
}

void CBulletCreator::setPattern(const sBULLET_PATTERN* data)
{
	this->setData(data);
}

void CBulletCreator::setData(const sBULLET_PATTERN* data)
{
    if(m_Running) return;
    
	m_CurrentPattern    = data;
	m_CurrentHeight     = data->_height;
	m_LineIntervalLimit = BULLET_STANDARD_PADDING / BULLET_STANDARD_SPEED;
	m_Running           = true;
    m_IsFlip            = random<int>(0, 1);
    m_CreateDistance    = 2700.f;
    
    if (CTutorialManager::Instance()->getIsRunning()) {
        m_IsFlip         = false;
        m_CreateDistance = 1700.f;
    }
    
    m_StandardDelay  = (m_CreateDistance - PLANET_DEFINE::BOUNDING_RADIUS) / BULLET_STANDARD_SPEED;
}

void CBulletCreator::createOneLine(const sBULLET_PATTERN* data,
                                   int currentHeight,
                                   float distance,
								   float angle,
								   bool isDelay)
{
    auto line = this->getOneLineOfPattern(data->_pattern, data->_width, currentHeight);
    
    if(PERFORMANCETEST) this->createOneLine("111111111111111111111111111111111111111111111111111111111111", currentHeight, distance, angle, false, 6);
    else                this->createOneLine(line, currentHeight, distance, angle, isDelay, data->_widthPadding);

}

void CBulletCreator::createOneLine(std::string line,
                                   int currentHeight,
                                   float distance,
                                   float angle,
                                   bool isDelay,
                                   float widthPadding)
{
    if(m_IsFlip) std::reverse(line.begin(), line.end());
    
    int width = int(line.length());
    for(auto symbol : line)
    {
        width--;
        if(symbol == ' ') continue;
        
        float bulletAngle = (widthPadding * width) - angle;
        bulletAngle += widthPadding / 2; // 각도 보정 (패턴이 중앙에 오도록)
        bulletAngle += (90 - ((widthPadding * line.size() - 1) / 2)); // 각도 보정
        
        this->CreateBullet(symbol, bulletAngle, distance, isDelay);
    }
}

std::string CBulletCreator::getOneLineOfPattern(const char* pattern, int width, int curHeight)
{
    std::string line = "";
    line.resize(width);
    int startIdx = (width * curHeight);
    int endIdx   = startIdx + width;
    std::copy(pattern + startIdx, pattern + endIdx, line.begin());
    return line;
}

void CBulletCreator::CreateImmediately(int index,
                                       float angle,
                                       float distance)
{
    auto data = CBulletPatternDataManager::Instance()->getMissilePatternByIndex(index);
    this->CreateImmediately(data, angle, distance);
}

void CBulletCreator::CreateImmediately(const sBULLET_PATTERN* data,
                                       float angle,
                                       float distance,
                                       eITEM_FLAG type)
{
    m_IsFlip = false;
    for (int height = data->_height - 1; height >= 0; height--)
    {
        auto distanceH = distance - (height * BULLET_STANDARD_PADDING);
        auto line      = this->getOneLineOfPattern(data->_pattern, data->_width, height);
        
        if(type == eITEM_FLAG_star)      std::replace(line.begin(), line.end(), '1', 'P');
        else if(type == eITEM_FLAG_coin) std::replace(line.begin(), line.end(), '1', 'U');

        this->createOneLine(line, height, distanceH, 90-angle, false, data->_widthPadding);
    }
}


void CBulletCreator::CreateConstellation(const sBULLET_PATTERN* data)
{
	for (int height = 0; height < data->_height; height++)
    {
        for (int width = 0; width < data->_width; width++)
        {
            int index = (data->_width * height) + width;
            auto symbol = data->_pattern[index];
			
			if (symbol == ' ') continue;

            auto bullet = CBulletCreator::CreateBullet(symbol, width * data->_widthPadding, 0, false);
			auto visible = Director::getInstance()->getVisibleSize();
			auto pos = CBullet::getSquarePosition(width * data->_widthPadding, visible.height - (height * BULLET_STANDARD_PADDING));
            bullet->setPosition(pos);
            bullet->setLocalZOrder(ZORDER::PLAYER);
        }
    }
}

float CBulletCreator::getCalculatedDelayTime(float speed)
{
    auto standardDelay  = this->getStandardDelay();
    auto createDistance = this->getCreateDistance();
    return (standardDelay - ((createDistance - BOUNDING_RADIUS) / speed));
}

CBullet* CBulletCreator::CreateBullet(char symbol, float angle, float distance, bool isDelay/* = true*/)
{
    CBullet* bullet = nullptr;

    // bullet create
	if (BULLETCREATOR::COIN_CREATE && (symbol >= 'P' && symbol <= 'T')) {
        if (!CTutorialManager::Instance()->getIsRunning()){
            symbol = (cocos2d::random<int>(1, 10) < 5) ? 'U' : symbol;
            symbol = (cocos2d::random<int>(1, 10) < 3) ? 'z' : symbol;
        }
        BULLETCREATOR::COIN_CREATE = false;
	}
    if      (symbol == 'z')                     symbol = cocos2d::random<int>('B', 'F');
    if      (symbol >= '1' && symbol <= '3')    bullet = CNormalBullet::create();
    else if (symbol >= '4' && symbol <= '5')    bullet = CNormalMissile::create();
    else if (symbol == '6')                     bullet = CStickBullet::create();
    else if (symbol == '7')                     bullet = CStandBullet::create();
    else if (symbol == '8')                     bullet = CRivalBullet::create();
    else if (symbol >= 'A' && symbol <= 'F')    bullet = CPlayItem::create();
	else if (symbol >= 'P' && symbol <= 'T')    bullet = CPlayStar::create();
	else if (symbol >= 'U' && symbol <= 'Y')    bullet = CPlayCoin::create();
//    else if (symbol == 'Z')                     bullet = CBonusLetter::create();
    else                                        return nullptr;
    
    auto data = *(CBulletDataManager::Instance()->getBulletInfo(symbol));
    data._distance = distance;
    data._angle = angle;
    data._isFly = true;
    if(PERFORMANCETEST)
        data._power = 0;
    
    auto creator = CObjectManager::Instance()->getBulletCreator();
    if (isDelay && data._speed > BULLET_STANDARD_SPEED){
        data._delayTime = creator->getCalculatedDelayTime(data._speed);
    }
    creator->setBulletDataByUserData(data, symbol);

    bullet
    ->setBulletInfo(data)
    ->build();
    
    bullet->setVisible(!isDelay);
    
    CGameScene::getZoomLayer()->addChild(bullet, ZORDER::BULLET);
    
#if(!USE_MEMORY_POOLING)
    CObjectManager::Instance()->AddBullet(bullet);
#endif
    
    return bullet;
}

void CBulletCreator::setBulletDataByUserData(sBULLET_PARAM& data, char symbol)
{
    std::string name = "hello.png";
    
    switch (symbol) {
        case '1':  name = m_PlanetInfo->_normalBulletTexture;   break;
        case '4':  name = m_PlanetInfo->_normalMissileTexture;  break;
        case '5':  name = m_PlanetInfo->_normalMissileTexture;  break;
        case '6':  name = m_PlanetInfo->_stickBulletTexture;    break;
        case '7':  name = m_PlanetInfo->_standBulletTexture;    break;
        case '8':  name = "rivalBullet_3.png";                           break;

        case 'A':  name = "playItem_1.png";                     break;
        case 'B':  name = "playItem_2.png";                     break;
        case 'C':  name = "playItem_3.png";                     break;
        case 'D':  name = "playItem_4.png";                     break;
        case 'E':  name = "playItem_5.png";                     break;
        case 'F':  name = "playItem_6.png";                     break;
        case 'G':  name = "playItem_7.png";                     break;
            
        case 'P':  name = "star_1.png";                         break;
        case 'Q':  name = "star_2.png";                         break;
        case 'R':  name = "star_3.png";                         break;
        case 'S':  name = "star_4.png";                         break;
        case 'T':  name = "star_5.png";                         break;
            
        case 'U':  name = "coin_1.png";                         break;
        case 'V':  name = "coin_2.png";                         break;
        case 'W':  name = "coin_3.png";                         break;
        case 'X':  name = "coin_4.png";                         break;
        case 'Y':  name = "coin_5.png";                         break;
            
        case 'Z':  name = "bonusLetter_0.png";                  break;
        default: break;
    }
    
    data._spriteName = name;
    data._particleName = name;
}
