#include "BulletDataManager.h"
#include "../Common/HSHUtility.h"
#include "../json/json.h"

using namespace cocos2d;

namespace BULLET_DEFAULT_PACK {
    const std::string PNG   = "defaultBulletTexturePack.png";
    const std::string PLIST = "defaultBulletTexturePack.plist";
}

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

		param._symbol			= (bullet["symbol"].asString())[0];
        param._boundingRadius   = bullet["bindingRadius"].asDouble();
        param._power            = bullet["power"].asDouble();
		param._speed			= bullet["speed"].asDouble();
        param._isAiming         = bullet["isAimingMissile"].asBool();
        
        AddBulletData(param);
    }
    
    auto util = FileUtils::getInstance();
    auto spriteFrameCache = SpriteFrameCache::getInstance();
    
    if (util->isFileExist(BULLET_DEFAULT_PACK::PNG) &&
        util->isFileExist(BULLET_DEFAULT_PACK::PLIST))
    {
        if (!spriteFrameCache->isSpriteFramesWithFileLoaded(BULLET_DEFAULT_PACK::PLIST)){
            spriteFrameCache->addSpriteFramesWithFile(BULLET_DEFAULT_PACK::PLIST,
                                                      BULLET_DEFAULT_PACK::PNG);
        }
    }
}

CBulletDataManager::~CBulletDataManager()
{
	for (auto data : m_BulletDataList)
	{
		delete data.second;
		data.second = nullptr;
	}
	m_BulletDataList.clear();
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

