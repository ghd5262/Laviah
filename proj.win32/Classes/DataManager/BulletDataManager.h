#pragma once
#include "../GameObject/Bullet/BulletHeaders.h"
#include <map>

class CBulletDataManager
{
public:
	static CBulletDataManager* Instance();
	bool AddBulletData(const char bulletSymbol, sBULLET_PARAM bullet);

	//getter & setter
	sBULLET_PARAM getBulletInfo(const char bulletSymbol) const;

private:
	CBulletDataManager();
	virtual ~CBulletDataManager();

private:
	std::map<const char, sBULLET_PARAM> m_BulletDataList;
};

