#include "BulletDataManager.h"
#include "../Common/HSHUtility.h"
//#include "../../cocos2d/external/json/rapidjson.h"
//#include "../../cocos2d/external/json/document.h"
//#include "../../cocos2d/external/json/filestream.h"
#include "../json/json.h"

CBulletDataManager::CBulletDataManager()
{
    Json::Value root;
    Json::Reader reader;
    
    // patternList.json 파일 읽음
    std::string strBulletList = FileUtils::getInstance()->fullPathForFilename("bulletList.json");
    std::string bulletListClearData = FileUtils::getInstance()->getStringFromFile(strBulletList);
    size_t pos = bulletListClearData.rfind("}");
    bulletListClearData = bulletListClearData.substr(0, pos + 1);
    
    // patternList.json log출력
    bool parsingSuccessful = reader.parse(bulletListClearData, root);
    if (! parsingSuccessful)
    {
        CCASSERT(false, MakeString("parser failed : \n %s", bulletListClearData.c_str()).c_str());
        return ;
    }
    CCLOG("strBulletList JSON : \n %s\n", bulletListClearData.c_str());
    
    
    // stage는 배열이다.
    const Json::Value bulletArray = root["bullets"];
    
    for (unsigned int bulletCount = 0; bulletCount < bulletArray.size(); ++bulletCount)
    {
        const Json::Value valueBullet = bulletArray[bulletCount];
        
        sBULLET_PARAM param;
        
        param._fBouningRadius = valueBullet["bindingRadius"].asDouble();
        param._fPower = valueBullet["power"].asDouble();
        param._fDistance = valueBullet["distance"].asDouble();
        param._symbol = (valueBullet["symbol"].asString())[0];
        param._isAimingMissile = valueBullet["isAimingMissile"].asBool();
        param._coinType = static_cast<eCOIN_TYPE>(valueBullet["coinType"].asInt());
        param._starType = static_cast<eSTAR_TYPE>(valueBullet["starType"].asInt());
        param._itemType = static_cast<eITEM_TYPE>(valueBullet["itemType"].asInt());
        
        AddBulletData((valueBullet["symbol"].asString())[0], param);
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

sBULLET_PARAM CBulletDataManager::getBulletInfo(const char bulletSymbol) const
{
	if (m_BulletDataList.find(bulletSymbol) == m_BulletDataList.end())
		CCASSERT(false, MakeString("BULLET KEY IS WRONG, DO NOT HAVE KEY : %s", bulletSymbol).c_str());
	return m_BulletDataList.find(bulletSymbol)->second;
}

bool CBulletDataManager::AddBulletData(const char bulletSymbol, sBULLET_PARAM bullet){
	bool addSuccess = m_BulletDataList.emplace(std::pair<const char, sBULLET_PARAM>(bulletSymbol, bullet)).second;
	if (!addSuccess)
		CCASSERT(addSuccess, "BULLET KEY WAS DUPLICATED");
	return addSuccess;
}

