#include "CouponDataManager.hpp"
#include "UserDataManager.h"
#include "DataManagerUtils.h"
#include "NetworkManager.hpp"
#include "../Common/StringUtility.h"

using namespace cocos2d;

CCouponDataManager::CCouponDataManager()
: m_CouponCheckListener(nullptr)
{
    initWithJson(m_RewardList, "couponDataList.json");
}

CCouponDataManager::~CCouponDataManager(){}

CCouponDataManager* CCouponDataManager::Instance()
{
    static CCouponDataManager instance;
    return &instance;
}

void CCouponDataManager::initWithJson(COUPON_REWARD_LIST &list, std::string fileName)
{
    Json::Value        root;
    Json::Reader       reader;
    auto jsonString  = FileUtils::getInstance()->fullPathForFilename(fileName);
    auto clearString = FileUtils::getInstance()->getStringFromFile(jsonString);
    auto pos         = clearString.rfind("}");
    clearString      = clearString.substr(0, pos + 1);
    bool succeed     = reader.parse(clearString, root);
    if (!succeed)
    {
        CCASSERT(false, StringUtils::format("parser failed : \n %s", clearString.c_str()).c_str());
        return;
    }
    CCLOG("RANK REWARD JSON : \n %s\n", clearString.c_str());
    
    const Json::Value array = root["couponDataList"];
    for(auto reward : array)
        this->addRewardToList(reward);
}

void CCouponDataManager::addRewardToList(const Json::Value& json)
{
    COUPON_REWARD_DATA data;
    
    data._index          = json["index"].asInt();
    data._eventStartTime = json["eventStartTime"].asDouble();
    data._eventEndTime   = json["eventEndTime"].asDouble();
    data._couponKey      = StringUtility::toUpper(json["couponKey"].asString());
    auto keyList         = json["rewardKey"];
    auto valueList       = json["rewardValue"];
    
    for(auto key : keyList){
        if(key.isNull()) continue;
        data._keyList.emplace_back(key.asString());
    }
    for(auto value : valueList){
        if(value.isNull()) continue;
        data._valueList.emplace_back(value.asInt());
    }
    
    auto couponKey = data._couponKey;
    if(m_RewardList.find(StringUtility::toUpper(couponKey)) != m_RewardList.end())
    {
        CCLOG("The coupon key duplicated %s", couponKey.c_str());
        CCASSERT(false, "The coupon key duplicated");
        return;
    }
    m_RewardList.emplace(std::pair<std::string, COUPON_REWARD_DATA>(couponKey, data));
}

COUPON_REWARD_DATA CCouponDataManager::getRewardByData(std::string couponKey) const
{
    
    
    auto data = m_RewardList.find(StringUtility::toUpper(couponKey));
    if(data == m_RewardList.end()) return COUPON_REWARD_DATA();
    
    return data->second;
}

void CCouponDataManager::forDebug(Json::Value data, std::string key)
{
    auto rewardData = this->getRewardByData(key);
    
    CCLOG("server request succeed");
    auto eventStartTime    = rewardData._eventStartTime;
    auto eventEndTime      = rewardData._eventEndTime;
    auto currentTimestamp  = time_t(data["currentSeconds"].asDouble());
    bool isEnable = ((eventStartTime <= currentTimestamp) && (eventEndTime >= currentTimestamp));
    
    if(m_CouponCheckListener){
        if(isEnable) m_CouponCheckListener(COUPON_STATE::AVAILABLE, rewardData);
        else         m_CouponCheckListener(COUPON_STATE::EXPIRED, rewardData);
    }
}

void CCouponDataManager::CouponCheck(BOOL_LISTENER listener, std::string couponKey)
{
    m_CouponCheckListener = listener;
    auto rewardData = this->getRewardByData(couponKey);
    if(this->IsUsedCoupon(couponKey)){
        if(m_CouponCheckListener){
            if(rewardData._index <= 0)  m_CouponCheckListener(COUPON_STATE::WRONG, rewardData);
            else                        m_CouponCheckListener(COUPON_STATE::ALREADY_USED, rewardData);
        }
        return;
    }
    
    SERVER_REQUEST([=](Json::Value data){
        forDebug(data, couponKey);
    }, SERVER_REQUEST_KEY::TIMESTAMP_PHP);
}

bool CCouponDataManager::IsUsedCoupon(std::string couponKey)
{
    auto data = this->getRewardByData(couponKey);
    return CUserDataManager::Instance()->getUserData_IsItemExist(USERDATA_KEY::COUPON_LIST, data._index);
}

void CCouponDataManager::UseCoupon(std::string couponKey)
{
    auto data = this->getRewardByData(couponKey);
    CUserDataManager::Instance()->setUserData_ItemExist(USERDATA_KEY::COUPON_LIST, data._index);
}
