#include "StageStates.h"
#include "../../GameObject/ItemManager.h"
#include "../../GameObject/ObjectManager.h"
#include "../../GameObject/Player.h"
#include "../../DataManager/StageDataManager.h"
#include "../../DataManager/BulletPatternDataManager.h"
#include "../../MyUI/UIManager.h"
#include "../../MyUI/BonusTimeUI.h"
#include "../../Scene/GameScene.h"
//------------------------------------------------------------------------

CNormalStageState* CNormalStageState::Instance(){
	static CNormalStageState instance;

	return &instance;
}

void CNormalStageState::Enter(CObjectManager* objectMng){
}

void CNormalStageState::Execute(CObjectManager* objectMng, float delta){

	objectMng->ExecuteAllObject(delta);

	// 보너스타임을 다모았다면 보너스타임 상태로 변경
	if (eITEM_FLAG_bonustime & CItemManager::Instance()->getCurrentItem())
	{
		objectMng->getFSM()->ChangeState(CBonusTimeStageState::Instance());
	}
}

void CNormalStageState::Exit(CObjectManager* objectMng){

}


//------------------------------------------------------------------------

CBonusTimeStageState* CBonusTimeStageState::Instance(){
	static CBonusTimeStageState instance;

	return &instance;
}

void CBonusTimeStageState::Enter(CObjectManager* objectMng){

	auto screwShooter = CPatternShooter::create(
		sSHOOTER_PARAM(
		MakeString("bonusTime%d_Pattern", 1)
		, 0.f
		, 0.f
		, 200.f
		, 90.f
		, 0.f
		, 1
		, 1
		, true
		, true));
}

void CBonusTimeStageState::Execute(CObjectManager* objectMng, float delta){
    
    objectMng->ExecuteAllObject(delta);
    
    CItemManager::Instance()->StartItemTimer(eITEM_TYPE_star);
    
	if (!(eITEM_FLAG_bonustime & CItemManager::Instance()->getCurrentItem()))
	{
		objectMng->getFSM()->ChangeState(CNormalStageState::Instance());
	}
}

void CBonusTimeStageState::Exit(CObjectManager* objectMng){

	// BonusTimeUI 포인터 획득
	CBonusTimeUI* bonusTimeUI
		= static_cast<CBonusTimeUI*>(CUIManager::Instance()->FindUIWithName("BonusTime"));
	bonusTimeUI->BonusTimeIsFinish();
}


//------------------------------------------------------------------------

CCrazyStageState* CCrazyStageState::Instance(){
	static CCrazyStageState instance;

	return &instance;
}

void CCrazyStageState::Enter(CObjectManager* objectMng){

}

void CCrazyStageState::Execute(CObjectManager* objectMng, float delta){

}

void CCrazyStageState::Exit(CObjectManager* objectMng){

}


//------------------------------------------------------------------------

CGameCountDownState* CGameCountDownState::Instance(){
	static CGameCountDownState instance;

	return &instance;
}

void CGameCountDownState::Enter(CObjectManager* objectMng){

}

void CGameCountDownState::Execute(CObjectManager* objectMng, float delta){

}

void CGameCountDownState::Exit(CObjectManager* objectMng){

}