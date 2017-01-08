#include "ChallengeDataManager.hpp"
#include "UserDataManager.h"
#include "ChallengeChecker/ChallengeClearChecker.h"
#include "ChallengeRewarder/ChallengeRewarder.hpp"
#include "../json/json.h"
#include "../Common/StringUtility.h"

using namespace cocos2d;
using namespace CHALLENGE_DATA_KEY;


CChallengeDataManager::CChallengeDataManager()
: m_Checker(new CChallengeClearChecker())
, m_Rewarder(new CChallengeRewarder())
{
	initMaterialKeyList();
	initRewardKeyList();
    initWithJson(m_ChallengeDataList, "challengeList_1.json");
	initWithJson(m_ChallengeDataList, "challengeList_2.json");
	initWithJson(m_ChallengeDataList, "challengeList_3.json");
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
    
    cleanList(m_ChallengeDataList);
    
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

    for (unsigned int count = 0; count < challengeArray.size(); ++count)
    {
        const Json::Value challenge = challengeArray[count];
        
        sCHALLENGE_PARAM* challengeInfo = new sCHALLENGE_PARAM();
        
        challengeInfo->_index               = int(list.size());
        challengeInfo->_level               = challenge["level"].asInt();
        challengeInfo->_continuingType      = challenge["continuingType"].asBool();
        challengeInfo->_contents            = challenge["contents"].asString();
        challengeInfo->_materialKey         = challenge["materialKey"].asString();
        challengeInfo->_rewardKey           = challenge["rewardKey"].asString();
        challengeInfo->_materialValue       = challenge["materialValue"].asInt();
        challengeInfo->_rewardValue         = challenge["rewardValue"].asInt();

        list.emplace_back(challengeInfo);
    }
}

bool CChallengeDataManager::CheckCompleteAll()
{
	auto currentList = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHALLENGE_CUR_LIST);
	for (auto index : *currentList)
		if (!CheckChallengeComplete(index)) return false;

	return true;
}

bool CChallengeDataManager::CheckChallengeComplete(int index)
{
	if (CUserDataManager::Instance()->getUserData_IsItemHave(USERDATA_KEY::CHALLENGE_COM_LIST, index))
		return true;

	auto challengeData = this->getChallengeByIndex(index);

	//    if(challengeData->_continuingType)
	//    {
	//        auto sequence = CUserDataManager::getUserDataSequenceFromList(USERDATA_KEY::CHALLENGE_CUR_LIST, index);
	//        if(m_Checker->continuingTypeCheck(int(sequence))) return true;
	//        return false;
	//    }

	auto key = challengeData->_materialKey;
	auto mtrlValue = challengeData->_materialValue;

	auto checker = m_CheckerList.find(key);
	if (checker == std::end(m_CheckerList)){
		if (!m_Checker->checkWithGlobal(key, mtrlValue)) return false;

		GLOBAL->CHALLENGE_CLEAR_COUNT += 1;
		CUserDataManager::Instance()->setUserData_ItemGet(USERDATA_KEY::CHALLENGE_COM_LIST, index);
		return true;
	}

	if (!checker->second(mtrlValue)) return false;

	GLOBAL->CHALLENGE_CLEAR_COUNT += 1;
	CUserDataManager::Instance()->setUserData_ItemGet(USERDATA_KEY::CHALLENGE_COM_LIST, index);
	return true;
}

void CChallengeDataManager::Reward(int index)
{
    auto challengeData = getChallengeByIndex(index);
    
    auto key = challengeData->_rewardKey;
    auto rewardValue = challengeData->_rewardValue;
    
	this->RewardByKey(key, rewardValue);
}

void CChallengeDataManager::RewardByKey(std::string key, int value)
{
	auto rewarder = m_RewarderList.find(key);
	if (rewarder == std::end(m_RewarderList)) return;

	rewarder->second(value);
}

bool CChallengeDataManager::NonCompleteChallengeExist(int level,
                                                      bool below,
                                                      bool continuingType/* = false*/)
{
    return getNonCompletedChallengeList(level, below, continuingType).size();
}

const sCHALLENGE_PARAM* CChallengeDataManager::SkipChallenge(int index)
{
/** continuing type 은 기존 로직으로 처리하지 못한다.
 *  기존 로직으로 구글 클라우드 데이터에 저장해 보려했지만 
 *  현재 로직으로는 CHALLENGE_CUR_LIST와 CHALLENGE_CUR_VALUE_LIST 를 1 : 1 대응 하지 못한다.
 *  ( CHALLENGE_CUR_LIST 중 3번째가 continuing type이면 sequence가 2이지만
 *  CHALLENGE_CUR_VALUE_LIST에서는 첫번째 이기 때문)
 *  후에 challenge용으로 클라우드 데이터 저장을 따로 해야할 것 같다.
 */
//    auto challengeData = getChallengeByIndex(index);
//    if(challengeData->_continuingType){
//        auto sequence = CUserDataManager::getUserDataSequenceFromList(USERDATA_KEY::CHALLENGE_CUR_LIST, index);
//        auto savedDataList = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHALLENGE_CUR_VALUE_LIST);
//        auto savedData = savedDataList->at(sequence);
//        CUserDataManager::Instance()->setUserData_ItemRemove(USERDATA_KEY::CHALLENGE_CUR_VALUE_LIST, savedData);
//    }
    
    CCLOG("Skip challenge %d", index);
	CUserDataManager::Instance()->setUserData_ItemRemove(USERDATA_KEY::CHALLENGE_CUR_LIST, index);
	auto newChallenge = this->getNewRandomChallenge(1, false);
    CCLOG("Get new challenge %d", newChallenge->_index);
	CUserDataManager::Instance()->setUserData_ItemGet(USERDATA_KEY::CHALLENGE_CUR_LIST, newChallenge->_index);
    
//    if(newChallenge->_continuingType){
//        
//    }
    
	return newChallenge;
}

