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
    
    
    // stageListIndex.json ���� ����
    std::string strPatternListIndex = CCFileUtils::sharedFileUtils()->fullPathForFilename("jsonRes/patternList/patternListIndex.json");
    ssize_t bufferSize = 0;
    unsigned char* patternListIndexJson = CCFileUtils::sharedFileUtils()->getFileData(strPatternListIndex.c_str(), "rb", &bufferSize);
    std::string patternListIdxClearData((const char*)patternListIndexJson);
    size_t pos = patternListIdxClearData.rfind("}");
    patternListIdxClearData = patternListIdxClearData.substr(0, pos + 1);
    
    
    bool parsingSuccessful = reader.parse(patternListIdxClearData, root);
    if (! parsingSuccessful)
    {
        CCASSERT(false, MakeString("parser failed : \n %s", patternListIdxClearData.c_str()).c_str());
        return ;
    }
    // stageListIndex.json log���
    CCLOG("strPatternListIndex JSON : \n %s\n", patternListIdxClearData.c_str());
    
    //array
    Json::Value patternListArray = root["patternListIndex"];
    for (unsigned int patternListCount = 0; patternListCount < patternListArray.size(); ++patternListCount)
    {
        std::string patternListFileName = patternListArray[patternListCount].asString();
        CCLOG("%s ", patternListFileName.c_str());
        
        // patternList.json ���� ����
        std::string strPatternList = CCFileUtils::sharedFileUtils()->fullPathForFilename(MakeString("jsonRes/patternList/%s.json", patternListFileName.c_str()));
        bufferSize = 0;
        unsigned char* patternListJson = CCFileUtils::sharedFileUtils()->getFileData(strPatternList.c_str(), "rb", &bufferSize);
        std::string patternListClearData((const char*)patternListJson);
        pos = patternListClearData.rfind("}");
        patternListClearData = patternListClearData.substr(0, pos + 1);
        
        // patternList.json log���
        parsingSuccessful = reader.parse(patternListClearData, patternListArray[patternListCount]);
        if (! parsingSuccessful)
        {
            CCASSERT(false, MakeString("parser failed : \n %s", patternListClearData.c_str()).c_str());
            return ;
        }
        CCLOG("strPatternList JSON : \n %s\n", patternListClearData.c_str());
        
        
        // stage�� �迭�̴�.
        const Json::Value patternArray = patternListArray[patternListCount]["patterns"];
        
        for (unsigned int patternCount = 0; patternCount < patternArray.size(); ++patternCount)
        {
            const Json::Value valuePattern = patternArray[patternCount];
            
            sPATTERN_SHOOTER_PARAM patternInfo;
            
            // index ����
            patternInfo._index = patternCount;
            
            // name ����
            patternInfo._patternName = valuePattern["name"].asString();
            
            // widthAngleDistance ���� = ������ width ����
            patternInfo._widthAngleDistance = valuePattern["widthAngleDistance"].asDouble();
            
            // heightDistance ���� = ������ height ����
            patternInfo._heightDistance = valuePattern["heightDistance"].asDouble();
            
            // pattern ����
            const Json::Value pattern = valuePattern["pattern"];
            
            // pattern�� sPATTERN_SHOOTER_PARAM�� int�� �迭�� 2���� ���·� �ִ´�.
            for (unsigned int patternHeightCount = 0; patternHeightCount < pattern.size(); patternHeightCount++)
            {
                std::string patternStr = pattern[patternHeightCount].asString();
                CCLOG("pattern[%d] = %s\n", patternHeightCount, patternStr.c_str());
    
                // pattern�� height ����
                patternInfo._height = pattern.size();
        
                // pattern�� width ����
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
//	// patternListIndex.json log���
//	CCLOG("PatternListIndex JSON : \n %s\n", clearData.c_str());
//	if (patternListIdxDocument.Parse<0>(clearData.c_str()).HasParseError()) {
//		CCLOG("Err..%s", patternListIdxDocument.Parse<0>(clearData.c_str()).GetParseError());
//        assert(false);
//        //CCASSERT(false, "Err..%s", patternListDocument.Parse<0>(clearData.c_str()).GetParseError());
//	}
//
//	// patternListIndex�� �迭�̴�.
//	const rapidjson::Value& patternListIndex = patternListIdxDocument["patternListIndex"];
//	for (rapidjson::SizeType patternListCount = 0; patternListCount < patternListIndex.Size(); patternListCount++)
//	{
//		// ���� �̸�
//		std::string patternListFileName = patternListIndex[patternListCount].GetString();
//
//		rapidjson::Document patternListDocument;
//
//		// patternList.json ���� ����
//		std::string strPatternList = CCFileUtils::sharedFileUtils()->fullPathForFilename(MakeString("jsonRes/patternList/%s.json", patternListFileName.c_str()));
//		bufferSize = 0;
//		json = CCFileUtils::sharedFileUtils()->getFileData(strPatternList.c_str(), "rb", &bufferSize);
//		clearData = ((const char*)json);
//		pos = clearData.rfind("}");
//		clearData = clearData.substr(0, pos + 1);
//
//		// patternList.json log���
//		CCLOG("PatternList JSON : \n %s\n", clearData.c_str());
//		if (patternListDocument.Parse<0>(clearData.c_str()).HasParseError()) {
//			CCLOG("Err..%s", patternListDocument.Parse<0>(clearData.c_str()).GetParseError());
//			assert(false);//CCASSERT(false, "Err..%s", patternListDocument.Parse<0>(clearData.c_str()).GetParseError());
//		}
//
//		// patterns�� �迭�̴�.
//		const rapidjson::Value& patternList = patternListDocument["patterns"];
//
//		// patterns��ü�� size��ŭ ��ȸ�ϸ鼭 ������ pattern�� ����Ʈ�� ����
//		for (rapidjson::SizeType patternCount = 0; patternCount < patternList.Size(); patternCount++)
//		{
//			rapidjson::Document patternDocument;
//			const rapidjson::Value& valuePattern = patternList[patternCount];
//			sPATTERN_SHOOTER_PARAM patternInfo;
//
//			// index ����
//			patternInfo._index = patternCount;
//
//			// name ����
//			patternInfo._patternName = valuePattern["name"].GetString();
//
//			// widthAngleDistance ���� = ������ width ����
//			patternInfo._widthAngleDistance = valuePattern["widthAngleDistance"].GetDouble();
//
//			// heightDistance ���� = ������ height ����
//			patternInfo._heightDistance = valuePattern["heightDistance"].GetDouble();
//
//			// pattern ����
//			const rapidjson::Value& pattern = valuePattern["pattern"];
//
//			// pattern�� sPATTERN_SHOOTER_PARAM�� int�� �迭�� 2���� ���·� �ִ´�.
//			for (rapidjson::SizeType patternHeightCount = 0; patternHeightCount < pattern.Size(); patternHeightCount++)
//			{
//				std::string patternStr = pattern[patternHeightCount].GetString();
//				CCLOG("pattern[%d] = %s\n", patternHeightCount, patternStr.c_str());
//
//				// pattern�� height ����
//				patternInfo._height = pattern.Size();
//
//				// pattern�� width ����
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

	// Ȯ�� �α� ���
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