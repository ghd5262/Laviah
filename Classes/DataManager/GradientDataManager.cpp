#include "GradientDataManager.h"
#include "../json/json.h"

using namespace cocos2d;

CGradientDataManager::CGradientDataManager()
	: m_GradientIdx(0)
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
        CCASSERT(false, StringUtils::format("parser failed : \n %s", gradientListClearData.c_str()).c_str());
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
    
    auto addColor = [=](int r, int g, int b){
        m_ScoreColorList.emplace_back(Color3B(r, g, b));
    };
    
    m_ScoreColorList.clear();
    addColor( 255, 255, 255 );
    addColor( 240, 222, 218 );
    addColor( 230, 196, 193 );
    addColor( 230, 171, 163 );
    addColor( 242, 148, 148 );
    addColor( 240, 142, 115 );
    addColor( 245, 157, 91  );
    addColor( 235, 170, 66  );
    addColor( 240, 190, 53  );
    addColor( 240, 218, 26  );
    addColor( 237, 245, 24  );
    addColor( 227, 227, 25  );
    addColor( 201, 224, 20  );
    addColor( 151, 242, 24  );
    addColor( 81,  242, 22  );
    addColor( 7,   242, 140 );
    addColor( 0,   224, 232 );
    addColor( 12,  186, 235 );
    addColor( 21,  163, 235 );
    addColor( 24,  139, 240 );
    addColor( 29,  88,  224 );
    addColor( 54,  22,  240 );
    addColor( 54,  22,  240 );
    addColor( 54,  22,  240 );
    addColor( 54,  22,  240 );
}

CGradientDataManager::~CGradientDataManager(){}

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

sGRADIENT_PARAM CGradientDataManager::getNextGradient()
{
	m_GradientIdx = m_GradientIdx % (m_GradientList.size() - 1);
	auto result = m_GradientList.at(m_GradientIdx);
	m_GradientIdx++;

	return result;
}

cocos2d::Color3B CGradientDataManager::getColorByLevel(int level)
{
    if(level >= m_ScoreColorList.size()) return m_ScoreColorList.back();
    
    return m_ScoreColorList.at(level);
}