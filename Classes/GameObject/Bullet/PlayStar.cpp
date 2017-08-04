#include "PlayStar.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../../MyUI/ScoreUI.h"
#include "../../MyUI/ComboScore.h"
#include "../../Particle/Particles.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/GradientDataManager.h"
#include <algorithm>

using namespace cocos2d;

CPlayStar::CPlayStar()
: m_OldLevel(0){}

CPlayStar* CPlayStar::create()
{
	CPlayStar* pRet = (CPlayStar*)new(std::nothrow)CPlayStar();

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

bool CPlayStar::init()
{
	if (!CBullet::init()) return false;

    this->setItemEffect(eITEM_FLAG_magnet);
    return true;
}


void CPlayStar::CollisionWithPlayer()
{
//	CAudioManager::Instance()->PlayEffectSound("sounds/Star_2.mp3", false);
	this->R_ScaleWithFadeOut(2.f, 0.5f, 0.5f);

//	auto value = CItemManager::Instance()->getValueOfStar((eSTAR_TYPE)(this->getSymbol() - 'P' + 1));
    CComboScore::Instance()->AddCombo();
	GVALUE->STAR_COUNT += 1;
    
    if(eITEM_FLAG_star & CItemManager::Instance()->getCurrentItem())
        GVALUE->STAR_BARRIER_COUNT += 1;
}

void CPlayStar::Execute(float delta)
{
    m_Time += delta;
    if (!IsTimeUP()) return;
    
    this->updateStateByCombo();
    this->setVisible(true);
    m_FSM->Execute(delta);
}

void CPlayStar::updateStateByCombo()
{
    if(m_OldLevel == GVALUE->COMBO_LEVEL) return;
    
    m_OldLevel = GVALUE->COMBO_LEVEL;
    
    auto name  = StringUtils::format("star_%d.png", ((GVALUE->COMBO_LEVEL - 1) % 5) + 1);
    this->getBulletSprite()->setSpriteFrame(name);
    this->setColor(CGradientDataManager::Instance()->getScoreColorByLevel(m_OldLevel));

//    auto scale = std::min(1.0f, (m_OldLevel + 1) / 10.f);
//    scale      = std::max(0.7f, scale);
    this->setScale(0.7f);
}
