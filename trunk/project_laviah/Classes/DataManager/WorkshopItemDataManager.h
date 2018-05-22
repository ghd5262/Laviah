#pragma once
#include "../Common/HSHUtility.h"
#include <map>

namespace WORKSHOP_DEFINE {
	static const std::string NAME    = "WORKSHOP_ITEM_NAME_%d";
	static const std::string EXPLAIN = "WORKSHOP_ITEM_EXPLAIN_%d";
    static const std::string TEXTURE = "workshop_texture_%d.png";
}

struct WORKSHOPITEM_PARAM{
	int _index;
    int _itemIndex;
    int _sortIndex;
	int _maxLevel;
	float _valuePerLevel;
	std::string _name;
	std::string _explain;
    std::string _textureName;
	std::vector<int> _costPerLevel;
    
    WORKSHOPITEM_PARAM()
    : _index(-1)
    , _itemIndex(0)
    , _sortIndex(0)
    , _maxLevel(0)
    , _valuePerLevel(0.f)
    , _name("")
    , _explain(""){
        _costPerLevel.clear();
    }
};

typedef std::map<int, const WORKSHOPITEM_PARAM*> ITEM_LIST;
class CWorkshopItemDataManager
{
public:
	static CWorkshopItemDataManager* Instance();

	//getter & setter
	const WORKSHOPITEM_PARAM* getItemDataByIndex(int index) const;
	ITEM_LIST getItemList();
    int getSkillIndexByItemIndex(int itemIndex);
private:
	CWorkshopItemDataManager();
	virtual ~CWorkshopItemDataManager();

private:
	ITEM_LIST m_WorkshopItemList;
};
