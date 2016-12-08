#include "PlayerStates.h"
#include "../../GameObject/Player.h"
#include "../../GameObject/ItemManager.h"
#include "../../GameObject/ObjectManager.h"

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
    player->schedule([=](float delta){
        CObjectManager::Instance()->RotateAccelerationUpdate(0.1f);
    }, 0.1f, 5, 0.f, "AccelerationUP");
    
    // change player to GiantMode
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
    player->schedule([=](float delta){
        CObjectManager::Instance()->RotateAccelerationUpdate(-0.1f);
    }, 0.1f, 5, 0.f, "AccelerationDOWN");
    
    // change player to NormalMode
	player->NormalMode();
}
