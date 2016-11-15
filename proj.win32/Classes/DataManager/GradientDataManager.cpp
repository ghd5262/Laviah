#include "GradientDataManager.h"
#include "../Common/HSHUtility.h"
#include "../json/json.h"

using namespace cocos2d;

CGradientDataManager::CGradientDataManager()
{
    Json::Value root;
    Json::Reader reader;
    
    std::string strGradientList = FileUtils::getInstance()->fullPathForFilename("backgroundGradientList.json");
    std::string gradientListClearData = FileUtils::getInstance()->getStringFromFile(strGradientList);
    size_t pos = gradientListClearData.rfind("}");
    gradientListClearData = gradientListClearData.substr(0, pos + 1);
    
    bool parsingSuccessful = reader.parse(gradientListClearData, root);
    if (!parsingSuccessful)
    {
        CCASSERT(false, MakeString("parser failed : \n %s", gradientListClearData.c_str()).c_str());
        return;
    }
    CCLOG("strGradientList JSON : \n %s\n", gradientListClearData.c_str());
    
    const Json::Value itemArray = root["gradient"];
    
    for (unsigned int itemCount = 0; itemCount < itemArray.size(); ++itemCount)
    {
        const Json::Value valueItem = itemArray[itemCount];
        
        int idx = 0;
        int colorArr[6] = {0, };
        for (auto gradientIdx : valueItem)
        {
            colorArr[idx++] = gradientIdx.asInt();
        }
        
        sGRADIENT_PARAM param;
        
        param._idx = itemCount;
        param._leftColor = Color3B(colorArr[0], colorArr[1], colorArr[2]);
        param._rightColor = Color3B(colorArr[3], colorArr[4], colorArr[5]);
        
        m_GradientList.push_back(param);
    }
}

CGradientDataManager::~CGradientDataManager()
{
}

CGradientDataManager* CGradientDataManager::Instance()
{
    static CGradientDataManager instance;
    return &instance;
}

sGRADIENT_PARAM CGradientDataManager::getRandomGradient() const
{
    auto listSize = m_GradientList.size();
    auto idx = random<size_t>(0, listSize-1);
    auto result = sGRADIENT_PARAM();
    if(listSize > 0)
        result = m_GradientList.at(idx);
    return result;
}