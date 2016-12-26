#include "BulletPatternDataManager.h"
#include "../Common/HSHUtility.h"
#include "../json/json.h"

using namespace cocos2d;

const static std::string FILE_NAME = "patternListIndex.json";

CBulletPatternDataManager::CBulletPatternDataManager()
	: m_TestPattern(nullptr)
{
	InitWithJson(m_PatternList, "patternList_1.json");
	InitWithJson(m_PatternList, "patternList_2.json");
	InitWithJson(m_PatternList, "patternList_3.json");
	InitWithJson(m_PatternList, "patternList_4.json");
	InitWithJson(m_PatternList, "patternList_5.json");
	InitWithJson(m_PatternList, "patternList_6.json");
	InitWithJson(m_PatternList, "patternList_7.json");

	InitWithJson(m_MissilePatternList, "missilePatternList.json");
	InitWithJson(m_BonusTimePatternList, "bonusTimePatternList.json");

	m_TestPattern = new sBULLET_PATTERN();
}

CBulletPatternDataManager::~CBulletPatternDataManager()
{
	auto cleanList = [=](PATTERN_LIST &list){
		for (auto data : list)
		{
			delete data;
			data = nullptr;
		}
		list.clear();
	};

	cleanList(m_PatternList);
	cleanList(m_MissilePatternList);
	cleanList(m_BonusTimePatternList);

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
		patternInfo._level = valuePattern["level"].asInt();
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

		list.emplace_back(new sBULLET_PATTERN(patternInfo));
	}
}

const sBULLET_PATTERN* CBulletPatternDataManager::getNormalPatternByIndex(int index) const
{
	if (m_PatternList.size() >= index) {
		CCLOG("Wrong index : %d", index);
		CCASSERT(false, "Wrong index");
		return nullptr;
	}
	return m_PatternList.at(index);
}

const sBULLET_PATTERN* CBulletPatternDataManager::getMissilePatternByIndex(int index) const
{
	if (m_MissilePatternList.size() >= index) 
		return m_MissilePatternList.at(0);

	return m_MissilePatternList.at(index);
}

const sBULLET_PATTERN* CBulletPatternDataManager::getBonusPatternByIndex(int index) const
{
	if (m_BonusTimePatternList.size() >= index) {
		CCLOG("Wrong index : %d", index);
		CCASSERT(false, "Wrong index");
		return nullptr;
	}

	return m_BonusTimePatternList.at(index);
}

const sBULLET_PATTERN* CBulletPatternDataManager::getRandomPatternByLevel(int level)
{    
	return getRandomDataFromList(m_PatternList, level);
}

const sBULLET_PATTERN* CBulletPatternDataManager::getRandomBonusTimePattern()
{
	return getRandomDataFromList(m_BonusTimePatternList, 0);
}

const sBULLET_PATTERN* CBulletPatternDataManager::getRandomDataFromList(PATTERN_LIST &list, int level)
{
	const sBULLET_PATTERN* picked;
	do{
		auto randomIdx = random<int>(0, list.size() - 1);
		picked = list.at(randomIdx);
	} while (picked->_level > level);

	CCLOG("Pick a pattern :: idx %d level %d", picked->_index, picked->_level);

	return picked;
}