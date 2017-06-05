#pragma once
#include "../Common/HSHUtility.h"
#include <map>

enum USERDATA_PARAM_WORKSHOP{
    ITEM_LEVEL = 0,
};

namespace WORKSHOP_DEFINE {
	static const std::string NAME = "WORKSHOP_ITEM_NAME_%d";
	static const std::string EXPLAIN = "WORKSHOP_ITEM_EXPLAIN_%d";
}

struct WORKSHOPITEM_PARAM{
	int _idx;
	int _maxLevel;
	float _valuePerLevel;
	bool _isSelling;
	std::string _name;
	std::string _textureName;
	std::string _explain;
	std::vector<int> _costPerLevel;
};

typedef std::map<int, const WORKSHOPITEM_PARAM*> ITEM_LIST;
class CWorkshopItemDataManager
{
public:
	static CWorkshopItemDataManager* Instance();

	//getter & setter
	const WORKSHOPITEM_PARAM* getItemDataByIndex(int index) const;
	ITEM_LIST getItemList();
    ITEM_LIST getSellingItemList();
    float getCurrentItemValue(int index);

private:
	CWorkshopItemDataManager();
	virtual ~CWorkshopItemDataManager();

private:
	ITEM_LIST m_WorkshopItemList;
};