const sCHALLENGE_PARAM* CChallengeDataManager::getChallengeByIndex(int index) const
{
    if (m_ChallengeDataList.size() <= index) {
        CCLOG("Wrong index : %d", index);
        CCASSERT(false, "Wrong index");
        return nullptr;
    }
    return m_ChallengeDataList.at(index);
}

const sCHALLENGE_PARAM* CChallengeDataManager::getNewRandomChallenge(int level,
                                                                     bool below,
                                                                     bool continuingType/* = false*/)
{
    auto newList = getNonCompletedChallengeList(level, below, continuingType);
    return getNewRandomChallengeFromList(newList);
}

CHALLENGE_LIST CChallengeDataManager::getListByFunc(const CHALLENGE_PICK &func, CHALLENGE_LIST list)
{
    for(auto iter = list.begin(); iter != list.end(); )
    {
        auto &item = (*iter);
        if(item != nullptr){
            
            if(!func(item)) iter = list.erase(iter);
            else            iter++;
        }
    }
    
    return list;
}

const sCHALLENGE_PARAM* CChallengeDataManager::getNewRandomChallengeFromList(CHALLENGE_LIST &list)
{
    auto size = list.size();
    if(size <= 0) {
        CCLOG("There is no challenge anymore.");
        return nullptr;
    }
    const sCHALLENGE_PARAM* picked;
    auto randomIdx = random<int>(0, int(size) - 1);
    picked = list.at(randomIdx);
    
    CCLOG("Pick a challenge :: idx %d content %s level %d",
          picked->_index,
          picked->_contents.c_str(),
          picked->_level);
    
    return picked;
}

CHALLENGE_LIST CChallengeDataManager::getNonCompletedChallengeList(int level,
                                                                   bool below,
                                                                   bool continuingType/* = false*/)
{
    auto userDataMng = CUserDataManager::Instance();
    
    return getListByFunc([=](const sCHALLENGE_PARAM* data){
        
        if(userDataMng->getUserData_IsItemHave(USERDATA_KEY::CHALLENGE_COM_LIST, data->_index)) return false;
        if(userDataMng->getUserData_IsItemHave(USERDATA_KEY::CHALLENGE_CUR_LIST, data->_index)) return false;
        
        if(below)           { if(data->_level > level) return false;   }
        else                { if(data->_level != level) return false;  }
        
        if(continuingType)  { if(!data->_continuingType) return false; }
        
        return true;
    }, m_ChallengeDataList);
}

void CChallengeDataManager::initMaterialKeyList()
{
    auto initChecker = [=](std::string key, const CHECKER& func){
        m_CheckerList.emplace(std::pair<std::string, CHECKER>(key, func));
    };
    
    initChecker(BEST_SCORE,          CC_CALLBACK_1(CChallengeClearChecker::bestScoreCheck        , m_Checker));
    initChecker(BEST_COMBO,          CC_CALLBACK_1(CChallengeClearChecker::bestComboCheck        , m_Checker));
    
    initChecker(CHARACTER_COLLECT,   CC_CALLBACK_1(CChallengeClearChecker::characterCollectCheck , m_Checker));
    initChecker(ROCKET_COLLECT,      CC_CALLBACK_1(CChallengeClearChecker::rocketCollectCheck    , m_Checker));
    
    initChecker(CHARACTER_COUNT,     CC_CALLBACK_1(CChallengeClearChecker::characterCountCheck   , m_Checker));
    initChecker(ROCKET_COUNT,        CC_CALLBACK_1(CChallengeClearChecker::rocketCountCheck      , m_Checker));
    
    initChecker(USER_LEVEL,          CC_CALLBACK_1(CChallengeClearChecker::userLevelCheck        , m_Checker));
    initChecker(COIN_ITEM_LEVEL,     CC_CALLBACK_1(CChallengeClearChecker::coinItemLevelCheck    , m_Checker));
    initChecker(STAR_ITEM_LEVEL,     CC_CALLBACK_1(CChallengeClearChecker::starItemLevelCheck    , m_Checker));
    initChecker(BONUS_ITEM_LEVEL,    CC_CALLBACK_1(CChallengeClearChecker::bonusItemLevelCheck   , m_Checker));
    initChecker(GIANT_ITEM_LEVEL,    CC_CALLBACK_1(CChallengeClearChecker::giantItemLevelCheck   , m_Checker));
    initChecker(MAGNET_ITEM_LEVEL,   CC_CALLBACK_1(CChallengeClearChecker::magnetItemLevelCheck  , m_Checker));
    initChecker(MAGNET_SIZE_LEVEL,   CC_CALLBACK_1(CChallengeClearChecker::magnetSizeLevelCheck  , m_Checker));
    initChecker(COIN,                CC_CALLBACK_1(CChallengeClearChecker::coinCheck             , m_Checker));
}

void CChallengeDataManager::initRewardKeyList()
{
    auto initRewarder = [=](std::string key, const REWARDER& func){
        m_RewarderList.emplace(std::pair<std::string, REWARDER>(key, func));
    };
    
    initRewarder(REWARD_COIN,        CC_CALLBACK_1(CChallengeRewarder::coinReward, m_Rewarder));
}

