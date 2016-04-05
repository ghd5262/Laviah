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
//		MakeString("playItem_%d.png", itemNum),							//이미지 이름
//		20.f,															//충돌 범위
//		shotAngle,														//초기 각도
//		param._fSpeed,													//속도
//		itemNum,														//item type
//		param._isFly));													//fly Item 여부
//}
////----------------------------------------------------