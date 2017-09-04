#pragma once
#include "../../DataManager/AchievementDataManager.hpp"
#include "../Popup.h"
#include <vector>
#include <map>

enum UFO_TYPE{
    UFO_NONE = 0,
    UFO_COSTUME = 1,
    UFO_FREE = 2,
    UFO_RANK_1 = 3,
    UFO_RANK_2 = 4,
    UFO_RANK_3 = 5
};

struct UFO_DATA{
    std::string _ufoName;
    std::string _lightName;
    cocos2d::Vec2 _startPos;
    cocos2d::Vec2 _targetPos1;
    cocos2d::Vec2 _targetPos2;
    cocos2d::Vec2 _particlePosVar;
    cocos2d::Vec2 _particleGravity;
    float _particleRotation;
    
    UFO_DATA()
    : _ufoName("")
    , _lightName("")
    , _startPos(cocos2d::Vec2::ZERO)
    , _targetPos1(cocos2d::Vec2::ZERO)
    , _targetPos2(cocos2d::Vec2::ZERO)
    , _particlePosVar(cocos2d::Vec2::ZERO)
    , _particleGravity(cocos2d::Vec2::ZERO)
    , _particleRotation(0.f){}
    
    UFO_DATA(std::string ufo,
             std::string light,
             cocos2d::Vec2 startPos,
             cocos2d::Vec2 targetPos1,
             cocos2d::Vec2 targetPos2,
             cocos2d::Vec2 particlePosVar,
             cocos2d::Vec2 particleGravity,
             float particleRotation)
    : _ufoName(ufo)
    , _lightName(light)
    , _startPos(startPos)
    , _targetPos1(targetPos1)
    , _targetPos2(targetPos2)
    , _particlePosVar(particlePosVar)
    , _particleGravity(particleGravity)
    , _particleRotation(particleRotation){}
};

typedef std::vector<sREWARD_DATA> REWARD_LIST;

class CRewardPopup : public CPopup
{
    typedef std::function<void(bool)> EXIT_CALLBACK;
    
public:
	static CRewardPopup* create();
    CRewardPopup* AddRewardToList(std::vector<sREWARD_DATA> list);
    CRewardPopup* setExitCallback(const EXIT_CALLBACK &callback);
    CRewardPopup* setTitle(std::string title);
    CRewardPopup* setIsPaidFeature(int cost);
    CRewardPopup* setIsUFOEnable(bool enable);
    CPopup* show(Node* parent = nullptr, int zOrder = 0);
    void AddRewardToList(std::string key, int value);

    static void createFlyAction(Node* sender,
                                Vec2 targetPos1,
                                Vec2 targetPos2);
private:
	CPopup* createRewardDP(sREWARD_DATA data);
    void createUFO();
    void lightOn();
	void open();
    void play();
    void end();
    void exit(bool isPlay = false);
    void showButtons();
    bool isItemRemain();

	CRewardPopup()
    : m_RewardDP(nullptr)
    , m_GetButton(nullptr)
    , m_PlayButton(nullptr)
    , m_ExitCallback(nullptr)
    , m_UFO(nullptr)
    , m_UFOLight(nullptr)
    , m_TitleLabel(nullptr)
    , m_Title("")
    , m_RewardIndex(0)
    , m_Cost(0)
    , m_IsPaidFeature(false)
    , m_IsUFOEnable(false){};
	virtual ~CRewardPopup(){};

private:
	CPopup* m_RewardDP;
    CMyButton* m_GetButton;
    CMyButton* m_PlayButton;
	REWARD_LIST m_RewardList;
    EXIT_CALLBACK m_ExitCallback;
    sREWARD_DATA m_LastSavedData;
    cocos2d::Sprite* m_UFO;
    cocos2d::Sprite* m_UFOLight;
    cocos2d::Label* m_TitleLabel;
    std::string m_Title;
	int m_RewardIndex;
    int m_Cost;
    bool m_IsPaidFeature;
    bool m_IsUFOEnable;
};
