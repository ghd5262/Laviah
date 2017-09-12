#include "PlayCoin.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../../MyUI/ScoreUI.h"
#include "../../MyUI/ComboScore.h"
#include "../../Particle/Particles.h"
#include "../../Scene/GameScene.h"

using namespace cocos2d;

CPlayCoin::CPlayCoin(){}

CPlayCoin* CPlayCoin::create()
{
	CPlayCoin* pRet = (CPlayCoin*)new(std::nothrow)CPlayCoin();

	if (pRet && pRet->init())
	{
#if(!USE_MEMORY_POOLING)
        pRet->autorelease();
#endif
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool CPlayCoin::init()
{
    if (!CBullet::init()) return false;
    
    this->setItemEffect(eITEM_FLAG_magnet);
    this->setColor(COLOR::GOLD);
    return true;
}

void CPlayCoin::CollisionWithPlayer()
{
    // create action
    auto currentPos   = CGameScene::getZoomLayer()->convertToWorldSpace(this->getPosition());
    auto sprite       = Sprite::createWithSpriteFrameName(this->getInfo()._spriteName);
    sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprite->setPosition(currentPos);
    sprite->setColor(COLOR::GOLD);
    sprite->setScale(CGameScene::getZoomLayer()->getScale());
    sprite->setRotation(-90);
    CGameScene::getGameScene()->addChild(sprite, ZORDER::BULLET);
    
    auto upActin      = MoveTo::create(0.6f, Vec2(sprite->getPositionX(), sprite->getPositionY() + 150.f));
    auto sineAction   = EaseExponentialOut::create(upActin);
    auto delayAction  = DelayTime::create(0.5f);
    
    auto seqAction    = Sequence::create(sineAction, delayAction,
                                         CallFunc::create([=](){
        auto layerSize   = Size(1080, 1920);
        // create bezier action config
        auto targetPos   = Vec2(layerSize.width * 0.065f, layerSize.height * 0.925f);
        auto length      = Vec2(targetPos - sprite->getPosition()).length();
        auto cPos1       = Vec2(sprite->getPosition().x - (length * 0.45f),
                                sprite->getPosition().y - 200.f);
        auto cPos2       = Vec2(targetPos.x, targetPos.y - (length * 0.3f));
        auto time        = 0.7f;
        auto scale       = 3.f;
        
        // create bezier action
        ccBezierConfig bezier;
        bezier.controlPoint_1 = Vec2(cPos1);
        bezier.controlPoint_2 = Vec2(cPos2);
        bezier.endPosition    = Vec2(targetPos);
        
        auto bezierAction = BezierTo::create(time, bezier);
        auto exponential  = EaseSineIn::create(bezierAction);
        auto scaleAction  = ScaleTo::create(0.4f, scale);
        auto fadeAction   = FadeTo::create(0.4f, 1);
        auto spawn        = Spawn::create(scaleAction, fadeAction, nullptr);
        auto removeSelf   = RemoveSelf::create();
        sprite->runAction(Sequence::create(exponential, spawn, removeSelf, nullptr));
        
    }), nullptr);
    
    sprite->runAction(seqAction);
    
    
//	CAudioManager::Instance()->PlayEffectSound("sounds/Star_2.mp3", false);
//    this->R_UpAndBezier();
//    auto value = CItemManager::Instance()->getValueOfCoin((eCOIN_TYPE)(this->getSymbol() - 'U' + 1));
    this->ReturnToMemoryBlock();
    CComboScore::Instance()->AddCombo();
    GVALUE->COIN_SCORE += 10;
	GVALUE->COIN_COUNT += 1;
    
    if(eITEM_FLAG_coin & CItemManager::Instance()->getCurrentItem())
        GVALUE->COIN_BARRIER_COUNT += 1;
}
