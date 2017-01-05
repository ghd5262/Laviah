#include "WorkshopItemDataManager.h"
#include "../Common/HSHUtility.h"
//#include "../../cocos2d/external/json/rapidjson.h"
//#include "../../cocos2d/external/json/document.h"
//#include "../../cocos2d/external/json/filestream.h"
#include "../json/json.h"

using namespace cocos2d;

CWorkshopItemDataManager::CWorkshopItemDataManager()
{
	Json::Value root;
	Json::Reader reader;

	// patternList.json 파일 읽음
	std::string strWorkshopItemList = FileUtils::getInstance()->fullPathForFilename("workshopItemList.json");
	std::string workshopItemListClearData = FileUtils::getInstance()->getStringFromFile(strWorkshopItemList);
	size_t pos = workshopItemListClearData.rfind("}");
	workshopItemListClearData = workshopItemListClearData.substr(0, pos + 1);

	// patternList.json log출력
	bool parsingSuccessful = reader.parse(workshopItemListClearData, root);
	if (!parsingSuccessful)
	{
		CCASSERT(false, MakeString("parser failed : \n %s", workshopItemListClearData.c_str()).c_str());
		return;
	}
	CCLOG("strWorkshopItemList JSON : \n %s\n", workshopItemListClearData.c_str());


	// stage는 배열이다.
	const Json::Value itemArray = root["workshopitems"];

	for (unsigned int itemCount = 0; itemCount < itemArray.size(); ++itemCount)
	{
		const Json::Value valueItem = itemArray[itemCount];

		sWORKSHOPITEM_PARAM param;
		
		param._idx = itemCount;
		param._textureName = valueItem["textureName"].asString();
		param._name = valueItem["name"].asString();
		param._maxLevel = valueItem["max_level"].asInt();
		param._valuePerLevel = valueItem["value_per_level"].asDouble();
		param._explain = valueItem["explain"].asString();
		param._isSelling = valueItem["selling"].asBool();
        param._userDataKey = valueItem["userDataKey"].asString();

		const Json::Value costPerLevelArray = valueItem["cost_per_level"];

		int i = 0;
		for (auto costIdx : costPerLevelArray)
		{
			CCLOG("itme name : %s level : %d : cost : %d", param._name.c_str(), i++, costIdx.asInt());
			param._costPerLevel.emplace_back(costIdx.asInt());
		}

		m_WorkshopItemList.emplace_back(param);
	}
}

CWorkshopItemDataManager::~CWorkshopItemDataManager()
{
}

CWorkshopItemDataManager* CWorkshopItemDataManager::Instance()
{
	static CWorkshopItemDataManager instance;
	return &instance;
}

sWORKSHOPITEM_PARAM CWorkshopItemDataManager::getWorkshopItemInfoByIndex(int index) const
{
	if (m_WorkshopItemList.size() <= index){
		CCLOG("WARNNING - There is no item with index %d", index);
		return m_WorkshopItemList.at(0);
	}
	return m_WorkshopItemList.at(index);
}

sWORKSHOPITEM_PARAM CWorkshopItemDataManager::getWorkshopItemInfoByKey(std::string key) const
{
	for (auto item : m_WorkshopItemList)
	{
		if (item._userDataKey == key)
		{
			return item;
		}
	}

	CCLOG("There is no item : %s", key.c_str());
	CCASSERT(false, "Wrong Item Key");
}