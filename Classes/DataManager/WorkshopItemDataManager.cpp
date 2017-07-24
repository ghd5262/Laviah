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

	std::string strData   = FileUtils::getInstance()->fullPathForFilename("workshopItemList.json");
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

	const Json::Value itemArray = root["workshopItems"];
	for (unsigned int itemCount = 0; itemCount < itemArray.size(); ++itemCount)
	{
		const Json::Value valueItem = itemArray[itemCount];

		auto param = new WORKSHOPITEM_PARAM();
		
		param->_index         = valueItem["index"].asInt();
        param->_itemIndex     = valueItem["itemIndex"].asInt();
		param->_maxLevel      = valueItem["maxLevel"].asInt();
		param->_valuePerLevel = valueItem["valuePerLevel"].asDouble();
		param->_name          = StringUtils::format(WORKSHOP_DEFINE::NAME.c_str(),    param->_index);
		param->_explain       = StringUtils::format(WORKSHOP_DEFINE::EXPLAIN.c_str(), param->_index);
        param->_textureName   = StringUtils::format(WORKSHOP_DEFINE::TEXTURE.c_str(), param->_index);

		const Json::Value costPerLevelArray = valueItem["cost"];

		int i = 0;
		for (auto costIdx : costPerLevelArray)
		{
			CCLOG("itme name : %s level : %d : cost : %d",
                  TRANSLATE(param->_name).c_str(), i++, costIdx.asInt());
			param->_costPerLevel.emplace_back(costIdx.asInt());
		}

        m_WorkshopItemList.emplace(std::pair<int, WORKSHOPITEM_PARAM*>(param->_index, param));
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

int CWorkshopItemDataManager::getSkillIndexByItemIndex(int itemIndex)
{
    auto list = DATA_MANAGER_UTILS::getMapByFunc([=](const WORKSHOPITEM_PARAM* data){
        return (data->_itemIndex == itemIndex);
    }, m_WorkshopItemList);
    
    if(list.size() <= 0){
        CCLOG("Wrong item index : %d", itemIndex);
        return 0;
    }
    
    int index = 0;
    for(auto data : list){
        index = (data.second)->_index;
        break;
    }
    return index;
}
