#include "PlayerStates.h"
#include "../../GameObject/Player.h"
#include "../../GameObject/ItemManager.h"
#include "../../GameObject/ObjectManager.h"
#include "../../GameObject/BulletCreator.h"
#include "../../GameObject/Rocket.h"

using namespace cocos2d;

CPlayerNormal* CPlayerNormal::Instance()
{
	static CPlayerNormal instance;
	return &instance;
}

void CPlayerNormal::Enter(CPlayer* player)
{}

void CPlayerNormal::Execute(CPlayer* player, float delta)
{
	// TODO: switch문이면 하나밖에 실행 되지 않을 텐데 의도한 것인가?
	switch (player->getItemEffect() & CItemManager::Instance()->getCurrentItem())
	{
	case eITEM_FLAG_giant:
	{
		player->ChangeState(CPlayerGiant::Instance());
	}break;

	default:
	{

	}
	}
}

void CPlayerNormal::Exit(CPlayer* player)
{}


CPlayerGiant* CPlayerGiant::Instance()
{
	static CPlayerGiant instance;
	return &instance;
}

void CPlayerGiant::Enter(CPlayer* player)
{    
    // change player to GiantMode
	player->GiantMode();
}

void CPlayerGiant::Execute(CPlayer* player, float delta)
{
	if (!(CItemManager::Instance()->isCurrentItem(eITEM_FLAG_giant)))
	{
		player->ChangeState(CPlayerNormal::Instance());
	}
}

void CPlayerGiant::Exit(CPlayer* player)
{
    // change player to NormalMode
	player->NormalMode();
}


CPlayerBonusTime* CPlayerBonusTime::Instance()
{
	static CPlayerBonusTime instance;
	return &instance;
}

void CPlayerBonusTime::Enter(CPlayer* player)
{
	player->StartBonusTime();
}

void CPlayerBonusTime::Execute(CPlayer* player, float delta)
{
	auto rocket = CObjectManager::Instance()->getRocket();
	player->setPosition(rocket->getPosition());
}

void CPlayerBonusTime::Exit(CPlayer* player)
{
	player->EndBonusTime();
}
