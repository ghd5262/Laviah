#include "ChallengeDataManager.hpp"
#include "UserDataManager.h"
#include "ChallengeChecker/ChallengeClearChecker.h"
#include "ChallengeRewarder/ChallengeRewarder.hpp"
#include "../json/json.h"
#include "../Common/StringUtility.h"

using namespace cocos2d;
using namespace CHALLENGE_DATA_KEY;

const static std::string DATA_FILE_NAME = "challengeList.json";

CChallengeDataManager::CChallengeDataManager()
: m_Checker(new CChallengeClearChecker())
, m_Rewarder(new CChallengeRewarder())
{
    initMaterialKeyList();
    initRewardKeyList();
    initWithJson(m_CallengeDataList, DATA_FILE_NAME);
}

CChallengeDataManager::~CChallengeDataManager()
{
    auto cleanList = [=](CHALLENGE_LIST &list){
        for (auto data : list)
        {
            delete data;
            data = nullptr;
        }
        list.clear();
    };
    
    cleanList(m_CallengeDataList);
    
    CC_SAFE_DELETE(m_Checker);
    CC_SAFE_DELETE(m_Rewarder);
}

CChallengeDataManager* CChallengeDataManager::Instance()
{
    static CChallengeDataManager instance;
    return &instance;
}

void CChallengeDataManager::initWithJson(CHALLENGE_LIST &list, std::string fileName)
{
    Json::Value root;
    Json::Reader reader;
    
    std::string file = FileUtils::getInstance()->fullPathForFilename(fileName);
    std::string fileData = FileUtils::getInstance()->getStringFromFile(file);
    size_t pos = fileData.rfind("}");
    fileData = fileData.substr(0, pos + 1);
    
    bool parsingSuccessful = reader.parse(fileData, root);
    if (!parsingSuccessful)
    {
        CCASSERT(false, MakeString("parser failed : \n %s", fileData.c_str()).c_str());
        return;
    }
    CCLOG("Challenge List JSON : \n %s\n", fileData.c_str());
    
    
    const Json::Value challengeArray = root["challenges"];
	typedef std::pair<std::string, int> STD_PAIR;
    
    auto initList = [=](MATERIAL_LIST& list, KEY_LIST &keyList, const Json::Value array){
        for(auto key : keyList)
        {
            if(!array[key].asInt()) continue;
            
            list.emplace(STD_PAIR(key, array[key].asInt()));
        }
    };

    for (unsigned int count = 0; count < challengeArray.size(); ++count)
    {
        const Json::Value challenge = challengeArray[count];
        
        sCHALLENGE_PARAM* challengeInfo = new sCHALLENGE_PARAM();
        
        challengeInfo->_index        = int(list.size());
        challengeInfo->_level        = challenge["level"].asInt();
        challengeInfo->_oneTime      = challenge["oneTime"].asBool();
        challengeInfo->_contents     = challenge["contents"].asString();
        const Json::Value materialArray  = challenge["material"];
        const Json::Value rewardArray    = challenge["reward"];
        
        initList(challengeInfo->_materialList, m_MaterialKeyList, materialArray);
        initList(challengeInfo->_rewardList,   m_RewardKeyList,   rewardArray);

        list.emplace_back(challengeInfo);
    }
}

void CChallengeDataManager::Reward(int index)
{
    auto challengeData = getChallengeByIndex(index);
    
    for(auto reward : challengeData->_rewardList)
    {
        auto key = reward.first;
        auto rewardValue = reward.second;
        
        auto rewarder = m_RewarderList.find(key);
        if(rewarder == std::end(m_RewarderList)) continue;
        
        rewarder->second(rewardValue);
    }
}

const sCHALLENGE_PARAM* CChallengeDataManager::SkipChallenge(int index)
{
	CUserDataManager::Instance()->setUserData_ItemRemove(USERDATA_KEY::CHALLENGE_CUR_LIST, index);
	auto newChallenge = this->getMewRandomChallengeByLevel(1, false);
	CUserDataManager::Instance()->setUserData_ItemGet(USERDATA_KEY::CHALLENGE_CUR_LIST, newChallenge->_index);

	return newChallenge;
}

