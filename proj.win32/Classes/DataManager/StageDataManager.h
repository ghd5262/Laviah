#pragma once
#include "../GameObject/Shooter/ShooterHeaders.h"
#include <vector>

class CStageDataManager
{
public:
	static CStageDataManager* Instance();
	bool AddShooter(std::string shooterName, CShooter* shooter);

	//getter & setter
	sSHOOTER_PARAM getStageInfoByIndex(int index) const;
	const std::vector<sSHOOTER_PARAM>* getStageList(){ return m_StageList; };

private:
	CStageDataManager();
	virtual ~CStageDataManager();

private:
	std::vector<sSHOOTER_PARAM>* m_StageList;
};

