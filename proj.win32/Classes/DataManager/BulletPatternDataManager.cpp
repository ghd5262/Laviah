#include "BulletPatternDataManager.h"
#include "../Common/HSHUtility.h"
#include "../json/json.h"

using namespace cocos2d;

const static std::string FILE_NAME = "patternListIndex.json";

CBulletPatternDataManager::CBulletPatternDataManager()
	: m_TestPattern(nullptr)
{
	InitWithJson(m_PatternList, "patternList_1.json");
	InitWithJson(m_MissilePatternList, "aimingMissilePatternList.json");
	InitWithJson(m_MissilePatternList, "normalMissilePatternList.json");
	InitWithJson(m_BonusTimePatternList, "bonusTimePatternList.json");

	m_TestPattern = new sBULLET_PATTERN();
}

CBulletPatternDataManager::~CBulletPatternDataManager()
{
	for (auto data : m_PatternList)
	{
		delete data.second;
		data.second = nullptr;
	}
	m_PatternList.clear();

	if (m_TestPattern != nullptr)
	{
		delete m_TestPattern;
		m_TestPattern = nullptr;
	}
}

CBulletPatternDataManager* CBulletPatternDataManager::Instance()
{
	static CBulletPatternDataManager instance;
	return &instance;
}

const sBULLET_PATTERN* CBulletPatternDataManager::getDataByName(std::string name) const
{
	const sBULLET_PATTERN* data = nullptr;
    if (m_PatternList.find(name) != m_PatternList.end()){
		return m_PatternList.find(name)->second;
    }

	if (m_MissilePatternList.find(name) != m_MissilePatternList.end()){
		return m_MissilePatternList.find(name)->second;
	}

	if (m_BonusTimePatternList.find(name) != m_BonusTimePatternList.end()){
		return m_BonusTimePatternList.find(name)->second;
	}

	CCASSERT(false, StringUtils::format("It is not a pattern key : %s", name.c_str()).c_str());
	return nullptr;
}

void CBulletPatternDataManager::InitWithJson(PATTERN_LIST &list, std::string fileName)
{
	Json::Value root;
	Json::Reader reader;

	// patternList.json 파일 읽음
	std::string file = FileUtils::getInstance()->fullPathForFilename(fileName);
	std::string fileData = FileUtils::getInstance()->getStringFromFile(file);
	size_t pos = fileData.rfind("}");
	fileData = fileData.substr(0, pos + 1);

	// patternList.json log출력
	bool parsingSuccessful = reader.parse(fileData, root);
	if (!parsingSuccessful)
	{
		CCASSERT(false, MakeString("parser failed : \n %s", fileData.c_str()).c_str());
		return;
	}
	CCLOG("Bullet List JSON : \n %s\n", fileData.c_str());


	const Json::Value patternArray = root["patterns"];

	for (unsigned int patternCount = 0; patternCount < patternArray.size(); ++patternCount)
	{
		const Json::Value valuePattern = patternArray[patternCount];

		sBULLET_PATTERN patternInfo;

		patternInfo._index = patternCount;
		patternInfo._patternName = valuePattern["name"].asString();
		patternInfo._widthPadding = valuePattern["widthAngleDistance"].asDouble();
		const Json::Value pattern = valuePattern["pattern"];

		for (unsigned int height = 0; height < pattern.size(); height++)
		{
			std::string patternStr = pattern[height].asString();

			patternInfo._height = pattern.size();
			patternInfo._width = static_cast<int>(patternStr.length());

			for (int width = 0; width < patternInfo._width; width++)
			{
				patternInfo._pattern[(patternInfo._width * height) + width] = patternStr[width];
			}
		}

		this->AddPatternToList(list, patternInfo);
	}
}

void CBulletPatternDataManager::AddPatternToList(PATTERN_LIST &list, const sBULLET_PATTERN& data){
	bool addSuccess = list.emplace(std::pair<std::string, sBULLET_PATTERN*>
                                            (data._patternName, new sBULLET_PATTERN(data))).second;
    
    if (!addSuccess){
        CCLOG("Pattern key was duplicated : %s", data._patternName.c_str());
		CCASSERT(false, StringUtils::format("Pattern key was duplicated : %s",
                                            data._patternName.c_str()).c_str());
    }
}

const sBULLET_PATTERN* CBulletPatternDataManager::getRandomPattern() const 
{
    auto item = m_PatternList.begin();
    std::advance( item, random<int>(0, m_PatternList.size()-1) );
    auto picked = (*item).second;
    
    CCLOG("Pick a pattern :: idx %d name %s", picked->_index, picked->_patternName.c_str());
    
    return picked;
}

const sBULLET_PATTERN* CBulletPatternDataManager::getRandomBonusTimePattern() const
{
	auto item = m_BonusTimePatternList.begin();
	std::advance(item, random<int>(0, m_BonusTimePatternList.size() - 1));
	auto picked = (*item).second;

	CCLOG("Pick a pattern :: idx %d name %s", picked->_index, picked->_patternName.c_str());

	return picked;
}