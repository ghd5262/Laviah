#include "BulletPatternDataManager.h"
#include "../Common/HSHUtility.h"
#include "../GameObject/Shooter/ShooterHeaders.h"
//#include "../../cocos2d/external/json/rapidjson.h"
//#include "../../cocos2d/external/json/document.h"
//#include "../../cocos2d/external/json/filestream.h"
#include "../json/json.h"

CBulletPatternDataManager::CBulletPatternDataManager()
{
    Json::Value root;
    Json::Reader reader;
    
    
    // stageListIndex.json 파일 읽음
    std::string strPatternListIndex = FileUtils::getInstance()->fullPathForFilename("jsonRes/patternList/patternListIndex.json");
    ssize_t bufferSize = 0;
    unsigned char* patternListIndexJson = FileUtils::getInstance()->getFileData(strPatternListIndex.c_str(), "rb", &bufferSize);
    std::string patternListIdxClearData((const char*)patternListIndexJson);
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
        std::string strPatternList = CCFileUtils::sharedFileUtils()->fullPathForFilename(MakeString("jsonRes/patternList/%s.json", patternListFileName.c_str()));
        bufferSize = 0;
        unsigned char* patternListJson = CCFileUtils::sharedFileUtils()->getFileData(strPatternList.c_str(), "rb", &bufferSize);
        std::string patternListClearData((const char*)patternListJson);
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
    
    
    
    
//	rapidjson::Document patternListIdxDocument;
//	
//	std::string strPatternListIdx = CCFileUtils::sharedFileUtils()->fullPathForFilename("jsonRes/patternList/patternListIndex.json");
//	ssize_t bufferSize = 0;
//	unsigned char* json = CCFileUtils::sharedFileUtils()->getFileData(strPatternListIdx.c_str(), "rb", &bufferSize);
//	std::string clearData((const char*)json);
//	size_t pos = clearData.rfind("}");
//	clearData = clearData.substr(0, pos + 1);
//
//	// patternListIndex.json log출력
//	CCLOG("PatternListIndex JSON : \n %s\n", clearData.c_str());
//	if (patternListIdxDocument.Parse<0>(clearData.c_str()).HasParseError()) {
//		CCLOG("Err..%s", patternListIdxDocument.Parse<0>(clearData.c_str()).GetParseError());
//        assert(false);
//        //CCASSERT(false, "Err..%s", patternListDocument.Parse<0>(clearData.c_str()).GetParseError());
//	}
//
//	// patternListIndex는 배열이다.
//	const rapidjson::Value& patternListIndex = patternListIdxDocument["patternListIndex"];
//	for (rapidjson::SizeType patternListCount = 0; patternListCount < patternListIndex.Size(); patternListCount++)
//	{
//		// 파일 이름
//		std::string patternListFileName = patternListIndex[patternListCount].GetString();
//
//		rapidjson::Document patternListDocument;
//
//		// patternList.json 파일 읽음
//		std::string strPatternList = CCFileUtils::sharedFileUtils()->fullPathForFilename(MakeString("jsonRes/patternList/%s.json", patternListFileName.c_str()));
//		bufferSize = 0;
//		json = CCFileUtils::sharedFileUtils()->getFileData(strPatternList.c_str(), "rb", &bufferSize);
//		clearData = ((const char*)json);
//		pos = clearData.rfind("}");
//		clearData = clearData.substr(0, pos + 1);
//
//		// patternList.json log출력
//		CCLOG("PatternList JSON : \n %s\n", clearData.c_str());
//		if (patternListDocument.Parse<0>(clearData.c_str()).HasParseError()) {
//			CCLOG("Err..%s", patternListDocument.Parse<0>(clearData.c_str()).GetParseError());
//			assert(false);//CCASSERT(false, "Err..%s", patternListDocument.Parse<0>(clearData.c_str()).GetParseError());
//		}
//
//		// patterns는 배열이다.
//		const rapidjson::Value& patternList = patternListDocument["patterns"];
//
//		// patterns전체를 size만큼 순회하면서 각각의 pattern을 리스트에 삽입
//		for (rapidjson::SizeType patternCount = 0; patternCount < patternList.Size(); patternCount++)
//		{
//			rapidjson::Document patternDocument;
//			const rapidjson::Value& valuePattern = patternList[patternCount];
//			sPATTERN_SHOOTER_PARAM patternInfo;
//
//			// index 저장
//			patternInfo._index = patternCount;
//
//			// name 저장
//			patternInfo._patternName = valuePattern["name"].GetString();
//
//			// widthAngleDistance 저장 = 패턴의 width 간격
//			patternInfo._widthAngleDistance = valuePattern["widthAngleDistance"].GetDouble();
//
//			// heightDistance 저장 = 패턴의 height 간격
//			patternInfo._heightDistance = valuePattern["heightDistance"].GetDouble();
//
//			// pattern 저장
//			const rapidjson::Value& pattern = valuePattern["pattern"];
//
//			// pattern을 sPATTERN_SHOOTER_PARAM의 int형 배열에 2차원 형태로 넣는다.
//			for (rapidjson::SizeType patternHeightCount = 0; patternHeightCount < pattern.Size(); patternHeightCount++)
//			{
//				std::string patternStr = pattern[patternHeightCount].GetString();
//				CCLOG("pattern[%d] = %s\n", patternHeightCount, patternStr.c_str());
//
//				// pattern의 height 저장
//				patternInfo._height = pattern.Size();
//
//				// pattern의 width 저장
//				patternInfo._width = patternStr.length();
//
//
//				for (int charCount = 0; charCount < patternInfo._width; charCount++)
//				{
//					patternInfo._pattern
//						[(patternInfo._width * patternHeightCount) + charCount]
//					= patternStr[charCount];
//				}
//			}
//			this->AddPattern(patternInfo._patternName, patternInfo);
//		}
//	}

	// 확인 로그 출력
//	std::for_each(m_PatternList.begin(), m_PatternList.end(), [](std::pair<std::string, sPATTERN_SHOOTER_PARAM> pattern)
//	{
//		for (int patternHeightCount = 0; patternHeightCount < pattern.second._height; patternHeightCount++)
//		{
//			for (int charCount = 0; charCount < pattern.second._width; charCount++)
//			{
//				if (pattern.second._pattern[(pattern.second._width * patternHeightCount) + charCount] != -1){
//					OutputDebugStringA(MakeString("%c", pattern.second._pattern[(pattern.second._width * patternHeightCount) + charCount]).c_str());
//				}
//				else{
//					OutputDebugStringA(" ");
//				}
//			}
//			CCLOG("\n");
//		}
//	});
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
	if (m_PatternList.find(patternName) == m_PatternList.end())
		CCASSERT(false, "PATTERN KEY IS WRONG");
	return m_PatternList.find(patternName)->second;
}

bool CBulletPatternDataManager::AddPattern(std::string patternName, sPATTERN_SHOOTER_PARAM pattern){
	bool addSuccess = m_PatternList.emplace(std::pair<std::string, sPATTERN_SHOOTER_PARAM>(patternName, pattern)).second;
	if (!addSuccess)
		CCASSERT(addSuccess, "PATTERN KEY WAS DUPLICATED");
	return addSuccess;
}