#include "BulletDataManager.h"
#include "../Common/HSHUtility.h"
#include "../../cocos2d/external/json/rapidjson.h"
#include "../../cocos2d/external/json/document.h"
#include "../../cocos2d/external/json/filestream.h"


CBulletDataManager::CBulletDataManager()
{
	rapidjson::Document bulletListDocument;

	// shooterList.json 파일 읽음
	std::string strBulletList = CCFileUtils::sharedFileUtils()->fullPathForFilename("json/bulletList.json");
	ssize_t bufferSize = 0;
	unsigned char* json = CCFileUtils::sharedFileUtils()->getFileData(strBulletList.c_str(), "rb", &bufferSize);
	std::string clearData((const char*)json);
	size_t pos = clearData.rfind("}");
	clearData = clearData.substr(0, pos + 1);

	// shooterList.json log출력
	CCLOG("PatternList JSON : \n %s\n", clearData.c_str());
	if (bulletListDocument.Parse<0>(clearData.c_str()).HasParseError()) {
		CCLOG("Err..%s", bulletListDocument.Parse<0>(clearData.c_str()).GetParseError());
		CCASSERT(false, "Err..%s", bulletListDocument.Parse<0>(clearData.c_str()).GetParseError());
	}

	// shooterName는 배열이다.
	const rapidjson::Value& bulletList = bulletListDocument["bullets"];

	// shooterName전체를 size만큼 순회하면서 각각의 shooter을 리스트에 삽입
	for (rapidjson::SizeType bulletDataCount = 0; bulletDataCount < bulletList.Size(); bulletDataCount++)
	{
		const rapidjson::Value& valueBullet = bulletList[bulletDataCount];

		sBULLET_PARAM param;

		param._TextureName = valueBullet["textureName"].GetString();
		param._fBouningRadius = valueBullet["bindingRadius"].GetDouble();
		param._fPower = valueBullet["power"].GetDouble();
		param._fDistance = valueBullet["distance"].GetDouble();
		param._symbol = (valueBullet["symbol"].GetString())[0];
		param._isFly = valueBullet["isFly"].GetBool();
		param._isAimingMissile = valueBullet["isAimingMissile"].GetBool();
		param._coinType = static_cast<eCOIN_TYPE>(valueBullet["coinType"].GetInt());
		param._starType = static_cast<eSTAR_TYPE>(valueBullet["starType"].GetInt());
		param._itemType = static_cast<eITEM_TYPE>(valueBullet["itemType"].GetInt());

		AddBulletData((valueBullet["symbol"].GetString())[0], param);
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
		CCASSERT(false, "BULLET KEY IS WRONG");
	return m_BulletDataList.find(bulletSymbol)->second;
}

bool CBulletDataManager::AddBulletData(const char bulletSymbol, sBULLET_PARAM bullet){
	bool addSuccess = m_BulletDataList.emplace(std::pair<const char, sBULLET_PARAM>(bulletSymbol, bullet)).second;
	if (!addSuccess)
		CCASSERT(addSuccess, "BULLET KEY WAS DUPLICATED");
	return addSuccess;
}