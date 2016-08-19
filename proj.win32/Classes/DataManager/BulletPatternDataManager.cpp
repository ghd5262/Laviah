#include "BulletPatternDataManager.h"
#include "../Common/HSHUtility.h"
#include "../GameObject/Shooter/ShooterHeaders.h"
#include "../json/json.h"

CBulletPatternDataManager::CBulletPatternDataManager()
{
    Json::Value root;
    Json::Reader reader;
    
    
    // stageListIndex.json 파일 읽음
    std::string strPatternListIndex = FileUtils::getInstance()->fullPathForFilename("jsonRes/patternList/patternListIndex.json");
    std::string patternListIdxClearData = FileUtils::getInstance()->getStringFromFile(strPatternListIndex);
    size_t pos = patternListIdxClearData.rfind("}");
    patternListIdxClearData = patternListIdxClearData.substr(0, pos + 1);
    
    
    bool parsingSuccessful = reader.parse(patternListIdxClearData, root);
    if (! parsingSuccessful)
    {
        CCASSERT(false, MakeString("parser failed : \n %s", patternListIdxClearData.c_str()).c_str());
        return ;
    }
    // stageListIndex.json log출력
    CCLOG("strPatternListIndex JSON : \n %s\n", patternListIdxClearData.c_str());
    
    //array
    Json::Value patternListArray = root["patternListIndex"];
    for (unsigned int patternListCount = 0; patternListCount < patternListArray.size(); ++patternListCount)
    {
        std::string patternListFileName = patternListArray[patternListCount].asString();
        CCLOG("%s ", patternListFileName.c_str());
        
        // patternList.json 파일 읽음
        std::string strPatternList = FileUtils::getInstance()->fullPathForFilename(MakeString("jsonRes/patternList/%s.json", patternListFileName.c_str()));
        std::string patternListClearData = FileUtils::getInstance()->getStringFromFile(strPatternList);
        pos = patternListClearData.rfind("}");
        patternListClearData = patternListClearData.substr(0, pos + 1);
        
        // patternList.json log출력
        parsingSuccessful = reader.parse(patternListClearData, patternListArray[patternListCount]);
        if (! parsingSuccessful)
        {
            CCASSERT(false, MakeString("parser failed : \n %s", patternListClearData.c_str()).c_str());
            return ;
        }
        CCLOG("strPatternList JSON : \n %s\n", patternListClearData.c_str());
        
        
        // stage는 배열이다.
        const Json::Value patternArray = patternListArray[patternListCount]["patterns"];
        
        for (unsigned int patternCount = 0; patternCount < patternArray.size(); ++patternCount)
        {
            const Json::Value valuePattern = patternArray[patternCount];
            
            sPATTERN_SHOOTER_PARAM patternInfo;
            
            // index 저장
            patternInfo._index = patternCount;
            
            // name 저장
            patternInfo._patternName = valuePattern["name"].asString();
            
            // widthAngleDistance 저장 = 패턴의 width 간격
            patternInfo._widthAngleDistance = valuePattern["widthAngleDistance"].asDouble();
            
            // heightDistance 저장 = 패턴의 height 간격
            patternInfo._heightDistance = valuePattern["heightDistance"].asDouble();
            
            // pattern 저장
            const Json::Value pattern = valuePattern["pattern"];
            
            // pattern을 sPATTERN_SHOOTER_PARAM의 int형 배열에 2차원 형태로 넣는다.
            for (unsigned int patternHeightCount = 0; patternHeightCount < pattern.size(); patternHeightCount++)
            {
                std::string patternStr = pattern[patternHeightCount].asString();
                CCLOG("pattern[%d] = %s\n", patternHeightCount, patternStr.c_str());
    
                // pattern의 height 저장
                patternInfo._height = pattern.size();
        
                // pattern의 width 저장
                patternInfo._width = static_cast<int>(patternStr.length());
    
    
                for (int charCount = 0; charCount < patternInfo._width; charCount++)
                {
                    patternInfo._pattern
                    [(patternInfo._width * patternHeightCount) + charCount]
                    = patternStr[charCount];
                }
            }
			this->AddPattern(patternInfo._patternName, patternInfo);
        }
    }
}

CBulletPatternDataManager::~CBulletPatternDataManager()
{
}

CBulletPatternDataManager* CBulletPatternDataManager::Instance()
{
	static CBulletPatternDataManager instance;
	return &instance;
}

sPATTERN_SHOOTER_PARAM CBulletPatternDataManager::getPatternInfo(std::string patternName) const
{
    if (m_PatternList.find(patternName) == m_PatternList.end()){
		CCASSERT(false, MakeString("PATTERN KEY IS WRONG : %s", patternName.c_str()).c_str());
    }
	return m_PatternList.find(patternName)->second;
}

bool CBulletPatternDataManager::AddPattern(std::string patternName, sPATTERN_SHOOTER_PARAM pattern){
	bool addSuccess = m_PatternList.emplace(std::pair<std::string, sPATTERN_SHOOTER_PARAM>(patternName, pattern)).second;
    if (!addSuccess){
		CCASSERT(addSuccess, MakeString("PATTERN KEY WAS DUPLICATED : %s", patternName.c_str()).c_str());
    }
	return addSuccess;
}