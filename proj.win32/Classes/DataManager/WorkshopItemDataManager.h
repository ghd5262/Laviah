#pragma once
#include "../Common/HSHUtility.h"
#include <vector>

struct sWORKSHOPITEM_PARAM{
	int _idx;
	int _maxLevel;
	float _timePerLevel;
	std::string _name;
	std::string _textureName;
	std::string _explain;
	std::vector<int> _costPerLevel;
};

class CWorkshopItemDataManager
{
public:
	static CWorkshopItemDataManager* Instance();

	//getter & setter
	sWORKSHOPITEM_PARAM getWorkshopItemInfoByIndex(int index) const;
	std::vector<sWORKSHOPITEM_PARAM> getWorkshopItemList(){ return m_WorkshopItemList; };

private:
	CWorkshopItemDataManager();
	virtual ~CWorkshopItemDataManager();

private:
	std::vector<sWORKSHOPITEM_PARAM> m_WorkshopItemList;
};