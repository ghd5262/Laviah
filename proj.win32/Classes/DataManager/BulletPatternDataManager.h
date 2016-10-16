#pragma once
#include "../GameObject/Shooter/ShooterHeaders.h"
#include <map>

class CBulletPatternDataManager
{
public:
	static CBulletPatternDataManager* Instance();
	bool AddPattern(std::string patternName, const sPATTERN_SHOOTER_PARAM& pattern);

	//getter & setter
	sPATTERN_SHOOTER_PARAM getPatternInfo(std::string patternName) const ;
	
private:
	CBulletPatternDataManager();
	virtual ~CBulletPatternDataManager();

private:
	std::map<std::string, sPATTERN_SHOOTER_PARAM> m_PatternList;
};

