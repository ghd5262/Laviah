#include "ChallengeDataManager.hpp"
#include "UserDataManager.h"
#include "CharacterDataManager.h"
#include "ChallengeChecker/ChallengeClearChecker.h"
#include "ChallengeRewarder/ChallengeRewarder.hpp"
#include "../json/json.h"
#include "../Common/StringUtility.h"

using namespace cocos2d;
using namespace CHALLENGE_DATA_KEY;
using namespace CHALLENGE_REWARD_KEY;

CChallengeDataManager::CChallengeDataManager()
: m_Checker(new CChallengeClearChecker())
, m_Rewarder(new CChallengeRewarder())
{
	initETCChekerList();
	initRewarderList();
    initWithJson("challengeList.json");
}

CChallengeDataManager::~CChallengeDataManager()
{
	auto cleanList = [=](CHALLENGE_LIST &list){
		for (auto data : list)
		{
			delete data.second;
			data.second = nullptr;
		}
		list.clear();
	};
    
    cleanList(m_NormalChallengeDataList);
    
    CC_SAFE_DELETE(m_Checker);
    CC_SAFE_DELETE(m_Rewarder);
}

CChallengeDataManager* CChallengeDataManager::Instance()
{
    static CChallengeDataManager instance;
    return &instance;
}

void CChallengeDataManager::initWithJson(std::string fileName)
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
    
	m_NormalChallengeDefaultSet				= root["normalChallengeDefaultSet"];
	m_HiddenChallengeDefaultSet				= root["hiddenChallengeDefaultSet"];
    const Json::Value normalChallengeArray	= root["normalChallenges"];
	const Json::Value hiddenChallengeArray  = root["hiddenChallenges"];

	for (unsigned int count = 0; count < normalChallengeArray.size(); ++count)
    {
		const Json::Value valueItem = normalChallengeArray[count];
		this->addChallengeToList(m_NormalChallengeDataList, valueItem, false);
    }

	for (unsigned int count = 0; count < hiddenChallengeArray.size(); ++count)
	{
		const Json::Value valueItem = hiddenChallengeArray[count];
		this->addChallengeToList(m_HiddenChallengeDataList, valueItem, true);
	}
}

bool CChallengeDataManager::CheckCompleteAll()
{
	auto currentList = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHALLENGE_CUR_LIST);
    if(currentList.size() <= 0) {
        /** If there are no challenges. get new challenges.
         *  If there are no non-complete challenges. Nothing do. */
        this->getNewChallenges();
        return false;
    }
    
	bool result = true;
	for (auto index : currentList)
		if (!CheckChallengeComplete(index, false)) result = false;

	return result;
}

bool CChallengeDataManager::CheckChallengeComplete(int index, bool isHidden)
{
	std::string		userDataKey = USERDATA_KEY::CHALLENGE_COM_NORMAL_LIST;
	if (isHidden)	userDataKey = USERDATA_KEY::CHALLENGE_COM_HIDDEN_LIST;
	if (CUserDataManager::Instance()->getUserData_IsItemHave(userDataKey, index)) return true;

	auto challengeData	= this->getChallengeByIndex(index);
	auto keyList		= challengeData->_materialKeyList;
	auto mtrlValueList	= challengeData->_materialValueList;
	auto checkerType	= challengeData->_checkerType;
	for (int idx = 0; idx < keyList.size(); idx++)
	{
		auto key		= keyList.at(idx);
		auto mtrlValue	= mtrlValueList.at(idx);
		auto checker	= m_CheckerList.find(key);

		switch (checkerType){
		case CHECKER_TYPE::ETC:
			if (checker == m_CheckerList.end()) {
				CCLOG("There is no checker with key %s", key.c_str());
				CCASSERT(false, "There is no checker with key");
				return false;
			}
			if (!checker->second(mtrlValue)) return false;
			break;
		case CHECKER_TYPE::GLOBAL_DATA: if (!m_Checker->checkWithGlobal(key, mtrlValue))		return false; break;
		case CHECKER_TYPE::SINGLE_DATA: if (!m_Checker->checkWithSingleUserData(key, mtrlValue))return false; break;
		case CHECKER_TYPE::ITEM_EXIST:	if (!m_Checker->checkWithItemExist(key, mtrlValue))		return false; break;
		case CHECKER_TYPE::ITEM_COUNT:	if (!m_Checker->checkWithCount(key, mtrlValue))			return false; break;
		case CHECKER_TYPE::CONTINUE:	CCLOG("Developed yet."); break;
		}
	}

	CUserDataManager::Instance()->setUserData_ItemGet(userDataKey, index);
	if (!isHidden) GLOBAL->CHALLENGE_CLEAR_COUNT += 1;
	
	return true;
}

