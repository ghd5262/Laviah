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