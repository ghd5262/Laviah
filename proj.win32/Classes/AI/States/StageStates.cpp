#include "StageStates.h"
#include "../../GameObject/ItemManager.h"
#include "../../GameObject/ObjectManager.h"
#include "../../DataManager/StageDataManager.h"
#include "../../DataManager/BulletPatternDataManager.h"
#include "../../MyUI/UIManager.h"
#include "../../MyUI/BonusTimeUI.h"
//------------------------------------------------------------------------

CNormalStageState* CNormalStageState::Instance(){
	static CNormalStageState instance;

	return &instance;
}

void CNormalStageState::Enter(CObjectManager* objectMng){

	// All Shooter Resume
	objectMng->ShooterResume();
}

void CNormalStageState::Execute(CObjectManager* objectMng, float delta){

	// ���ʽ�Ÿ���� �ٸ�Ҵٸ� ���ʽ�Ÿ�� ���·� ����
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

	// All Shooter Pause
	objectMng->ShooterPause();

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
	if (!(eITEM_FLAG_bonustime & CItemManager::Instance()->getCurrentItem()))
	{
		objectMng->getFSM()->ChangeState(CNormalStageState::Instance());
	}
}

void CBonusTimeStageState::Exit(CObjectManager* objectMng){

	// BonusTimeUI ������ ȹ��
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