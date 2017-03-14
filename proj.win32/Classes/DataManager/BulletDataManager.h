#pragma once
#include "../GameObject/Bullet/BulletHeaders.h"
#include <map>

class CBulletDataManager
{
    typedef std::map<const char, const sBULLET_PARAM*> BULLET_LIST;
public:
	static CBulletDataManager* Instance();
	void AddBulletData(const sBULLET_PARAM& data);

	//getter & setter
	const sBULLET_PARAM* getBulletInfo(const char key) const;

private:
	CBulletDataManager();
	virtual ~CBulletDataManager();

private:
	BULLET_LIST m_BulletDataList;
};