const sCHALLENGE_PARAM* CChallengeDataManager::CompleteCheckRealTime()
{
	auto currentList = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHALLENGE_CUR_LIST);
	if (currentList.size() <= 0) return nullptr;
	
	for (auto index : currentList)
	{
		auto originCount = GLOBAL->CHALLENGE_CLEAR_COUNT;

		if (this->CheckChallengeComplete(index, false)){
			if (originCount < GLOBAL->CHALLENGE_CLEAR_COUNT)
			{
				return this->getChallengeByIndex(index);
			}
		}
	}

	return nullptr;
}

sREWARD_DATA CChallengeDataManager::Reward(int index)
{
    auto challengeData = getChallengeByIndex(index);
    
    auto key = challengeData->_rewardKey;
    auto rewardValue = challengeData->_rewardValue;
    
	return this->RewardByKey(key, rewardValue);
}

sREWARD_DATA CChallengeDataManager::RewardByKey(std::string key, int value)
{
	auto rewarder = m_RewarderList.find(key);
	if (rewarder == std::end(m_RewarderList)) return sREWARD_DATA();

	return rewarder->second(sREWARD_DATA(key, value));
}

int CChallengeDataManager::NonCompleteChallengeExist()
{
    return getNonCompletedChallengeList().size();
}

void CChallengeDataManager::getNewChallenges()
{
    auto list = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHALLENGE_CUR_LIST);
    auto nonCompleteList = getNonCompletedChallengeList();
    
    // Challenges not completed are less than limit count.
    // then remove current list only.
    if(nonCompleteList.size() < CHALLENGE_DEFINE::LIMIT_COUNT)
    {
		for (int index = 0; index < list.size(); index++)
			this->removeChallengeFromUserData(list.at(index));
        
        return;
    }
    
    // Current challenges are less than limit count.
    // then get new challenges until limit count.
	if (list.size() < CHALLENGE_DEFINE::LIMIT_COUNT)
    {
		for (int count = 0; count < CHALLENGE_DEFINE::LIMIT_COUNT - list.size(); count++)
            this->getNewRandomChallenge();
        
        return;
    }
    
    // There are enough challenges to skip.
	for (int count = 0; count < CHALLENGE_DEFINE::LIMIT_COUNT; count++)
    {
		this->SkipChallenge(list.at(count));
    }
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
    
	auto newChallenge = this->getNewRandomChallenge();
	this->removeChallengeFromUserData(index);

//    if(newChallenge->_continuingType){
//        
//    }
    
	return newChallenge;
}

const sCHALLENGE_PARAM* CChallengeDataManager::getChallengeByIndex(int index) const
{
    if (m_NormalChallengeDataList.size() <= index) {
        CCLOG("Wrong index : %d", index);
        CCASSERT(false, "Wrong index");
        return nullptr;
    }
    return m_NormalChallengeDataList.at(index);
}

const sCHALLENGE_PARAM* CChallengeDataManager::getNewRandomChallenge()
{
    auto newList = getNonCompletedChallengeList();
    auto newChallenge = getNewRandomChallengeFromList(newList);
    CCLOG("Get new challenge %d", newChallenge->_index);
    CUserDataManager::Instance()->setUserData_ItemGet(USERDATA_KEY::CHALLENGE_CUR_LIST, newChallenge->_index);
    return newChallenge;
}

