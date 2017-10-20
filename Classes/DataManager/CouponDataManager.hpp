#pragma once
#include "../Common/HSHUtility.h"
#include "../json/json.h"
#include <vector>
#include <map>

enum COUPON_STATE{
    EXPIRED = 0,
    ALREADY_USED = 1,
    WRONG = 2,
    AVAILABLE = 3,
};

struct COUPON_REWARD_DATA{
    int _index;
    long long _eventStartTime;
    long long _eventEndTime;
    std::string _couponKey;
    std::vector<std::string> _keyList;
    std::vector<int> _valueList;
    
    COUPON_REWARD_DATA()
    : _index(0)
    , _eventStartTime(0LL)
    , _eventEndTime(0LL)
    , _couponKey(""){
        _keyList.clear();
        _valueList.clear();
    }
};

typedef std::function<void(int, COUPON_REWARD_DATA)> BOOL_LISTENER;
typedef std::map<std::string, COUPON_REWARD_DATA> COUPON_REWARD_LIST;
class CCouponDataManager
{
public:
    static CCouponDataManager* Instance();
    
    COUPON_REWARD_DATA getRewardByData(std::string couponKey) const;
    void forDebug(Json::Value data, std::string key);
    void CouponCheck(BOOL_LISTENER listener, std::string couponKey);
    bool IsUsedCoupon(std::string couponKey);
    void UseCoupon(std::string couponKey);
    CC_SYNTHESIZE(BOOL_LISTENER, m_CouponCheckListener, CouponCheckListener);
    
private:
    void initWithJson(COUPON_REWARD_LIST &list, std::string fileName);
    void addRewardToList(const Json::Value& json);
    
    CCouponDataManager();
    virtual ~CCouponDataManager();
    
private:
    COUPON_REWARD_LIST m_RewardList;
};
