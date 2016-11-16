#include "BulletCreator.hpp"
#include "Bullet/BulletHeaders.h"
#include "../DataManager/BulletDataManager.h"
#include "../Scene/GameScene.h"

using namespace cocos2d;

CBulletCreator::CBulletCreator()
: m_CurrentPattern(nullptr)
, m_BulletDataManager(CBulletDataManager::Instance())
, m_RotationAngle(0)
, m_CurrentHeight(0)
, m_RotationSpeed(100)
, m_Running(true){}

CBulletCreator::~CBulletCreator(){}

CBulletCreator* CBulletCreator::Instance()
{
    static CBulletCreator instance;
    return &instance;
}

void CBulletCreator::Update(float delta)
{
    if(!m_Running) return;
    if(m_CurrentHeight <= 0) this->clear();
    if(m_CurrentPattern == nullptr) return;
    
    this->createOneLine(m_CurrentPattern, --m_CurrentHeight, m_CreateDistance);
}

void CBulletCreator::setRotationAngle(float dir, float delta)
{
    m_RotationAngle -= (dir * (m_RotationSpeed * delta));
}

void CBulletCreator::setPattern(const sBULLET_PATTERN* data, float distance)
{
    m_CurrentPattern = data;
    m_CurrentHeight = data->_height;
    m_CreateDistance = distance;
}

void CBulletCreator::createImmediately(const sBULLET_PATTERN* data, float angle, float distance)
{
    for(int height = data->_height; height >= 0; --height)
    {
        this->createOneLine(data, height, distance);
    }
}

void CBulletCreator::createOneLine(const sBULLET_PATTERN* data, int currentHeight, float distance)
{
    for(int width = 0; width < data->_width; width++)
    {
        int index = (data->_width * currentHeight) + width;
        auto symbol = data->_pattern[index];
        if(symbol == ' ') continue;
        
        // 각 총알의 각도
        // width번째 총알 = (padding * width) - 프레임 간 회전 정도
        float angle = (data->_widthPadding * width) - m_RotationAngle;
        angle -= 108.f; // 각도 보정

        
        auto bullet = m_BulletDataManager->getBulletInfo(symbol);
        
        this->createBullet(*bullet, angle, distance);
    }
}

CBullet* CBulletCreator::createBullet(sBULLET_PARAM data, float angle, float distance)
{
    CBullet* bullet = nullptr;
    char symbol = data._symbol;

    float speed = 300.f;

    // bullet create
    if      (symbol == 'z') /* 랜덤 아이템*/       symbol = random<char>('A', 'G');
    if      (symbol >= '1' && symbol <= '3')    bullet = CNormalBullet::create  (data, angle);
    else if (symbol >= '4' && symbol <= '5')    bullet = CNormalMissile::create (data, angle);
    else if (symbol == '6')                     bullet = CStickBullet::create   (data, angle);
    else if (symbol == '7')                     bullet = CTargetMark::create    (data, angle);
    else if (symbol >= 'A' && symbol <= 'G')    bullet = CPlayItem::create      (data, angle);
    else if (symbol >= 'P' && symbol <= 'T')    bullet = CPlayCoin::create      (data, angle);
    else if (symbol >= 'U' && symbol <= 'Y')    bullet = CPlayStar::create      (data, angle);
    else if (symbol == 'Z')                     bullet = CBonusLetter::create   (data, angle);
    
    bullet->setBulletSpeed(speed);
    CGameScene::getGridWorld()->addChild(bullet);
    
#if(!USE_MEMORY_POOLING)
    CObjectManager::Instance()->AddBullet(bullet);
#endif
    
    return bullet;
}

void CBulletCreator::clear()
{
    m_CurrentHeight = 0;
    m_CreateDistance = 0.f;
    m_CurrentPattern = nullptr;
}