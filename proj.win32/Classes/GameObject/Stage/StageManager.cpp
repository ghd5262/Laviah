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

	rapidjson::Document patternListDocument;
	std::string strPatternList = CCFileUtils::sharedFileUtils()->fullPathForFilename("json/patternList.json");
	ssize_t bufferSize = 0;
	unsigned char* json = CCFileUtils::sharedFileUtils()->getFileData(strPatternList.c_str(), "rb", &bufferSize);
	std::string clearData((const char*)json);
	size_t pos = clearData.rfind("}");
	clearData = clearData.substr(0, pos + 1);
	CCLOG("PatternList JSON : \n %s\n", clearData.c_str());
	if (patternListDocument.Parse<0>(clearData.c_str()).HasParseError()) {
		CCLOG("Err..%s", patternListDocument.Parse<0>(clearData.c_str()).GetParseError());
		CCASSERT(false, "Err..%s", patternListDocument.Parse<0>(clearData.c_str()).GetParseError());
	}

	const rapidjson::Value& patternList = patternListDocument["patterns"];

	for (rapidjson::SizeType patternCount = 0; patternCount < patternList.Size(); patternCount++)
	{
		rapidjson::Document patternDocument;
		const rapidjson::Value& valuePattern = patternList[patternCount];

		m_PatternList[patternCount]._index = patternCount;
		m_PatternList[patternCount]._patternName = valuePattern["name"].GetString();
		const rapidjson::Value& pattern = valuePattern["pattern"];

		for (rapidjson::SizeType patternHeightCount = 0; patternHeightCount < pattern.Size(); patternHeightCount++)
		{
			std::string patternStr = pattern[patternHeightCount].GetString();
			CCLOG("pattern[%d] = %s\n", patternHeightCount, patternStr.c_str());
			m_PatternList[patternCount]._height = pattern.Size();
			m_PatternList[patternCount]._width = patternStr.length();
			for (int charCount = 0; charCount < m_PatternList[patternCount]._width; charCount++)
			{
				if (patternStr.at(charCount) != ' '){
					m_PatternList[patternCount]._pattern
						[(m_PatternList[patternCount]._width * patternHeightCount) + charCount]
					= static_cast<int>(patternStr[charCount] - '0');
				}
				else{
					m_PatternList[patternCount]._pattern
						[(m_PatternList[patternCount]._width * patternHeightCount) + charCount] = 0;
				}
			}
		}
	}

	for (int i = 0; i < 10; i++)
	{
		for (int patternHeightCount = 0; patternHeightCount < m_PatternList[i]._height; patternHeightCount++)
		{
			for (int charCount = 0; charCount < m_PatternList[i]._width; charCount++)
			{
				if (m_PatternList[i]._pattern[(m_PatternList[i]._width * patternHeightCount) + charCount] != -1){
					OutputDebugStringA(MakeString("%d", m_PatternList[i]._pattern[(m_PatternList[i]._width * patternHeightCount) + charCount]).c_str());
				}
				else{
					OutputDebugStringA(" ");
				}
			}
			CCLOG("\n");
		}
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