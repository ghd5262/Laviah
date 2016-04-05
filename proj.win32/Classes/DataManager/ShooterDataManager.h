#pragma once
#include "../GameObject/Shooter/ShooterHeaders.h"
#include <map>

class CShooterListDataManager
{
public:
	static CShooterListDataManager* Instance();
	bool AddShooter(std::string shooterName, CShooter* shooter);

	//getter & setter
	CShooter* getShooterInfo(std::string shooterName) const;

private:
	CShooterListDataManager();
	virtual ~CShooterListDataManager();

private:
	std::map<std::string, CShooter*> m_ShooterList;
};

