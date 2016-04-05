#include "StageStates.h"
#include "../../GameObject/ItemManager.h"
#include "../../GameObject/Stage/StageManager.h"
#include "../../DataManager/ShooterDataManager.h"
#include "../../DataManager/BulletPatternDataManager.h"
#include "../../MyUI/UIManager.h"
#include "../../MyUI/BonusTimeUI.h"
//------------------------------------------------------------------------

CNormalStageState* CNormalStageState::Instance(){
	static CNormalStageState instance;

	return &instance;
}

void CNormalStageState::Enter(CStageManager* stageMng){

}

void CNormalStageState::Execute(CStageManager* stageMng, float delta){
	// Ω√∞£ø° ∏¬√Á shooterµø¿€ 

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
	CShooter* patternShooter = CShooterListDataManager::Instance()->getShooterInfo("pattern_Shooter");
	
	std::string patternName = MakeString("bonusTime%d_Pattern", 1);
	patternShooter->setShooterParam(sSHOOTER_PARAM(patternName, 0.f, 0.f, 500.f, 90.f, 0.f));
	patternShooter->ShootOnce();
}

void CBonusTimeStageState::Execute(CStageManager* stageMng, float delta){
	if (!(eITEM_FLAG_bonustime & CItemManager::Instance()->getCurrentItem()))
	{
		stageMng->getFSM()->ChangeState(CNormalStageState::Instance());
	}
}

void CBonusTimeStageState::Exit(CStageManager* stageMng){

	// BonusTimeUI ∆˜¿Œ≈Õ »πµÊ
	CBonusTimeUI* bonusTimeUI
		= static_cast<CBonusTimeUI*>(CUIManager::Instance()->FindUIWithName("BonusTime"));
	bonusTimeUI->BonusTimeIsFinish();
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