CHALLENGE_LIST CChallengeDataManager::getListByFunc(const CHALLENGE_PICK &func, 
													CHALLENGE_LIST list)
{
	for (auto iter = list.begin(); iter != list.end();)
	{
		auto &item = (*iter).second;
		if (item != nullptr){

			if (!func(item)) iter = list.erase(iter);
			else             iter++;
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
    
    CCLOG("Pick a challenge :: idx %d content %s",
          picked->_index,
          picked->_contents.c_str());
    
    return picked;
}

CHALLENGE_LIST CChallengeDataManager::getNonCompletedChallengeList()
{
    auto userDataMng = CUserDataManager::Instance();
    
    return getListByFunc([=](const sCHALLENGE_PARAM* data){
        
        if (userDataMng->getUserData_IsItemHave(USERDATA_KEY::CHALLENGE_COM_NORMAL_LIST, data->_index)) return false;
		if (userDataMng->getUserData_IsItemHave(USERDATA_KEY::CHALLENGE_CUR_LIST, data->_index)) return false;
		if (data->_isHighLevel) return false;
        return true;
    }, m_NormalChallengeDataList);
}

void CChallengeDataManager::removeChallengeFromUserData(int index)
{
    CCLOG("Remove challenge %d", index);
    CUserDataManager::Instance()->setUserData_ItemRemove(USERDATA_KEY::CHALLENGE_CUR_LIST, index);
}

void CChallengeDataManager::initETCChekerList()
{
    auto initChecker = [=](std::string key, const CHECKER& func){
        m_CheckerList.emplace(std::pair<std::string, CHECKER>(key, func));
    };
   
	initChecker(CHARACTER_RARE_COUNT,CC_CALLBACK_1(CChallengeClearChecker::characterRareCountCheck, m_Checker));
    initChecker(ROCKET_RARE_COUNT,   CC_CALLBACK_1(CChallengeClearChecker::rocketRareCountCheck   , m_Checker));
}

void CChallengeDataManager::initRewarderList()
{
    auto initRewarder = [=](std::string key, const REWARDER& func){
        m_RewarderList.emplace(std::pair<std::string, REWARDER>(key, func));
    };
    
   	initRewarder(REWARD_COIN,			  CC_CALLBACK_1(CChallengeRewarder::coinReward				, m_Rewarder));
	initRewarder(REWARD_CHARACTER,		  CC_CALLBACK_1(CChallengeRewarder::characterReward			, m_Rewarder));
	initRewarder(REWARD_ROCKET,			  CC_CALLBACK_1(CChallengeRewarder::RocketReward			, m_Rewarder));
	initRewarder(REWARD_PET,			  CC_CALLBACK_1(CChallengeRewarder::PetReward				, m_Rewarder));

	initRewarder(REWARD_COIN_RANDOM,	  CC_CALLBACK_1(CChallengeRewarder::coinRewardRandom		, m_Rewarder));
	initRewarder(REWARD_CHARACTER_RANDOM, CC_CALLBACK_1(CChallengeRewarder::characterRewardRandom	, m_Rewarder));
	initRewarder(REWARD_ROCKET_RANDOM,	  CC_CALLBACK_1(CChallengeRewarder::RocketRewardRandom		, m_Rewarder));
	initRewarder(REWARD_PET_RANDOM,		  CC_CALLBACK_1(CChallengeRewarder::PetRewardRandom			, m_Rewarder));
}

void CChallengeDataManager::initMaterialValueListByUserData(sCHALLENGE_PARAM* data)
{
	for (auto key : data->_materialKeyList)
	{
		int value = 0;
		if     ( key == CHALLENGE_DATA_KEY::TOTAL_SCORE           ) value = 0;
		else if( key == CHALLENGE_DATA_KEY::COIN_SCORE            ) value = 0;
		else if( key == CHALLENGE_DATA_KEY::STAR_SCORE            ) value = 0;
		else if( key == CHALLENGE_DATA_KEY::RUN_SCORE             ) value = 0;
		else if( key == CHALLENGE_DATA_KEY::COIN_COUNT		      ) value = 0;
		else if( key == CHALLENGE_DATA_KEY::STAR_COUNT		      ) value = 0;
		else if( key == CHALLENGE_DATA_KEY::GIANT_SCORE_TOTAL     ) value = 0;
		else if( key == CHALLENGE_DATA_KEY::GIANT_SCORE_BULLET    ) value = 0;
		else if( key == CHALLENGE_DATA_KEY::GIANT_SCORE_MISSILE   ) value = 0;
		else if( key == CHALLENGE_DATA_KEY::GIANT_SCORE_STICK     ) value = 0;
		else if( key == CHALLENGE_DATA_KEY::MAGNET_SCORE          ) value = 0;
		else if( key == CHALLENGE_DATA_KEY::GIANT_COUNT_TOTAL     ) value = 0;
		else if( key == CHALLENGE_DATA_KEY::GIANT_COUNT_BULLET    ) value = 0;
		else if( key == CHALLENGE_DATA_KEY::GIANT_COUNT_MISSILE   ) value = 0;
		else if( key == CHALLENGE_DATA_KEY::GIANT_COUNT_STICK     ) value = 0;
		else if( key == CHALLENGE_DATA_KEY::MAGNET_COUNT          ) value = 0;

		data->_materialValueList.emplace_back(value);
	}
}

const Json::Value CChallengeDataManager::initChallengeWithDefaultValue(bool hidden,
																	   std::string key, 
																	   const Json::Value data)
{
	if (data.isNull())
	{
		if (!hidden)	 return m_NormalChallengeDefaultSet[key.c_str()];
		else			 return m_HiddenChallengeDefaultSet[key.c_str()];
	}
	return data;
}

void CChallengeDataManager::addChallengeToList(CHALLENGE_LIST &list, 
											   const Json::Value& data, 
											   bool hiddenType)
{
	sCHALLENGE_PARAM* param = new sCHALLENGE_PARAM();

	param->_index			= data["index"].asInt();
	param->_hiddenType		= hiddenType;

	std::string content		= StringUtils::format(CHALLENGE_DEFINE::NORMAL_CONTENT.c_str(), param->_index);
	if (hiddenType) content = StringUtils::format(CHALLENGE_DEFINE::NORMAL_HIDDEN.c_str(), param->_index);
	param->_contents		= content;

	auto initData = [=](std::string key){
		return this->initChallengeWithDefaultValue(param->_hiddenType, key, data[key.c_str()]);
	};
	param->_checkerType		= (CHECKER_TYPE)initData("checkerType").asInt();
	param->_rewardKey		= initData("rewardKey").asString();
	param->_rewardValue		= initData("rewardValue").asInt();
	param->_continuingType	= initData("continuingType").asBool();
	param->_visible			= initData("visible").asBool();

	const Json::Value materialKeyArray   = data["materialKey"];
	const Json::Value materialValueArray = data["materialValue"];

	if (hiddenType) {
		for (auto key : materialKeyArray)
			param->_materialKeyList.emplace_back(key.asString());
		for (auto value : materialValueArray)
			param->_materialValueList.emplace_back(value.asInt());
	}
	else {
		param->_materialKeyList.emplace_back(materialKeyArray.asString());
		if (materialValueArray.isNull()) this->initMaterialValueListByUserData(param);
		else param->_materialValueList.emplace_back(materialValueArray.asInt());
	}
	list.emplace(std::pair<int, const sCHALLENGE_PARAM*>(param->_index, param));
}

cocos2d::Sprite* CChallengeDataManager::getRewardSprite(std::string rewardKey, 
														int rewardValue)
{
    auto createTitle = [=](std::string title, Sprite* parent){
        auto label  = Label::createWithTTF(title, FONT::MALGUNBD, 50);
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        label->setPosition(Vec2(parent->getContentSize().width * 0.5f,
                                parent->getContentSize().height * 0.9f));
        label->setColor(COLOR::DARKGRAY);
        parent->addChild(label);
    };
    
    Sprite* sprite = nullptr;
    
    if (REWARD_COIN == rewardKey){
        sprite = Sprite::create("workshopCoinTime.png");
        createTitle(StringUtils::format("%d", rewardValue), sprite);
        
    }
	if (REWARD_CHARACTER == rewardKey){
		auto data = CCharacterDataManager::Instance()->getCharacterByIndex(rewardValue);
        sprite = Sprite::createWithSpriteFrameName(data->_normalTextureName);
        createTitle(StringUtils::format("%s", data->_name.c_str()), sprite);
	}
    sprite->setScale(1.5f);
    return sprite;
}