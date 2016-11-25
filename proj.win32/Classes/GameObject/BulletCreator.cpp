#include "BulletCreator.h"
#include "Bullet/BulletHeaders.h"
#include "../DataManager/BulletDataManager.h"
#include "../Scene/GameScene.h"
#include "../DataManager/UserDataManager.h"
#include "../GameObject/ObjectManager.h"

using namespace cocos2d;
using namespace BULLETCREATOR;

CBulletCreator::CBulletCreator()
: m_CurrentPattern(nullptr)
, m_BulletDataManager(CBulletDataManager::Instance())
, m_RotationAngle(0)
, m_CurrentHeight(0)
, m_Time(0)
, m_LineIntervalLimit(0.0f)
, m_Running(false)
, m_Pause(false)
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
    
    this->clear();
    
    auto index = CUserDataManager::Instance()->getUserData_Number("USER_CUR_CHARACTER");
    m_CharacterInfo = CCharacterDataManager::Instance()->getCharacterInfoByIndex(index);
    
    return true;
}

void CBulletCreator::Update(float delta)
{
	m_Time += delta;
	if (m_Time < m_LineIntervalLimit) return;
    if(m_CurrentHeight <= 0) this->clear();
    if(m_CurrentPattern == nullptr) return;
    
	this->createOneLine(m_CurrentPattern, --m_CurrentHeight, CREATE_DISTANCE, m_RotationAngle);
	m_Time = 0.0f;
}

void CBulletCreator::setRotationAngle(float dir, float delta)
{
	m_RotationAngle -= (dir * (ROTATION_SPEED * delta));
}

void CBulletCreator::setPattern(std::string patternName)
{	
	auto data = CBulletPatternDataManager::Instance()->getDataByName(patternName);
	this->setData(data);
}

void CBulletCreator::setPattern(const sBULLET_PATTERN* data)
{
	this->setData(data);
}

void CBulletCreator::setData(const sBULLET_PATTERN* data)
{
	m_CurrentPattern = data;
	m_CurrentHeight = data->_height;
	m_LineIntervalLimit = BULLET_STANDARD_PADDING / BULLET_STANDARD_SPEED;
	m_Running = true;
}

void CBulletCreator::createOneLine(const sBULLET_PATTERN* data,
                                   int currentHeight,
                                   float distance,
								   float angle)
{
    //for(int width = 0; width < data->_width; width++)
	for (int width = data->_width-1; width >= 0; width--)
    {
		int index = (data->_width * currentHeight) + ((data->_width - 1) - width);
        auto symbol = data->_pattern[index];
        if(symbol == ' ') continue;
        
        // 각 총알의 각도
        // width번째 총알 = (padding * width) - 프레임 간 회전 정도
        float bulletAngle = (data->_widthPadding * width) - angle;
		bulletAngle += data->_widthPadding / 2;									 // 각도 보정 (패턴이 중앙에 오도록)
//		bulletAngle += (90 - ((data->_widthPadding * data->_width - 1) / 2));	 // 각도 보정 
        
		this->createBullet(symbol, bulletAngle, distance);
    }
}

void CBulletCreator::CreateImmediately(std::string patternName,
                                       float angle,
                                       float distance)
{
	auto data = CBulletPatternDataManager::Instance()->getDataByName(patternName);
	for (int height = data->_height - 1; height >= 0; height--)
	{
		auto distanceH = distance - (height * BULLET_STANDARD_PADDING);
		this->createOneLine(data, height, distanceH, 90 - angle);
	}
}

CBullet* CBulletCreator::createBullet(char symbol, float angle, float distance)
{
    CBullet* bullet = nullptr;

    // bullet create
    if      (symbol == 'z')                     symbol = cocos2d::random<int>('A', 'F');
    if      (symbol >= '1' && symbol <= '3')    bullet = CNormalBullet::create();
    else if (symbol >= '4' && symbol <= '5')    bullet = CNormalMissile::create();
    else if (symbol == '6')                     bullet = CStickBullet::create();
    else if (symbol >= 'A' && symbol <= 'F')    bullet = CPlayItem::create();
    else if (symbol >= 'P' && symbol <= 'T')    bullet = CPlayCoin::create();
    else if (symbol >= 'U' && symbol <= 'Y')    bullet = CPlayStar::create();
    else if (symbol == 'Z')                     bullet = CBonusLetter::create();
    else                                        return nullptr;
    
    auto data = *(CBulletDataManager::Instance()->getBulletInfo(symbol));
    data._distance = distance;
    data._angle = angle;
    data._isFly = true;
	if (data._speed > BULLET_STANDARD_SPEED)
        data._delayTime = (BULLET_STANDARD_DELAY - ((CREATE_DISTANCE - PLANET::BOUNDING_RADIUS) / data._speed));

    CObjectManager::Instance()->getBulletCreator()->setBulletDataByUserData(data, symbol);

    bullet
    ->setBulletInfo(data)
    ->build();
    
    CGameScene::getGridWorld()->addChild(bullet);
    
#if(!USE_MEMORY_POOLING)
    CObjectManager::Instance()->AddBullet(bullet);
#endif
    
    return bullet;
}

void CBulletCreator::setBulletDataByUserData(sBULLET_PARAM& data, char symbol)
{
    std::string name = "hello.png";
    
    if      (symbol == '1')                     name = m_CharacterInfo._normalBulletTextureName;
    else if (symbol == '4')                     name = m_CharacterInfo._normalMissileTextureName;
    else if (symbol == '5')                     name = m_CharacterInfo._aimingMissileTextureName;
    else if (symbol == '6')                     name = m_CharacterInfo._stickBulletTextureName;
    
    else if (symbol == 'A')                     name = "playItem_1.png";
    else if (symbol == 'B')                     name = "playItem_2.png";
    else if (symbol == 'C')                     name = "playItem_3.png";
    else if (symbol == 'D')                     name = "playItem_4.png";
    else if (symbol == 'E')                     name = "playItem_5.png";
    else if (symbol == 'F')                     name = "playItem_6.png";
//    else if (symbol == 'G')                     name = "playItem_7.png";
    
    else if (symbol == 'P')                     name = "star_1.png";
    else if (symbol == 'Q')                     name = "star_2.png";
    else if (symbol == 'R')                     name = "star_3.png";
    else if (symbol == 'S')                     name = "star_4.png";
    else if (symbol == 'T')                     name = "star_5.png";
    
    else if (symbol == 'U')                     name = "coin_1.png";
    else if (symbol == 'V')                     name = "coin_2.png";
    else if (symbol == 'W')                     name = "coin_3.png";
    else if (symbol == 'X')                     name = "coin_4.png";
    else if (symbol == 'Y')                     name = "coin_5.png";

    else if (symbol == 'Z')                     name = "bonusLetter_0.png";
    
    data._spriteName = name;
}

void CBulletCreator::clear()
{
    m_CurrentHeight = 0;
	//m_RotationAngle = 0.f;
    m_CurrentPattern = nullptr;
	m_Running = false;
}