#include "StageStates.h"
#include "../../GameObject/ItemManager.h"
#include "../../GameObject/Stage/StageManager.h"


//------------------------------------------------------------------------

CNormalStageState* CNormalStageState::Instance(){
	static CNormalStageState instance;

	return &instance;
}

void CNormalStageState::Enter(CStageManager* stageMng){

}

void CNormalStageState::Execute(CStageManager* stageMng, float delta){
	// 시간에 맞춰 shooter동작 

	if (eITEM_FLAG_bonustime & CItemManager::Instance()->getCurrentItem())
	{
		stageMng->getFSM()->ChangeState(CBonusTimeStageState::Instance());
	}
}

void CNormalStageState::Exit(CStageManager* stageMng){

}


//------------------------------------------------------------------------

CBonusTimeStageState* CBonusTimeStageState::Instance(){
	static CBonusTimeStageState instance;

	return &instance;
}

void CBonusTimeStageState::Enter(CStageManager* stageMng){
	// bonus 패턴중 하나 실행
	
}

void CBonusTimeStageState::Execute(CStageManager* stageMng, float delta){
	if (!(eITEM_FLAG_bonustime & CItemManager::Instance()->getCurrentItem()))
	{
		stageMng->getFSM()->ChangeState(CNormalStageState::Instance());
	}
}

void CBonusTimeStageState::Exit(CStageManager* stageMng){

}


//------------------------------------------------------------------------

CCrazyStageState* CCrazyStageState::Instance(){
	static CCrazyStageState instance;

	return &instance;
}

void CCrazyStageState::Enter(CStageManager* stageMng){

}

void CCrazyStageState::Execute(CStageManager* stageMng, float delta){

}

void CCrazyStageState::Exit(CStageManager* stageMng){

}