const sCHALLENGE_PARAM* CChallengeDataManager::getChallengeByIndex(int index) const
{
    if (m_CallengeDataList.size() <= index) {
        CCLOG("Wrong index : %d", index);
        CCASSERT(false, "Wrong index");
        return nullptr;
    }
    return m_CallengeDataList.at(index);
}

const sCHALLENGE_PARAM* CChallengeDataManager::getNewRandomChallenge(bool oneTime)
{
    if(oneTime){
		return getNewRandomChallengeFromList([=](const sCHALLENGE_PARAM* data){
            return !data->_oneTime;
        }, m_CallengeDataList);
    }
    else
    {
		return getNewRandomChallengeFromList([=](const sCHALLENGE_PARAM* data){
            return data->_oneTime;
        }, m_CallengeDataList);
    }
}

const sCHALLENGE_PARAM* CChallengeDataManager::getMewRandomChallengeByLevel(int level, bool below)
{
    if(below){
		return getNewRandomChallengeFromList([=](const sCHALLENGE_PARAM* data){
            return data->_level > level;
        }, m_CallengeDataList);
    }
    else
    {
		return getNewRandomChallengeFromList([=](const sCHALLENGE_PARAM* data){
            return data->_level != level;
        }, m_CallengeDataList);
    }
}

const sCHALLENGE_PARAM* CChallengeDataManager::getNewRandomChallengeFromList(const CHALLENGE_PICK& callFunc,
                                                                          CHALLENGE_LIST &list)
{
    const sCHALLENGE_PARAM* picked;
    do{
        auto size = list.size();
        auto randomIdx = random<int>(0, int(size) - 1);
        picked = list.at(randomIdx);
    } while (callFunc(picked));
    
    CCLOG("Pick a challenge :: \nidx %d \ncontent %s \nlevel %d",
          picked->_index,
          picked->_contents.c_str(),
          picked->_level);
    
    return picked;
}

bool CChallengeDataManager::checkCurrentChallengeComplete(int index)
{
	auto challengeData = this->getChallengeByIndex(index);

    if(challengeData->_materialList.size() == 0) return false;

    for(auto material : challengeData->_materialList)
    {
        auto key = material.first;
        auto mtrlValue = material.second;
        
        auto checker = m_CheckerList.find(key);
        if(checker == std::end(m_CheckerList)) continue;
        if(!checker->second(mtrlValue)) return false;
    }
    
	return true;
}

