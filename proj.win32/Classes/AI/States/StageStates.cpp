#include "StageStates.h"
#include "../../GameObject/ItemManager.h"
#include "../../GameObject/ObjectManager.h"
#include "../../GameObject/Player.h"
#include "../../DataManager/BulletPatternDataManager.h"
#include "../../MyUI/BonusTimeUI.h"
#include "../../Scene/GameScene.h"
//------------------------------------------------------------------------

CNormalStageState* CNormalStageState::Instance(){
	static CNormalStageState instance;
	return &instance;
}

void CNormalStageState::Enter(CObjectManager* objectMng){}

void CNormalStageState::Execute(CObjectManager* objectMng, float delta){}

void CNormalStageState::Exit(CObjectManager* objectMng){}


//------------------------------------------------------------------------

CBonusTimeStageState* CBonusTimeStageState::Instance(){
	static CBonusTimeStageState instance;
	return &instance;
}

void CBonusTimeStageState::Enter(CObjectManager* objectMng){}

void CBonusTimeStageState::Execute(CObjectManager* objectMng, float delta){}

void CBonusTimeStageState::Exit(CObjectManager* objectMng){}


//------------------------------------------------------------------------

CCrazyStageState* CCrazyStageState::Instance(){
	static CCrazyStageState instance;
	return &instance;
}

void CCrazyStageState::Enter(CObjectManager* objectMng){}

void CCrazyStageState::Execute(CObjectManager* objectMng, float delta){}

void CCrazyStageState::Exit(CObjectManager* objectMng){}


//------------------------------------------------------------------------

CGameCountDownState* CGameCountDownState::Instance(){
	static CGameCountDownState instance;
	return &instance;
}

void CGameCountDownState::Enter(CObjectManager* objectMng){}

void CGameCountDownState::Execute(CObjectManager* objectMng, float delta){}

void CGameCountDownState::Exit(CObjectManager* objectMng){}