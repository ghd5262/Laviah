#include "BulletCreator.h"
#include "Bullet/BulletHeaders.h"
#include "../DataManager/BulletDataManager.h"
#include "../Scene/GameScene.h"
#include "../DataManager/UserDataManager.h"
#include "../GameObject/ObjectManager.h"
#include "../GameObject/ItemManager.h"


using namespace cocos2d;
using namespace BULLETCREATOR;
using namespace PLANET_DEFINE;

CBulletCreator::CBulletCreator()
: m_CurrentPattern(nullptr)
, m_CharacterInfo(nullptr)
, m_BulletDataManager(CBulletDataManager::Instance())
, m_RotationAngle(0)
, m_CurrentHeight(0)
, m_Time(0)
, m_LineIntervalLimit(0.0f)
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
    m_CharacterInfo = CCharacterDataManager::Instance()->getCharacterByIndex(index);
    
    return true;
}

void CBulletCreator::Update(float delta)
{
	m_Time += delta;
	if (m_Time < m_LineIntervalLimit) return;
	if (m_CurrentHeight <= 0) this->Clear();
    if (m_CurrentPattern == nullptr) return;
    
	this->createOneLine(m_CurrentPattern, --m_CurrentHeight, CREATE_DISTANCE, m_RotationAngle, true);
	m_Time = 0.0f;
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
	m_CurrentPattern = data;
	m_CurrentHeight = data->_height;
	m_LineIntervalLimit = BULLET_STANDARD_PADDING / BULLET_STANDARD_SPEED;
	m_Running = true;
	m_IsFlip = random<int>(0, 1);
	if (m_IsFlip)
		CCLOG("Flip");
}

void CBulletCreator::createOneLine(const sBULLET_PATTERN* data,
                                   int currentHeight,
                                   float distance,
								   float angle,
								   bool isDelay)
{
    //for(int width = 0; width < data->_width; width++)
	for (int width = data->_width-1; width >= 0; width--)
    {
		int index = (data->_width * currentHeight) + ((data->_width - 1) - width);
        auto symbol = data->_pattern[index];
		int w = width;
        if(symbol == ' ') continue;
        
        // 각 총알의 각도
        // width번째 총알 = (padding * width) - 프레임 간 회전 정도
		if (m_IsFlip) 
			w = ((data->_width - 1) - width);

		float bulletAngle = (data->_widthPadding * w) - angle;
		bulletAngle += data->_widthPadding / 2;									 // 각도 보정 (패턴이 중앙에 오도록)
        bulletAngle += (90 - ((data->_widthPadding * data->_width - 1) / 2));	 // 각도 보정
        
		this->CreateBullet(symbol, bulletAngle, distance, isDelay);
    }
}

void CBulletCreator::CreateImmediately(int index,
                                       float angle,
                                       float distance)
{
	auto data = CBulletPatternDataManager::Instance()->getMissilePatternByIndex(index);
	m_IsFlip = false;
	for (int height = data->_height - 1; height >= 0; height--)
	{
		auto distanceH = distance - (height * BULLET_STANDARD_PADDING);
		this->createOneLine(data, height, distanceH, 90-angle, false);
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

            auto bullet = CBulletCreator::CreateBullet(symbol, width * data->_widthPadding, 0);
			auto visible = CCDirector::getInstance()->getVisibleSize();
			auto pos = CBullet::getSquarePosition(width * data->_widthPadding, visible.height - (height * BULLET_STANDARD_PADDING));
            bullet->setPosition(pos);
            bullet->setLocalZOrder(ZORDER::PLAYER);
        }
    }
}

CBullet* CBulletCreator::CreateBullet(char symbol, float angle, float distance, bool isDelay/* = true*/)
{
    CBullet* bullet = nullptr;

    // bullet create
    if      (symbol == 'z')                     symbol = cocos2d::random<int>('A', 'F');
    if      (symbol >= '1' && symbol <= '3')    bullet = CNormalBullet::create();
    else if (symbol >= '4' && symbol <= '5')    bullet = CNormalMissile::create();
    else if (symbol == '6')                     bullet = CStickBullet::create();
    else if (symbol == '7')                     bullet = CStandBullet::create();
    else if (symbol >= 'A' && symbol <= 'F')    bullet = CPlayItem::create();
	else if (symbol >= 'P' && symbol <= 'T')    bullet = CPlayStar::create();
	else if (symbol >= 'U' && symbol <= 'Y')    bullet = CPlayCoin::create();
    else if (symbol == 'Z')                     bullet = CBonusLetter::create();
    else                                        return nullptr;
    
    auto data = *(CBulletDataManager::Instance()->getBulletInfo(symbol));
    data._distance = distance;
    data._angle = angle;
    data._isFly = true;
	if (isDelay && data._speed > BULLET_STANDARD_SPEED)
        data._delayTime = (BULLET_STANDARD_DELAY - ((CREATE_DISTANCE - BOUNDING_RADIUS) / data._speed));

    CObjectManager::Instance()->getBulletCreator()->setBulletDataByUserData(data, symbol);

    bullet
    ->setBulletInfo(data)
    ->build();
    
    CGameScene::getGameScene()->addChild(bullet, ZORDER::BULLET);
    
#if(!USE_MEMORY_POOLING)
    CObjectManager::Instance()->AddBullet(bullet);
#endif
    
    return bullet;
}

void CBulletCreator::setBulletDataByUserData(sBULLET_PARAM& data, char symbol)
{
    std::string name = "hello.png";
    
    if      (symbol == '1')                     name = m_CharacterInfo->_normalBulletTextureName;
    else if (symbol == '4')                     name = m_CharacterInfo->_normalMissileTextureName;
    else if (symbol == '5')                     name = m_CharacterInfo->_aimingMissileTextureName;
    else if (symbol == '6')                     name = m_CharacterInfo->_stickBulletTextureName;
    else if (symbol == '7')                     name = m_CharacterInfo->_stickBulletTextureName;

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

void CBulletCreator::Clear()
{
    m_CurrentHeight = 0;
	//m_RotationAngle = 0.f;
    m_CurrentPattern = nullptr;
	m_Running = false;
}