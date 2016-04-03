#include "StageManager.h"
#include "../Shooter/ShooterHeaders.h"
#include "../../AI/States/StageStates.h"
#include "../../cocos2d/external/json/rapidjson.h"
#include "../../cocos2d/external/json/document.h"
#include "../../cocos2d/external/json/filestream.h"
#include "../../Common/HSHUtility.h"
CStageManager::CStageManager()
: m_fTime(0.0f)
{
	/*m_StageParamList.emplace_back(sPARAM("RandomShoot", 0.0f, 50.0f, 250.f, 0.0f, 0.1f, 0, 1));
	m_StageParamList.emplace_back(sPARAM("RandomMissileShoot", 10.0f, 50.0f, 600.f, 0.0f, 10.0f, 0, 2));
	m_StageParamList.emplace_back(sPARAM("AimingMissileShoot", 10.0f, 50.0f, 1200.f, 0.0f, 15.0f, 0, 1));
	m_StageParamList.emplace_back(sPARAM("ScrewShoot", 25.0f, 30.0f, 250.f, 0.0f, 0.1f, 0, 1));


	m_StageParamList.emplace_back(sPARAM("RandomShoot", 50.0f, 100.0f, 250.f, 0.0f, 0.1f, 0, 1));
	m_StageParamList.emplace_back(sPARAM("RandomMissileShoot", 50.0f, 100.0f, 600.f, 0.0f, 10.0f, 0, 3));
	m_StageParamList.emplace_back(sPARAM("AimingMissileShoot", 50.0f, 100.0f, 1200.f, 0.0f, 15.0f, 0, 1));
	m_StageParamList.emplace_back(sPARAM("ScrewShoot", 75.0f, 80.f, 250.f, 0.0f, 0.1f, 1, 1));*/


	m_FSM = new CStateMachine<CStageManager>(this);

	if (m_FSM != nullptr){
		m_FSM->ChangeState(CNormalStageState::Instance());
	}
}

CStageManager::~CStageManager()
{
}

CStageManager* CStageManager::Instance()
{
	static CStageManager instance;
	return &instance;
}

void CStageManager::Execute(float delta)
{
	m_FSM->Execute(delta);
}

//bool CStageManager::AddShooter(std::string patternName, CShooter* shooter){
//	bool addSuccess = m_ShooterList.emplace(std::pair<std::string, CShooter*>(patternName, shooter)).second;
//	if (!addSuccess)
//		CCASSERT(addSuccess, "PATTERN KEY WAS DUPLICATED");
//	return addSuccess;
//}
//
//CShooter* CStageManager::FindShooterWithName(std::string patternName) const
//{
//	auto shooter = m_ShooterList.find(patternName)->second;
//	if (shooter == nullptr)
//		CCASSERT(shooter != nullptr, "PATTERN KEY IS WRONG");
//	return shooter;
//}