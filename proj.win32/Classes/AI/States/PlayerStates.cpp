#include "PlayerStates.h"
#include "../../GameObject/Player.h"
#include "../../GameObject/ItemManager.h"
CPlayerNormal* CPlayerNormal::Instance()
{
	static CPlayerNormal instance;

	return &instance;
}

void CPlayerNormal::Enter(CPlayer* player)
{}

void CPlayerNormal::Execute(CPlayer* player, float delta)
{
	switch (player->getItemEffect() & CItemManager::Instance()->getCurrentItem())
	{
	case eITEM_FLAG_giant:
	{
		player->getFSM()->ChangeState(CPlayerGiant::Instance());
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
	player->GiantMode();
}

void CPlayerGiant::Execute(CPlayer* player, float delta)
{
	if (!(CItemManager::Instance()->getCurrentItem() & eITEM_FLAG_giant))
	{
		player->getFSM()->ChangeState(CPlayerNormal::Instance());
	}
}

void CPlayerGiant::Exit(CPlayer* player)
{
	player->NormalMode();
}
