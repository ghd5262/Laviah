#include "StageDataManager.h"
#include "../Common/HSHUtility.h"
#include "../Scene/GameScene.h"
#include "../GameObject/Shooter/ShooterHeaders.h"
//#include "../../cocos2d/external/json/rapidjson.h"
//#include "../../cocos2d/external/json/document.h"
//#include "../../cocos2d/external/json/filestream.h"
#include "../json/json.h"

CStageDataManager::CStageDataManager()
{
    Json::Value root;
    Json::Reader reader;
    m_StageList = new std::vector<sSHOOTER_PARAM>();
    
    
    // stageListIndex.json 파일 읽음
    std::string strStageListIndex = FileUtils::getInstance()->fullPathForFilename("jsonRes/stageList/stageListIndex.json");
    std::string stageListIdxClearData = FileUtils::getInstance()->getStringFromFile(strStageListIndex);
    size_t pos = stageListIdxClearData.rfind("}");
    stageListIdxClearData = stageListIdxClearData.substr(0, pos + 1);
    
    bool parsingSuccessful = reader.parse(stageListIdxClearData, root);
    if (! parsingSuccessful)
    {
        CCASSERT(false, MakeString("parser failed : \n %s", stageListIdxClearData.c_str()).c_str());
        return ;
    }
    // stageListIndex.json log출력
    CCLOG("strStageListIndex JSON : \n %s\n", stageListIdxClearData.c_str());
    
    int ForLOG = 0;
    
    //array
    Json::Value stageListArray = root["stageListIndex"];
    for (unsigned int stageListCount = 0; stageListCount < stageListArray.size(); ++stageListCount)
    {
        
        std::string stageListFileName = stageListArray[stageListCount].asString();
        CCLOG("%s, ", stageListFileName.c_str());
        
        // patternList.json 파일 읽음
        std::string strStageList = FileUtils::getInstance()->fullPathForFilename(MakeString("jsonRes/stageList/%s.json", stageListFileName.c_str()));
        std::string stageListClearData = FileUtils::getInstance()->getStringFromFile(strStageList);
        pos = stageListClearData.rfind("}");
        stageListClearData = stageListClearData.substr(0, pos + 1);
        
        // patternList.json log출력
        parsingSuccessful = reader.parse(stageListClearData, stageListArray[stageListCount]);
        if (! parsingSuccessful)
        {
            CCASSERT(false, MakeString("parser failed : \n %s", stageListClearData.c_str()).c_str());
            return ;
        }
        CCLOG("strStageList JSON : \n %s\n", stageListClearData.c_str());
        
        
        // stage는 배열이다.
        const Json::Value stageArray = stageListArray[stageListCount]["stage"];
        
        for (unsigned int stageCount = 0; stageCount < stageArray.size(); ++stageCount)
        {
            const Json::Value valueStage = stageArray[stageCount];
            
            sSHOOTER_PARAM param;
            
            param._ShooterName = valueStage["ShooterName"].asString();
            param._PatternName = valueStage["PatternName"].asString();
            param._randomShootSymbol = (valueStage["Symbol"].asString())[0];
            param._fStartTime = valueStage["StartTime"].asDouble();
            param._fEndTime = valueStage["EndTime"].asDouble();
            param._fSpeed = valueStage["BulletSpeed"].asDouble();
			param._isAngleRandom = valueStage["AngleRandom"].asBool();
            param._fAngle = valueStage["BulletAngle"].asDouble();
			param._isOneShoot = valueStage["OneShoot"].asBool();
            param._fInterval = valueStage["Interval"].asDouble();
            param._nBulletCountAtOneShoot = valueStage["BulletOneShoot"].asInt();
            param._isFly = valueStage["Fly"].asBool();
            
            
            m_StageList->emplace_back(param);
            ForLOG++;
        }
    }
    
    CCLOG("The Loaded Stage Size is %d AND the Last Idx of Stage Is %d", static_cast<int>(m_StageList->size()), ForLOG);
    
    
    
    //
    //    CCLOG("JSON ReadTest");
    //
    //    std::string value1 = root.get("hello", "defaultvalue").asString();
    //    CCLOG("value1 %s", value1.c_str());
    //
    //    bool value2 = root.get("t", "default").asBool();
    //    CCLOG("value2 %s", value2?"true":"false");
    //
    //    bool value3 = root.get("f", "default").asBool();
    //    CCLOG("value3 %s", value3?"true":"false");
    //
    //    bool value4 = root.get("n", "default").isNull();
    //    CCLOG("value4 %s", value4?"null":"?");
    //
    //    int value5 = root.get("i", "default").asInt();
    //    CCLOG("value5 %d", value5);
    //
    //    double value6 = root.get("pi", "default").asDouble();
    //    CCLOG("value6 %g", value6);
    //
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
//    
//    
//    
//    if (stageListIndexDocument.Parse<0>(clearData.c_str()).HasParseError()) {
//        CCLOG("Err..%s", stageListIndexDocument.Parse<0>(clearData.c_str()).GetParseError());
//        assert(false);//CCASSERT(false, "Err..%s", stageListIndexDocument.Parse<0>(clearData.c_str()).GetParseError());
//    }
//    
//    // patternListIndex는 배열이다.
//    const rapidjson::Value& stageListIndex = stageListIndexDocument["stageListIndex"];
//    for (rapidjson::SizeType stageListCount = 0; stageListCount < stageListIndex.Size(); stageListCount++)
//    {
//        // 파일 이름
//        std::string stageListFileName = stageListIndex[stageListCount].GetString();
//        
//        rapidjson::Document stageListDocument;
//        
//        // patternList.json 파일 읽음
//        std::string strStageList = CCFileUtils::sharedFileUtils()->fullPathForFilename(MakeString("json/stageList/%s.json", stageListFileName.c_str()));
//        bufferSize = 0;
//        json = CCFileUtils::sharedFileUtils()->getFileData(strStageList.c_str(), "rb", &bufferSize);
//        clearData = ((const char*)json);
//        pos = clearData.rfind("}");
//        clearData = clearData.substr(0, pos + 1);
//        
//        // patternList.json log출력
//        CCLOG("strStageList JSON : \n %s\n", clearData.c_str());
//        if (stageListDocument.Parse<0>(clearData.c_str()).HasParseError()) {
//            CCLOG("Err..%s", stageListDocument.Parse<0>(clearData.c_str()).GetParseError());
//            assert(false);//CCASSERT(false, "Err..%s", stageListDocument.Parse<0>(clearData.c_str()).GetParseError());
//        }
//        
//        
//        // stage는 배열이다.
//        const rapidjson::Value& stageList = stageListDocument["stage"];
//        
//        // stage전체를 size만큼 순회하면서 각각의 shooter을 리스트에 삽입
//        for (rapidjson::SizeType stageCount = 0; stageCount < stageList.Size(); stageCount++)
//        {
//            const rapidjson::Value& valueStage = stageList[stageCount];
//            
//            sSHOOTER_PARAM param;
//            
//            param._ShooterName = valueStage["ShooterName"].GetString();
//            param._PatternName = valueStage["PatternName"].GetString();
//            param._randomShootSymbol = (valueStage["Symbol"].GetString())[0];
//            param._fStartTime = valueStage["StartTime"].GetDouble();
//            param._fEndTime = valueStage["EndTime"].GetDouble();
//            param._fSpeed = valueStage["BulletSpeed"].GetDouble();
//            param._fAngle = valueStage["BulletAngle"].GetDouble();
//            param._fInterval = valueStage["Interval"].GetDouble();
//            param._nBulletCountAtOneShoot = valueStage["BulletOneShoot"].GetInt();
//            param._isFly = valueStage["Fly"].GetBool();
//            
//            
//            m_StageList->emplace_back(param);
//        }
//    }
}

CStageDataManager::~CStageDataManager()
{
    delete m_StageList;
}

CStageDataManager* CStageDataManager::Instance()
{
    static CStageDataManager instance;
    return &instance;
}


sSHOOTER_PARAM CStageDataManager::getStageInfoByIndex(int index) const
{
    return m_StageList->at(index);
}