void CChallengeDataManager::initMaterialKeyList()
{
    auto initChecker = [=](std::string key, const CHECKER& func){
        m_MaterialKeyList.emplace_back(key);
        m_CheckerList.emplace(std::pair<std::string, CHECKER>(key, func));
    };
    
    initChecker(COIN_SCORE,        CC_CALLBACK_1(CChallengeClearChecker::coinScoreCheck        , m_Checker));
    initChecker(STAR_SCORE,        CC_CALLBACK_1(CChallengeClearChecker::starScoreCheck        , m_Checker));
    initChecker(RUN_SCORE,         CC_CALLBACK_1(CChallengeClearChecker::runScoreCheck         , m_Checker));
    initChecker(BEST_SCORE,        CC_CALLBACK_1(CChallengeClearChecker::bestScoreCheck        , m_Checker));
    initChecker(BEST_COMBO,        CC_CALLBACK_1(CChallengeClearChecker::bestComboCheck        , m_Checker));
    
    initChecker(CHARACTER_COLLECT, CC_CALLBACK_1(CChallengeClearChecker::characterCollectCheck , m_Checker));
    initChecker(ROCKET_COLLECT,    CC_CALLBACK_1(CChallengeClearChecker::rocketCollectCheck    , m_Checker));
    
    initChecker(CHARACTER_COUNT,   CC_CALLBACK_1(CChallengeClearChecker::characterCountCheck   , m_Checker));
    initChecker(ROCKET_COUNT,      CC_CALLBACK_1(CChallengeClearChecker::rocketCountCheck      , m_Checker));
    
    initChecker(USER_LEVEL,        CC_CALLBACK_1(CChallengeClearChecker::userLevelCheck        , m_Checker));
    initChecker(COIN_ITEM_LEVEL,   CC_CALLBACK_1(CChallengeClearChecker::coinItemLevelCheck    , m_Checker));
    initChecker(STAR_ITEM_LEVEL,   CC_CALLBACK_1(CChallengeClearChecker::starItemLevelCheck    , m_Checker));
    initChecker(BONUS_ITEM_LEVEL,  CC_CALLBACK_1(CChallengeClearChecker::bonusItemLevelCheck   , m_Checker));
    initChecker(GIANT_ITEM_LEVEL,  CC_CALLBACK_1(CChallengeClearChecker::giantItemLevelCheck   , m_Checker));
    initChecker(MAGNET_ITEM_LEVEL, CC_CALLBACK_1(CChallengeClearChecker::magnetItemLevelCheck  , m_Checker));
    initChecker(MAGNET_SIZE_LEVEL, CC_CALLBACK_1(CChallengeClearChecker::magnetSizeLevelCheck  , m_Checker));
    
    initChecker(GIANT_SCORE_TOTAL,   CC_CALLBACK_1(CChallengeClearChecker::giantScoreTotal     , m_Checker));
    initChecker(GIANT_SCORE_BULLET,  CC_CALLBACK_1(CChallengeClearChecker::giantScoreBullet    , m_Checker));
    initChecker(GIANT_SCORE_MISSILE, CC_CALLBACK_1(CChallengeClearChecker::giantScoreMissile   , m_Checker));
    initChecker(GIANT_SCORE_STICK,   CC_CALLBACK_1(CChallengeClearChecker::giantScoreStick     , m_Checker));
    initChecker(MAGNET_SCORE,        CC_CALLBACK_1(CChallengeClearChecker::magnetScore         , m_Checker));
    initChecker(BARRIER_SCORE,       CC_CALLBACK_1(CChallengeClearChecker::barrierScore        , m_Checker));
    
    initChecker(GIANT_COUNT_TOTAL,   CC_CALLBACK_1(CChallengeClearChecker::giantCountTotal     , m_Checker));
    initChecker(GIANT_COUNT_BULLET,  CC_CALLBACK_1(CChallengeClearChecker::giantCountBullet    , m_Checker));
    initChecker(GIANT_COUNT_MISSILE, CC_CALLBACK_1(CChallengeClearChecker::giantCountMissile   , m_Checker));
    initChecker(GIANT_COUNT_STICK,   CC_CALLBACK_1(CChallengeClearChecker::giantCountStick     , m_Checker));
    initChecker(MAGNET_COUNT,        CC_CALLBACK_1(CChallengeClearChecker::magnetCount         , m_Checker));
    initChecker(BARRIER_COUNT,       CC_CALLBACK_1(CChallengeClearChecker::barrierCount        , m_Checker));
    
    initChecker(COIN_ITEM_USE,       CC_CALLBACK_1(CChallengeClearChecker::coinItemUse         , m_Checker));
    initChecker(STAR_ITEM_USE,       CC_CALLBACK_1(CChallengeClearChecker::starItemUse         , m_Checker));
    initChecker(BONUS_ITEM_USE,      CC_CALLBACK_1(CChallengeClearChecker::bonusItemUse        , m_Checker));
    initChecker(GIANT_ITEM_USE,      CC_CALLBACK_1(CChallengeClearChecker::giantItemUse        , m_Checker));
    initChecker(MAGNET_ITEM_USE,     CC_CALLBACK_1(CChallengeClearChecker::magnetItemUse       , m_Checker));
    initChecker(BARRIER_ITEM_USE,    CC_CALLBACK_1(CChallengeClearChecker::barrierItemUse      , m_Checker));
    
    initChecker(COMBO,               CC_CALLBACK_1(CChallengeClearChecker::comboCheck          , m_Checker));
    initChecker(COIN,                CC_CALLBACK_1(CChallengeClearChecker::coinCheck           , m_Checker));
}

void CChallengeDataManager::initRewardKeyList()
{
    auto initRewarder = [=](std::string key, const REWARDER& func){
        m_RewardKeyList.emplace_back(key);
        m_RewarderList.emplace(std::pair<std::string, REWARDER>(key, func));
    };
    
    initRewarder(REWARD_COIN,        CC_CALLBACK_1(CChallengeRewarder::coinReward, m_Rewarder));
}
