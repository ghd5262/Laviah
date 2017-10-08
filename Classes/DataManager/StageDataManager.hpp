#pragma once
#include "../Common/HSHUtility.h"
#include "../json/json.h"
#include <vector>
#include <map>

enum STAGE_DATA_TYPE{
    FIT_RANDOM   = 0,
    BELOW_RANDOM = 1,
    NON_RANDOM   = 2,
};

struct STAGE_DATA{
    int _noticeLevel;
    int _patternLevel;
    int _type;
    int _patternIndex;
    int _targetStar;
    float _changeDuration;
    float _duration;
    float _changeTime;
    float _zoomAngle;
    float _zoomSize;
    float _speed;
    cocos2d::Vec2 _pos;
    cocos2d::Color3B _bulletColor;
    cocos2d::Color3B _bgColorTop;
    cocos2d::Color3B _bgColorBottom;

    STAGE_DATA()
    : _noticeLevel(0)
    , _patternLevel(1)
    , _type(0)
    , _patternIndex(0)
    , _targetStar(0)
    , _changeDuration(0.f)
    , _duration(0.f)
    , _changeTime(0.f)
    , _zoomAngle(0.f)
    , _zoomSize(0.f)
    , _speed(0.f)
    , _pos(cocos2d::Vec2::ZERO)
    , _bulletColor(cocos2d::Color3B::WHITE)
    , _bgColorTop(0, 4, 40)
    , _bgColorBottom(0, 63, 110){}
};

typedef std::vector<STAGE_DATA> STAGE_DATA_LIST;
struct STAGE{
    int _index;
    int _openLevel;
    STAGE_DATA_LIST _stageDataLiat;
    
    STAGE()
    : _index(-1)
    , _openLevel(0){
        _stageDataLiat.clear();
    }
};

typedef std::map<int, STAGE*> STAGE_LIST;
class CStageDataManager
{
public:
    static CStageDataManager* Instance();
    
    int getStageMaxLevel(int index);
    const STAGE* getStageByIndex(int index) const;
    const STAGE* getStageByUserLevel();
    
    CC_SYNTHESIZE(int, m_StageLevel, StageLevel);
    
    static cocos2d::Color3B getCurrentBulletColor();
    static cocos2d::Color3B getCurrentBGTopColor();
    static cocos2d::Color3B getCurrentBGBottomColor();
private:
    void initWithJson(STAGE_LIST &list, std::string fileName);
    void addStageToList(const Json::Value& json);
    void addStageDataToStage(STAGE_DATA_LIST& list,
                             const Json::Value& json);
    
    CStageDataManager();
    virtual ~CStageDataManager();
    
private:
    STAGE_LIST m_StageList;
};
