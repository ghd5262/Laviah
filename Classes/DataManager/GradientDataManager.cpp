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
    
    auto addScoreColor = [=](int r, int g, int b){
        m_ScoreColorList.emplace_back(Color3B(r, g, b));
    };
    
    m_ScoreColorList.clear();
//    addScoreColor( 255, 255, 255 );
//    addScoreColor( 240, 222, 218 );
//    addScoreColor( 230, 196, 193 );
//    addScoreColor( 230, 171, 163 );
//    addScoreColor( 242, 148, 148 );
//    addScoreColor( 240, 142, 115 );
//    addScoreColor( 245, 157, 91  );
//    addScoreColor( 235, 170, 66  );
//    addScoreColor( 240, 190, 53  );
//    addScoreColor( 240, 218, 26  );
//    addScoreColor( 237, 245, 24  );
//    addScoreColor( 227, 227, 25  );
//    addScoreColor( 201, 224, 20  );
//    addScoreColor( 151, 242, 24  );
//    addScoreColor( 81,  242, 22  );
//    addScoreColor( 7,   242, 140 );
//    addScoreColor( 0,   224, 232 );
//    addScoreColor( 12,  186, 235 );
//    addScoreColor( 21,  163, 235 );
//    addScoreColor( 24,  139, 240 );
//    addScoreColor( 29,  88,  224 );
//    addScoreColor( 54,  22,  240 );
//    addScoreColor( 54,  22,  240 );
//    addScoreColor( 54,  22,  240 );
//    addScoreColor( 54,  22,  240 );
    
    
//    addScoreColor( 230, 196, 193 );
//    addScoreColor( 240, 142, 115 );
//    addScoreColor( 240, 190, 53  );
//    addScoreColor( 227, 227, 25  );
//    addScoreColor( 81,  242, 22  );
    
//    addScoreColor( 83 , 174, 252 );
//    addScoreColor( 53 , 179, 181 );
//    addScoreColor( 53 , 181, 127 );
//    addScoreColor( 76 , 181, 53  );
//    addScoreColor( 187, 212, 76  );
//    addScoreColor( 255, 215, 36  );

//    addScoreColor( 237, 207, 85  );
//    addScoreColor( 203, 214, 75  );
//    addScoreColor( 14 , 191, 124 );
//    addScoreColor( 14 , 187, 207 );
//    addScoreColor( 0  , 158, 176 );
//    addScoreColor( 0  , 120, 176 );
    
    addScoreColor( 174, 171, 160  );
    addScoreColor( 194, 187, 161  );
    addScoreColor( 211, 199, 150  );
    addScoreColor( 222, 203, 124  );
    addScoreColor( 237, 207, 85   );
    
    auto addBulletColor = [=](int r, int g, int b){
        m_BulletColorList.emplace_back(Color3B(r, g, b));
    };
    m_BulletColorList.clear();
//    addBulletColor( 120, 157, 255 );
//    addBulletColor( 162, 221, 107 );
//    addBulletColor( 254, 251, 115 );
//    addBulletColor( 255, 186, 63  );
//    addBulletColor( 255, 65,  58  );
//    addBulletColor( 185, 140, 253 );
//    addBulletColor( 140, 252, 253 );
    
//    addBulletColor( 250, 183, 60  );
//    addBulletColor( 250, 107, 60  );
//    addBulletColor( 250, 60 , 66  );
//    addBulletColor( 250, 60 , 193 );
//    addBulletColor( 109, 60 , 250 );
//    addBulletColor( 145, 60 , 250 );
//    addBulletColor( 250, 183, 60  );
    
    
    
    addBulletColor( 255, 255, 255 );
    addBulletColor( 240, 222, 218 );
    addBulletColor( 230, 196, 193 );
    addBulletColor( 230, 171, 163 );
    addBulletColor( 255, 120, 120 );
    addBulletColor( 255, 79,  79  );
    addBulletColor( 255, 43,  43  );
    addBulletColor( 255, 30,  0   );
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
	m_GradientIdx = ++m_GradientIdx % (m_GradientList.size());
	auto result = m_GradientList.at(m_GradientIdx);

	return result;
}

cocos2d::Color3B CGradientDataManager::getScoreColorByLevel(int level)
{
    level = (level-1) / 5;// -1 : level starts from 0.
    if(level >= m_ScoreColorList.size()) return m_ScoreColorList.back();
    
    return m_ScoreColorList.at(level);
}

cocos2d::Color3B CGradientDataManager::getBulletColorByLevel(int level)
{
    if(level >= m_BulletColorList.size()) return m_BulletColorList.back();
    
    return m_BulletColorList.at(level);
}
