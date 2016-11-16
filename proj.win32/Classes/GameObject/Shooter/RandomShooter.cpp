#include "RandomShooter.h"
#include "../Bullet/BulletHeaders.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/BulletDataManager.h"
#include "../ObjectManager.h"

using namespace cocos2d;

CRandomShooter::CRandomShooter(sSHOOTER_PARAM param, float distance)
	: CShooterLegacy(param, distance)
{}

CRandomShooter* CRandomShooter::create(sSHOOTER_PARAM param, float distance/* = -1.f */)				// Bullet 생성 간격
{
	CRandomShooter* pRet = (CRandomShooter*)new(std::nothrow)
		CRandomShooter(param, distance);
	if (pRet)
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

void CRandomShooter::ShootOnce()
{
	m_fIntervalRandom = random<float>
		(m_ShooterParam._fInterval - (m_ShooterParam._fInterval * 0.2f), 
		m_ShooterParam._fInterval + (m_ShooterParam._fInterval * 0.2f));

	m_nBulletCountAtOnceRandom = random<int>(1, m_ShooterParam._nBulletCountAtOneShoot);

    this->setRandomAngleIfRandom();

	char p = m_ShooterParam._randomShootSymbol;
	sBULLET_PARAM bulletParam = CBulletDataManager::Instance()->getBulletInfo(p);
	bulletParam._isFly = m_ShooterParam._isFly;

	CBullet* test = nullptr;

	if (p >= '1' && p <= '3') //일반 bullet
		test = CNormalBullet::create(bulletParam, m_ShooterParam._fAngle);
	else if (p >= '4' && p <= '5')//미사일과 조준미사일
		test = CNormalMissile::create(bulletParam, m_ShooterParam._fAngle);
    else if (p == '6')
		test = CStickBullet::create(bulletParam, m_ShooterParam._fAngle);
	else if (p >= 'A' && p <= 'G')//아이템
		test = CPlayItem::create(bulletParam, m_ShooterParam._fAngle);
	else if (p >= 'P' && p <= 'T')//코인
		test = CPlayCoin::create(bulletParam, m_ShooterParam._fAngle);
	else if (p >= 'U' && p <= 'Y')//별
		test = CPlayStar::create(bulletParam, m_ShooterParam._fAngle);
	else if (p == 'Z')//보너스 아이템
		test = CBonusLetter::create(bulletParam, m_ShooterParam._fAngle);
	else if (p == 'z')//랜덤 아이템
	{
        bulletParam._itemType = static_cast<eITEM_TYPE>(random<int>(eITEM_TYPE_health, eITEM_TYPE_MAX - 2));
        bulletParam._isFly = random<int>(0, 1);
		test = CPlayItem::create(bulletParam, m_ShooterParam._fAngle);
	}
    
    CGameScene::getGridWorld()->addChild(test);
    test->setBulletSpeed(m_ShooterParam._fSpeed);
#if(!USE_MEMORY_POOLING)
	CObjectManager::Instance()->AddBullet(test);
#endif
}

void CRandomShooter::Execute(float delta) {
	return;
	if (m_isShooterPause)
		return;

	m_fIntervalTimer += delta;
	m_fTime += delta;
	if (m_fIntervalTimer >= m_fIntervalRandom)
	{
		for (int count = 0; count < m_nBulletCountAtOnceRandom; count++)
		{
			ShootOnce();
			m_fIntervalTimer = 0.f;
		}
	}
	if (m_ShooterParam._isOneShoot || m_fTime >= m_ShooterParam._fEndTime)
	{
		ReturnToMemoryBlock();
	}
}

