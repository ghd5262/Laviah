#pragma once
#include "ShooterHeaders.h"
#include "../ItemManager.h"
#include "../ObjectManager.h"
#include "../Bullet/BulletHeaders.h"
#include "../../Scene/GameScene.h"


////-------------------BonusLetter Shooter-------------------
//void PlayItemShoot(sSHOOTER_PARAM param)
//{
//	float shotAngle = random<float>(0.f, 360.f);
//	int itemNum = 3;// param._ShooterType - eSHOOTER_TYPE_item_0;
//
//	CGameScene::getGameScene()->addChild(CPlayItem::create(
//		MakeString("playItem_%d.png", itemNum),							//�̹��� �̸�
//		20.f,															//�浹 ����
//		shotAngle,														//�ʱ� ����
//		param._fSpeed,													//�ӵ�
//		itemNum,														//item type
//		param._isFly));													//fly Item ����
//}
////----------------------------------------------------