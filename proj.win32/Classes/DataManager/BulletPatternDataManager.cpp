#include "BulletPatternDataManager.h"
#include "../Common/HSHUtility.h"
#include "../GameObject/Shooter/ShooterHeaders.h"
#include "../json/json.h"

using namespace cocos2d;

const static std::string FILE_NAME = "patternListIndex.json";

CBulletPatternDataManager::CBulletPatternDataManager()
{
    Json::Value root;
    Json::Reader reader;
    
    std::string fileList = FileUtils::getInstance()->fullPathForFilename(FILE_NAME);
    std::string fileListData = FileUtils::getInstance()->getStringFromFile(fileList);
    size_t pos = fileListData.rfind("}");
    fileListData = fileListData.substr(0, pos + 1);
    
    
    bool succeed = reader.parse(fileListData, root);
    if (!succeed)
    {
        CCASSERT(false, StringUtils::format("parser failed : \n %s", FILE_NAME.c_str()).c_str());
        return ;
    }
    CCLOG("fileList JSON : \n %s\n", fileListData.c_str());
    
    //array
    Json::Value fileArray = root["patternListIndex"];
    for (unsigned int index = 0; index < fileArray.size(); ++index)
    {
        std::string file = fileArray[index].asString();
        
        // patternList.json 파일 읽음
        std::string fileName = FileUtils::getInstance()->fullPathForFilename(StringUtils::format("%s.json", file.c_str()));
        std::string patternFileData = FileUtils::getInstance()->getStringFromFile(fileName);
        pos = patternFileData.rfind("}");
        patternFileData = patternFileData.substr(0, pos + 1);
        
        // patternList.json log출력
        succeed = reader.parse(patternFileData, fileArray[index]);
        if (! succeed)
        {
            CCASSERT(false, StringUtils::format("parser failed : \n %s", fileName.c_str()).c_str());
            return ;
        }
        CCLOG("Pattern List JSON : \n %s\n", patternFileData.c_str());
        
        
        // stage는 배열이다.
        const Json::Value patternArray = fileArray[index]["patterns"];
        
        for (unsigned int patternCount = 0; patternCount < patternArray.size(); ++patternCount)
        {
            const Json::Value valuePattern = patternArray[patternCount];
            
            sBULLET_PATTERN patternInfo;
            
            patternInfo._index = patternCount;
            patternInfo._patternName = valuePattern["name"].asString();
            patternInfo._widthPadding = valuePattern["widthAngleDistance"].asDouble();
            patternInfo._heightPadding = valuePattern["heightDistance"].asDouble();
            
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
			this->AddPattern(patternInfo);
        }
    }
}

CBulletPatternDataManager::~CBulletPatternDataManager(){}

CBulletPatternDataManager* CBulletPatternDataManager::Instance()
{
	static CBulletPatternDataManager instance;
	return &instance;
}

const sBULLET_PATTERN* CBulletPatternDataManager::getDataByName(std::string name) const
{
    if (m_PatternList.find(name) == m_PatternList.end()){
		CCASSERT(false, StringUtils::format("It is not a pattern key : %s", name.c_str()).c_str());
    }
	return m_PatternList.find(name)->second;
}

void CBulletPatternDataManager::AddPattern(const sBULLET_PATTERN& data){
	bool addSuccess = m_PatternList.emplace(std::pair<std::string, sBULLET_PATTERN*>
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
    std::advance( item, random<unsigned long>(0, m_PatternList.size()) );
    auto picked = (*item).second;
    
    CCLOG("Pick a pattern :: idx %d name %s", picked->_index, picked->_patternName.c_str());
    
    return picked;
}