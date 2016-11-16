#include "BulletDataManager.h"
#include "../Common/HSHUtility.h"
#include "../json/json.h"

using namespace cocos2d;

CBulletDataManager::CBulletDataManager()
{
    Json::Value root;
    Json::Reader reader;
    
    // patternList.json 파일 읽음
    std::string file = FileUtils::getInstance()->fullPathForFilename("bulletList.json");
    std::string fileData = FileUtils::getInstance()->getStringFromFile(file);
    size_t pos = fileData.rfind("}");
    fileData = fileData.substr(0, pos + 1);
    
    // patternList.json log출력
    bool parsingSuccessful = reader.parse(fileData, root);
    if (! parsingSuccessful)
    {
        CCASSERT(false, MakeString("parser failed : \n %s", fileData.c_str()).c_str());
        return ;
    }
    CCLOG("Bullet List JSON : \n %s\n", fileData.c_str());
    
    
    // stage는 배열이다.
    const Json::Value bulletArray = root["bullets"];
    
    for (unsigned int index = 0; index < bulletArray.size(); ++index)
    {
        const Json::Value bullet = bulletArray[index];
        
        sBULLET_PARAM param;
        
        param._fBouningRadius = bullet["bindingRadius"].asDouble();
        param._fPower = bullet["power"].asDouble();
        param._fDistance = bullet["distance"].asDouble();
        param._symbol = (bullet["symbol"].asString())[0];
        param._isAimingMissile = bullet["isAimingMissile"].asBool();
        param._coinType = static_cast<eCOIN_TYPE>(bullet["coinType"].asInt());
        param._starType = static_cast<eSTAR_TYPE>(bullet["starType"].asInt());
        param._itemType = static_cast<eITEM_TYPE>(bullet["itemType"].asInt());
        
        AddBulletData(param);//(bullet["symbol"].asString())[0],
    }
}

CBulletDataManager::~CBulletDataManager()
{
}

CBulletDataManager* CBulletDataManager::Instance()
{
	static CBulletDataManager instance;
	return &instance;
}

const sBULLET_PARAM* CBulletDataManager::getBulletInfo(const char key) const
{
	if (m_BulletDataList.find(key) == m_BulletDataList.end())
		CCASSERT(false, MakeString("It is not a bullet key : %c", key).c_str());
	return m_BulletDataList.find(key)->second;
}

void CBulletDataManager::AddBulletData(const sBULLET_PARAM& data){
	bool succeed = m_BulletDataList.emplace(std::pair<const char, const sBULLET_PARAM*>
                                               (data._symbol, new sBULLET_PARAM(data))).second;
    if (!succeed){
        CCLOG("Bullet key was duplicated : %c", data._symbol);
        CCASSERT(false, StringUtils::format("Bullet key was duplicated : %c", data._symbol).c_str());
    }
}

