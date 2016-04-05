#include "ShooterDataManager.h"
#include "../Common/HSHUtility.h"
#include "../Scene/GameScene.h"
#include "../GameObject/Shooter/ShooterHeaders.h"
#include "../../cocos2d/external/json/rapidjson.h"
#include "../../cocos2d/external/json/document.h"
#include "../../cocos2d/external/json/filestream.h"


CShooterListDataManager::CShooterListDataManager()
{
	rapidjson::Document shooterListDocument;

	// shooterList.json 파일 읽음
	std::string strShooterList = CCFileUtils::sharedFileUtils()->fullPathForFilename("json/shooterList.json");
	ssize_t bufferSize = 0;
	unsigned char* json = CCFileUtils::sharedFileUtils()->getFileData(strShooterList.c_str(), "rb", &bufferSize);
	std::string clearData((const char*)json);
	size_t pos = clearData.rfind("}");
	clearData = clearData.substr(0, pos + 1);

	// shooterList.json log출력
	CCLOG("PatternList JSON : \n %s\n", clearData.c_str());
	if (shooterListDocument.Parse<0>(clearData.c_str()).HasParseError()) {
		CCLOG("Err..%s", shooterListDocument.Parse<0>(clearData.c_str()).GetParseError());
		CCASSERT(false, "Err..%s", shooterListDocument.Parse<0>(clearData.c_str()).GetParseError());
	}

	// shooterName는 배열이다.
	const rapidjson::Value& shooterList = shooterListDocument["shooterName"];

	// shooterName전체를 size만큼 순회하면서 각각의 shooter을 리스트에 삽입
	for (rapidjson::SizeType shooterCount = 0; shooterCount < shooterList.Size(); shooterCount++)
	{
		std::string shooterName = shooterList[shooterCount].GetString();
		
		if ("random_Shooter" == shooterName)
		{
			AddShooter(shooterName, CRandomShooter::create(sSHOOTER_PARAM("", 0.f, 0.f, 0.f, 0.f, 0.f)));
		}
		else if ("pattern_Shooter" == shooterName)
		{
			AddShooter(shooterName, CPatternShooter::create(sSHOOTER_PARAM("", 0.f, 0.f, 0.f, 0.f, 0.f)));
		}
	}


}

CShooterListDataManager::~CShooterListDataManager()
{
}

CShooterListDataManager* CShooterListDataManager::Instance()
{
	static CShooterListDataManager instance;
	return &instance;
}

CShooter* CShooterListDataManager::getShooterInfo(std::string shooterName) const
{
	if (m_ShooterList.find(shooterName) == m_ShooterList.end())
		CCASSERT(false, "SHOOTER KEY IS WRONG");
	return m_ShooterList.find(shooterName)->second;
}

bool CShooterListDataManager::AddShooter(std::string shooterName, CShooter* shooter){
	bool addSuccess = m_ShooterList.emplace(std::pair<std::string, CShooter*>(shooterName, shooter)).second;
	CGameScene::getGameScene()->addChild(shooter);
	shooter->setAlive(false);
	if (!addSuccess)
		CCASSERT(addSuccess, "SHOOTER KEY WAS DUPLICATED");
	return addSuccess;
}