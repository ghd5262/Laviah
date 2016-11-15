#include "AlienStates.h"
#include "../../GameObject/Alien.h"

using namespace cocos2d;

CWanderingState* CWanderingState::Instance()
{
	static CWanderingState instance;

	return &instance;
}

void CWanderingState::Enter(CAlien* alien)
{
	alien->setDirection(random<int>(0, 1) == 0 ? -1 : 1);
	alien->setCurrentAction(random<int>(0, 1));
}

void CWanderingState::Execute(CAlien* alien, float delta)
{
	alien->setActionTime(alien->getActionTime() + delta);
	if (alien->getActionTime() > 2.f)
	{
		alien->setActionTime(0.f);
		if (random<int>(0, 100) > 70 ? true : false)
			alien->getFSM()->ChangeState(CWanderingState::Instance());
	}

	switch (alien->getCurrentAction()) {
	case 0:
		alien->Walk(delta);
		break;

	case 1:
		alien->Break();
		break;
	}
}

void CWanderingState::Exit(CAlien* alien)
{
}

