#pragma once
#include "../Common/HSHUtility.h"
#include <vector>

enum USERDATA_PARAM_WORKSHOP{
    WORKSHOP_ITEM_INDEX = 0,
    WORKSHOP_ITEM_LEVEL = 1,
};

namespace WORKSHOP_DEFINE {
	static const std::string NAME = "WORKSHOP_ITEM_NAME_%d";
	static const std::string EXPLAIN = "WORKSHOP_ITEM_EXPLAIN_%d";
}

struct sWORKSHOPITEM_PARAM{
	int _idx;
	int _maxLevel;
	float _valuePerLevel;
	bool _isSelling;
	std::string _name;
	std::string _textureName;
	std::string _explain;
    std::string _userDataKey;
	std::vector<int> _costPerLevel;
};

class CWorkshopItemDataManager
{
public:
	static CWorkshopItemDataManager* Instance();

	//getter & setter
	sWORKSHOPITEM_PARAM getWorkshopItemInfoByKey(std::string key) const;
	sWORKSHOPITEM_PARAM getWorkshopItemInfoByIndex(int index) const;
	std::vector<sWORKSHOPITEM_PARAM> getWorkshopItemList(){ return m_WorkshopItemList; };
    std::vector<sWORKSHOPITEM_PARAM> getSellingWorkshopItemList(){
        std::vector<sWORKSHOPITEM_PARAM> sellingItemList;
        for(auto item : m_WorkshopItemList)
        {
            if(item._isSelling)
            {
                sellingItemList.emplace_back(item);
            }
        }
        return sellingItemList;
    }

private:
	CWorkshopItemDataManager();
	virtual ~CWorkshopItemDataManager();

private:
	std::vector<sWORKSHOPITEM_PARAM> m_WorkshopItemList;
};