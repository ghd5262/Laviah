#include "PatternShooter.h"
#include "../Bullet/BulletHeaders.h"
#include "../../DataManager/BulletPatternDataManager.h"
#include "../../DataManager/BulletDataManager.h"
#include "../../Scene/GameScene.h"
#include "../ObjectManager.h"
#include "../Planet.h"

using namespace cocos2d;

CPatternShooter::CPatternShooter(sSHOOTER_PARAM param, float distance)
: CShooterLegacy(param, distance)
, m_PatternHeightMax(0)
, m_PatternCurrentHeight(0)
, m_Angle(0.f)
{}

CPatternShooter* CPatternShooter::create(sSHOOTER_PARAM param, float distance/* = -1*/)
{
	CPatternShooter* pRet = (CPatternShooter*)new(std::nothrow)
		CPatternShooter(param, distance);
	if (pRet && pRet->init())
	{
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool CPatternShooter::init()
{
    if(!CGameObject::init()) return false;
    
//    m_PatternParam = CBulletPatternDataManager::Instance()->getPatternInfo(m_ShooterParam._PatternName);
    m_PatternHeightMax = m_PatternParam._height;
    this->setRandomAngleIfRandom();
    
    m_Angle = m_ShooterParam._fAngle + 18.f;
    m_fIntervalTimer = m_ShooterParam._fInterval;
    
    setRotation(m_Angle);
    
    return true;
}

void CPatternShooter::shootOnceByHeight(int height)
{
    auto rotation = getRotation();
    auto anglePlus = m_Angle - rotation;
    
    for (int j = 0; j < m_PatternParam._width; j++){
        
        char p = *(m_PatternParam._pattern + (m_PatternParam._width * height) + j);
        if (p != ' ') {
            
            sBULLET_PARAM bulletParam = CBulletDataManager::Instance()->getBulletInfo(p);
            
            float initDistance = 0.f;
            
            // 0보다 작으면(default -1) bullet에 설정된 distance로 사용한다.
            if (m_fDistance < 0.0f)
                initDistance = bulletParam._fDistance;
            else
                initDistance = m_fDistance;
            
            
            bulletParam._fDistance = initDistance + (height * m_PatternParam._heightDistance + 80);
            bulletParam._isFly = m_ShooterParam._isFly;
            
            bulletParam._fDistance = 500;
            
            float bulletAngle = ((j * -m_PatternParam._widthAngleDistance) + m_ShooterParam._fAngle + 18.f) + anglePlus;
//            float bulletAngle = ((j * -m_PatternParam._widthAngleDistance) + anglePlus);
            
            CBullet* test = nullptr;
            if (p >= '1' && p <= '3') //일반 bullet
                test = CNormalBullet::create(bulletParam, bulletAngle);
            else if (p >= '4' && p <= '5')//미사일과 조준미사일
                test = CNormalMissile::create(bulletParam, bulletAngle);
            else if (p == '6')
                test = CStickBullet::create(bulletParam, bulletAngle);
            else if (p >= 'A' && p <= 'G')//아이템
                test = CPlayItem::create(bulletParam, bulletAngle);
            else if (p >= 'P' && p <= 'T')//코인
                test = CPlayCoin::create(bulletParam, bulletAngle);
            else if (p >= 'U' && p <= 'Y')//별
                test = CPlayStar::create(bulletParam, bulletAngle);
            else if (p == 'Z')//보너스 아이템
                test = CBonusLetter::create(bulletParam, bulletAngle);
            else if (p == 'z')//랜덤 아이템
            {
                bulletParam._itemType = (eITEM_TYPE)random<int>(eITEM_TYPE_health, eITEM_TYPE_MAX - 2);
                test = CPlayItem::create(bulletParam, bulletAngle);
            }
            test->setBulletSpeed(m_ShooterParam._fSpeed);
            CGameScene::getGridWorld()->addChild(test);
#if(!USE_MEMORY_POOLING)
            CObjectManager::Instance()->AddBullet(test);
#endif
        }
    }
}

void CPatternShooter::Execute(float delta) {
    if (m_isShooterPause)
        return;
    
    m_fIntervalTimer += delta;
    m_fTime += delta;
    
    if (m_fIntervalTimer < m_ShooterParam._fInterval)
        return;
    
    this->shootOnceByHeight(m_PatternCurrentHeight++);
    m_fIntervalTimer = 0.f;
    
    if(m_PatternCurrentHeight >= m_PatternHeightMax)
    {
        m_PatternCurrentHeight = 0;
        
        this->setRandomAngleIfRandom();
        
        if(m_ShooterParam._isOneShoot || m_fTime >= m_ShooterParam._fEndTime)
        {
            this->ReturnToMemoryBlock();
        }
    }
}