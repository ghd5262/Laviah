#include "NormalMissile.h"
#include "PlayCoin.h"
#include "PlayStar.h"
#include "TargetMark.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../../Scene/GameScene.h"

CNormalMissile::CNormalMissile(
	std::string textureName,	    //bullet �̹���
	float boundingRadius,		    //bullet �浹 ����
	float angle,				    //bullet �ʱ� ���� 
	float speed,				    //bullet �ʱ� �ӵ�
	bool isAiming/* = false*/)		//���ع̻������� ����				
	: CBullet(textureName,
	boundingRadius,
	angle,
	speed)
	, m_bIsAimingMissile(isAiming)
{}

CNormalMissile* CNormalMissile::create(
	std::string textureName,		//bullet �̹���
	float boundingRadius,			//bullet �浹 ����
	float angle,					//bullet �ʱ� ���� 
	float speed,					//bullet �ʱ� �ӵ�
	bool isAiming/* = false*/)		//���ع̻������� ����
{
	CNormalMissile* pRet = 
		(CNormalMissile*)new(std::nothrow)CNormalMissile(
		textureName, boundingRadius, angle, speed, isAiming);

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

bool CNormalMissile::init()
{
	if (!initVariable())
		return false;
	return true;
}

bool CNormalMissile::initVariable()
{
	try{
		setItemEffect(eITEM_FLAG_giant | eITEM_FLAG_coin | eITEM_FLAG_star | eITEM_FLAG_shield);

		m_ScreenRect = Rect(0, 0, 720, 1280);
		setPositionX((cos(CC_DEGREES_TO_RADIANS(m_fAngle)) * 2500.f) + m_pPlanet->getPosition().x);
		setPositionY((sin(CC_DEGREES_TO_RADIANS(m_fAngle)) * 2500.f) + m_pPlanet->getPosition().y);
		setRotation(-m_fAngle);

		m_pTexture = Sprite::create(m_TextureName);
		m_pTexture->setAnchorPoint(Vec2(0.2f, 0.5f));
		addChild(m_pTexture);
	
		CGameScene::getGameScene()->addChild(CTargetMark::create(
			MakeString("missile_target_%d.png", m_bIsAimingMissile + 1),//�̹��� �̸�
			0.f,												//�浹 ����
			m_fAngle,											//�ʱ� ����
			0.f,												//�ӵ�
			this,												//������
			m_bIsAimingMissile), 100);
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTIONW__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}


void CNormalMissile::Rotation(int dir)
{
	// aimingMissile�� ��� ȭ��ȿ� ������ ������ ȸ���Ѵ�.
	if (true == m_bIsAimingMissile){
		if (!m_ScreenRect.containsPoint(getPosition()))
		{
			return;
		}
	}

	CBullet::Rotation(dir);
}

void CNormalMissile::Execute(float delta)
{
	getFSM()->Execute(delta);
}

void CNormalMissile::CollisionWithPlanet()
{
	if (true == m_bIsAimingMissile){
		CObjectManager::Instance()->getM_Planet()->CrushShake(
			0.01f, 0.5f, 0.1f, 5.0f);
		AudioEngine::play2d("sounds/explosion_2.mp3", false);
	}
	else {
		CObjectManager::Instance()->getM_Planet()->CrushShake(
			0.01f, 0.3f, 0.1f, 3.0f);
		AudioEngine::play2d("sounds/explosion_1.mp3", false);
	}

	ReturnToMemoryBlock();
}

void CNormalMissile::CollisionWithPlayer()
{
	if (CItemManager::Instance()->getCurrentItem() & eITEM_FLAG_giant){
		AudioEngine::play2d("sounds/explosion_2.mp3", false);
		R_BezierWithRotation(Vec2(920, 1580), Vec2(350, 900), Vec2(450, 1200), 0.5f);
	}
	else{
		if (true == m_bIsAimingMissile){
			CObjectManager::Instance()->getM_Planet()->CrushShake(
				0.01f, 0.5f, 0.1f, 5.0f);
			AudioEngine::play2d("sounds/explosion_2.mp3", false);
		}
		else {
			CObjectManager::Instance()->getM_Planet()->CrushShake(
				0.01f, 0.3f, 0.1f, 3.0f);
			AudioEngine::play2d("sounds/explosion_1.mp3", false);
		}
		ReturnToMemoryBlock();
	}
}

void CNormalMissile::ChangeToCoin()
{
	eCOIN_TYPE coinType;
	if (true == m_bIsAimingMissile){
		coinType = eCOIN_TYPE_bigGold;
	}
	else{
		coinType = eCOIN_TYPE_bigSilver;
	}
	//CPlayCoin* coin = CPlayCoin::create(coinType, 50.f, m_fAngle, m_fBulletSpeed, true, getPosition());
	//CGameScene::getGameScene()->addChild(coin);
	//CGameScene::getGameScene()->addChild(CTargetMark::create(
	//	MakeString("missile_target_%d.png", 1 + (coinType - eCOIN_TYPE_bigSilver)),	//�̹��� �̸�
	//	0.f,												//�浹 ����
	//	m_fAngle,											//�ʱ� ����
	//	0.f,												//�ӵ�
	//	coin), 100);												//������
	ReturnToMemoryBlock();
}

void CNormalMissile::ChangeToStar()
{
	eSTAR_TYPE starType;
	if (true == m_bIsAimingMissile){
		starType = eSTAR_TYPE_bigGold;
	}
	else{
		starType = eSTAR_TYPE_bigSilver;
	}
	//CPlayStar* star = CPlayStar::create(starType, 50.f, m_fAngle, m_fBulletSpeed, true, getPosition());
	//CGameScene::getGameScene()->addChild(star);
	//CGameScene::getGameScene()->addChild(CTargetMark::create(
	//	MakeString("missile_target_%d.png", 1 + (starType - eSTAR_TYPE_bigSilver)),	//�̹��� �̸�
	//	0.f,												//�浹 ����
	//	m_fAngle,											//�ʱ� ����
	//	0.f,												//�ӵ�
	//	star), 100);										//������
	ReturnToMemoryBlock();
}