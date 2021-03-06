#include "BulletPatternDataManager.h"
#include "DataManagerUtils.h"
#include "../json/json.h"

using namespace cocos2d;

const static std::string FILE_NAME = "patternListIndex.json";

CBulletPatternDataManager::CBulletPatternDataManager()
	: m_TestPattern(nullptr)
{
	initWithJson(m_PatternList, "patternList_1.json");
	initWithJson(m_PatternList, "patternList_2.json");
	initWithJson(m_PatternList, "patternList_3.json");
	initWithJson(m_PatternList, "patternList_4.json");
	initWithJson(m_PatternList, "patternList_5.json");
	initWithJson(m_PatternList, "patternList_6.json");
	initWithJson(m_PatternList, "patternList_7.json");

	initWithJson(m_MissilePatternList, "missilePatternList.json");
//	initWithJson(m_BonusTimePatternList, "bonusTimePatternList.json");
    
//    initWithJson(m_ConstellationPatternList, "constellationPatternList.json");
    
    initWithJson(m_TutorialPatternList, "tutorialPatternList.json");

	m_TestPattern = new sBULLET_PATTERN();
}

CBulletPatternDataManager::~CBulletPatternDataManager()
{   
    DATA_MANAGER_UTILS::listDeleteAndClean(m_PatternList);
    DATA_MANAGER_UTILS::listDeleteAndClean(m_MissilePatternList);
//    DATA_MANAGER_UTILS::listDeleteAndClean(m_BonusTimePatternList);
//    DATA_MANAGER_UTILS::listDeleteAndClean(m_ConstellationPatternList);
    DATA_MANAGER_UTILS::listDeleteAndClean(m_TutorialPatternList);
    
    CC_SAFE_DELETE(m_TestPattern);
}

CBulletPatternDataManager* CBulletPatternDataManager::Instance()
{
	static CBulletPatternDataManager instance;
	return &instance;
}

void CBulletPatternDataManager::initWithJson(PATTERN_LIST &list, std::string fileName)
{
	Json::Value root;
	Json::Reader reader;

	// patternList.json
	std::string file = FileUtils::getInstance()->fullPathForFilename(fileName);
	std::string fileData = FileUtils::getInstance()->getStringFromFile(file);
	size_t pos = fileData.rfind("}");
	fileData = fileData.substr(0, pos + 1);

	// patternList.json
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

        int index = int(list.size());
        if(!valuePattern["idx"].isNull())
            index = valuePattern["idx"].asInt();
        
        patternInfo._index = index;
		patternInfo._level = valuePattern["level"].asInt();
        patternInfo._type  = valuePattern["type"].asInt();
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
	if (m_PatternList.size() <= index) {
		CCLOG("Wrong normal pattern index : %d", index);
		CCASSERT(false, "Wrong index");
		return nullptr;
	}
	return m_PatternList.at(index);
}

const sBULLET_PATTERN* CBulletPatternDataManager::getMissilePatternByIndex(int index) const
{
    // missile patterns have to search using index.
    auto pattern = std::find_if(m_MissilePatternList.begin(), m_MissilePatternList.end(), [=](const sBULLET_PATTERN* data){
        return (data->_index == index);
    });
    
    if(pattern == m_MissilePatternList.end()) {
        CCLOG("There is no missile pattern index %d", index);
        pattern = m_MissilePatternList.begin();
    }
    return *pattern;
}

const sBULLET_PATTERN* CBulletPatternDataManager::getBonusPatternByIndex(int index) const
{
	if (m_BonusTimePatternList.size() <= index) {
		CCLOG("Wrong bonus pattern index : %d", index);
		CCASSERT(false, "Wrong index");
		return nullptr;
	}

	return m_BonusTimePatternList.at(index);
}

const sBULLET_PATTERN* CBulletPatternDataManager::getTutorialPatternByIndex(int index) const
{
    if (m_TutorialPatternList.size() <= index) {
        CCLOG("Wrong tutorial pattern index : %d", index);
        CCASSERT(false, "Wrong index");
        return nullptr;
    }
    
    return m_TutorialPatternList.at(index);
}

const sBULLET_PATTERN* CBulletPatternDataManager::getRandomNormalPatternByLevel(int level,
                                                                                int type,
                                                                                bool levelBelow)
{
    auto levelList = DATA_MANAGER_UTILS::getListByFunc([=](const sBULLET_PATTERN* data){
        if(levelBelow)
            return data->_level <= level && data->_type <= type;
        else
            return data->_level == level && data->_type <= type;
    }, m_PatternList);
    
    auto size = levelList.size();
    if(!size) {
        CCASSERT(false, "There is no pattern with type and level");
        return nullptr;
    }
    
    auto randomIndex = random<int>(0, size-1);
    return levelList.at(randomIndex);
}

const sBULLET_PATTERN* CBulletPatternDataManager::getRandomConstellationPatternByLevel(int level, bool below)
{
    if(below){
        return getRandomPatternFromList([=](const sBULLET_PATTERN* data){
            return data->_level > level;
        }, m_ConstellationPatternList);
    }
    else
    {
        return getRandomPatternFromList([=](const sBULLET_PATTERN* data){
            return data->_level != level;
        }, m_ConstellationPatternList);
    }
}

const sBULLET_PATTERN* CBulletPatternDataManager::getRandomBonusTimePattern()
{
    return getRandomPatternFromList([=](const sBULLET_PATTERN* data){
        return data->_level == 0;
    }, m_BonusTimePatternList);
}

const sBULLET_PATTERN* CBulletPatternDataManager::getRandomPatternFromList(const PATTERN_PICK& callFunc,
                                                                           PATTERN_LIST &list)
{
	const sBULLET_PATTERN* picked;
	do{
        auto size = list.size();
		auto randomIdx = random<int>(0, int(size) - 1);
		picked = list.at(randomIdx);
	} while (callFunc(picked));

	CCLOG("Pick a pattern :: idx %d level %d", picked->_index, picked->_level);

	return picked;
}
