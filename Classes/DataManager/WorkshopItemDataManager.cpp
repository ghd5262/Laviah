#include "WorkshopItemDataManager.h"
#include "DataManagerUtils.h"
#include "UserDataManager.h"
#include "../Common/HSHUtility.h"
#include "../json/json.h"

using namespace cocos2d;

CWorkshopItemDataManager::CWorkshopItemDataManager()
{
	Json::Value root;
	Json::Reader reader;

	std::string strData = FileUtils::getInstance()->fullPathForFilename("workshopItemList.json");
	std::string clearData = FileUtils::getInstance()->getStringFromFile(strData);
	size_t pos = clearData.rfind("}");
	clearData = clearData.substr(0, pos + 1);

	bool parsingSuccessful = reader.parse(clearData, root);
	if (!parsingSuccessful)
	{
		CCASSERT(false, MakeString("parser failed : \n %s", clearData.c_str()).c_str());
		return;
	}
	CCLOG("workshopItemList JSON : \n %s\n", clearData.c_str());

	const Json::Value itemArray = root["workshopitems"];
	for (unsigned int itemCount = 0; itemCount < itemArray.size(); ++itemCount)
	{
		const Json::Value valueItem = itemArray[itemCount];

		auto param = new WORKSHOPITEM_PARAM();
		
		param->_idx				= valueItem["idx"].asInt();
		param->_textureName		= valueItem["textureName"].asString();
		param->_maxLevel		= valueItem["max_level"].asInt();
		param->_valuePerLevel	= valueItem["value_per_level"].asDouble();
		param->_isSelling		= valueItem["selling"].asBool();

		param->_name			= StringUtils::format(WORKSHOP_DEFINE::NAME.c_str(), param->_idx);
		param->_explain			= StringUtils::format(WORKSHOP_DEFINE::EXPLAIN.c_str(), param->_idx);

		const Json::Value costPerLevelArray = valueItem["cost_per_level"];

		int i = 0;
		for (auto costIdx : costPerLevelArray)
		{
			CCLOG("itme name : %s level : %d : cost : %d",
                  TRANSLATE(param->_name).c_str(), i++, costIdx.asInt());
			param->_costPerLevel.emplace_back(costIdx.asInt());
		}

        m_WorkshopItemList.emplace(std::pair<int, WORKSHOPITEM_PARAM*>(param->_idx, param));
	}
}

CWorkshopItemDataManager::~CWorkshopItemDataManager()
{
    DATA_MANAGER_UTILS::mapDeleteAndClean(m_WorkshopItemList);
}

CWorkshopItemDataManager* CWorkshopItemDataManager::Instance()
{
	static CWorkshopItemDataManager instance;
	return &instance;
}

const WORKSHOPITEM_PARAM* CWorkshopItemDataManager::getItemDataByIndex(int index) const
{
    auto data = m_WorkshopItemList.find(index);
    if(data == m_WorkshopItemList.end()) {
        CCLOG("Wrong item index : %d", index);
        CCASSERT(false, "Wrong character index");
        return nullptr;
    }
    return data->second;
}

ITEM_LIST CWorkshopItemDataManager::getItemList()
{
    return m_WorkshopItemList;
}

ITEM_LIST CWorkshopItemDataManager::getSellingItemList()
{
    return DATA_MANAGER_UTILS::getMapByFunc([=](const WORKSHOPITEM_PARAM* data){
        return data->_isSelling;
    }, m_WorkshopItemList);
}

float CWorkshopItemDataManager::getCurrentItemValue(int index)
{
    auto data  = this->getItemDataByIndex(index);
    auto level = CUserDataManager::Instance()->getUserData_ParamData(USERDATA_KEY::ITEM_LEVEL,
                                                                     index,
                                                                     USERDATA_PARAM_WORKSHOP::ITEM_LEVEL,
                                                                     0);
    return data->_valuePerLevel * level;
}