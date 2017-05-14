#include "AchievementDataManager.hpp"
#include "CharacterDataManager.h"
#include "DataManagerUtils.h"
#include "AchievementChecker/AchievementClearChecker.h"
#include "AchievementRewarder/AchievementRewarder.hpp"
#include "../json/json.h"
#include "../Common/StringUtility.h"

using namespace cocos2d;
using namespace ACHIEVEMENT_DATA_KEY;
using namespace ACHIEVEMENT_REWARD_KEY;

CAchievementDataManager::CAchievementDataManager()
: m_Checker(new CAchievementClearChecker())
, m_Rewarder(new CAchievementRewarder())
{
	initETCChekerList();
	initRewarderList();
    initWithJson("achievementList.json");
}

CAchievementDataManager::~CAchievementDataManager()
{
    DATA_MANAGER_UTILS::mapDeleteAndClean(m_NormalAchievementDataList);
    
    CC_SAFE_DELETE(m_Checker);
    CC_SAFE_DELETE(m_Rewarder);
}

CAchievementDataManager* CAchievementDataManager::Instance()
{
    static CAchievementDataManager instance;
    return &instance;
}

void CAchievementDataManager::initWithJson(std::string fileName)
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
    CCLOG("Achievement List JSON : \n %s\n", fileData.c_str());
    
    const Json::Value materialValueArray    = root["materialValueList"];
    
    for (unsigned int count = 0; count < materialValueArray.size(); ++count)
    {
        const Json::Value valueItem = materialValueArray[count];
        this->addValueListToMap(valueItem);
    }
    
	m_NormalAchievementDefaultSet				= root["normalAchievementDefaultSet"];
	m_HiddenAchievementDefaultSet				= root["hiddenAchievementDefaultSet"];
    const Json::Value normalAchievementArray	= root["normalAchievements"];
	const Json::Value hiddenAchievementArray    = root["hiddenAchievements"];

    auto level = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::LEVEL);
//    MessageBox(StringUtils::format("Get material value from data of %d level.", level).c_str(), "NOTICE");
    
	for (unsigned int count = 0; count < normalAchievementArray.size(); ++count)
    {
		const Json::Value valueItem = normalAchievementArray[count];
		this->addAchievementToList(m_NormalAchievementDataList, valueItem, false);
    }

	for (unsigned int count = 0; count < hiddenAchievementArray.size(); ++count)
	{
		const Json::Value valueItem = hiddenAchievementArray[count];
		this->addAchievementToList(m_HiddenAchievementDataList, valueItem, true);
	}
}

bool CAchievementDataManager::CheckCompleteAll()
{
	auto currentList = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::ACHIEVEMENT_CUR_NORMAL_LIST);
    if(currentList.size() <= 0) {
        /** If there are no achievements. get new achievements.
         *  If there are no non-complete achievements. Nothing do. */
        this->getNewAchievements();
        return false;
    }
    
	bool result = true;
	for (auto index : currentList)
		if (!CheckAchievementComplete(index, false)) result = false;

	return result;
}

bool CAchievementDataManager::CheckAchievementComplete(int index, bool isHidden)
{
	std::string		userDataKey = USERDATA_KEY::ACHIEVEMENT_COM_NORMAL_LIST;
	if (isHidden)	userDataKey = USERDATA_KEY::ACHIEVEMENT_COM_HIDDEN_LIST;
	if (CUserDataManager::Instance()->getUserData_IsItemHave(userDataKey, index)) return true;
    
    const sACHIEVEMENT_PARAM* achievementData = nullptr;
    if (isHidden) achievementData = this->getHiddenAchievementByIndex(index);
    else          achievementData = this->getNormalAchievementByIndex(index);
    
    auto keyList		= achievementData->_materialKeyList;
	auto mtrlValueList	= achievementData->_materialValueList;
	auto checkerType	= achievementData->_checkerType;
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
            case CHECKER_TYPE::GLOBAL_DATA:
                if (!m_Checker->checkWithGlobal(key, mtrlValue))
                    return false; else break;
                
            case CHECKER_TYPE::SINGLE_DATA:
                if (!m_Checker->checkWithSingleUserData(key, mtrlValue))
                    return false; else break;
                
            case CHECKER_TYPE::ITEM_EXIST:
                if (!m_Checker->checkWithItemExist(key, mtrlValue))
                    return false; else break;
                
            case CHECKER_TYPE::ITEM_COUNT:
                if (!m_Checker->checkWithCount(key, mtrlValue))
                    return false; else break;
                
            case CHECKER_TYPE::CONTINUE:
                if (!m_Checker->checkWithContinuingType(key, mtrlValue))
                    return false; else break;
		}
	}

	CUserDataManager::Instance()->setUserData_ItemGet(userDataKey, index);
	if (isHidden) GLOBAL->HIDDEN_ACHIEVEMENT_CLEAR_COUNT += 1;
    else          GLOBAL->NORMAL_ACHIEVEMENT_CLEAR_COUNT += 1;
	
	return true;
}

const sACHIEVEMENT_PARAM* CAchievementDataManager::CompleteCheckRealTime(bool isHidden)
{
    ARRAY_DATA currentList;
    this->getCurAchievementListByType(currentList, isHidden);

    if (currentList.size() <= 0) return nullptr;
	
	for (auto index : currentList)
	{
		auto normalOriginCount = GLOBAL->NORMAL_ACHIEVEMENT_CLEAR_COUNT;
        auto hiddenOriginCount = GLOBAL->HIDDEN_ACHIEVEMENT_CLEAR_COUNT;
		if (this->CheckAchievementComplete(index, isHidden)){
			if (normalOriginCount < GLOBAL->NORMAL_ACHIEVEMENT_CLEAR_COUNT)
                return this->getNormalAchievementByIndex(index);
			if (hiddenOriginCount < GLOBAL->HIDDEN_ACHIEVEMENT_CLEAR_COUNT)
                return this->getHiddenAchievementByIndex(index);
		}
	}

	return nullptr;
}

void CAchievementDataManager::ResetNormalAchievements()
{
    CUserDataManager::Instance()->setUserData_ItemRemoveAll(USERDATA_KEY::ACHIEVEMENT_COM_NORMAL_LIST);
}

sREWARD_DATA CAchievementDataManager::Reward(int index)
{
    auto achievementData = getNormalAchievementByIndex(index);
    
    auto key = achievementData->_rewardKey;
    auto rewardValue = achievementData->_rewardValue;
    
	return this->RewardByKey(key, rewardValue);
}

sREWARD_DATA CAchievementDataManager::RewardByKey(std::string key, int value)
{
	auto rewarder = m_RewarderList.find(key);
	if (rewarder == std::end(m_RewarderList)) return sREWARD_DATA();

	return rewarder->second(sREWARD_DATA(key, value)); // call function by reward type.
}

int CAchievementDataManager::NonCompleteAchievementExist()
{
    return getNonCompletedAchievementList().size();
}

void CAchievementDataManager::getNewAchievements()
{
    auto list = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::ACHIEVEMENT_CUR_NORMAL_LIST);
    auto nonCompleteList = getNonCompletedAchievementList();
    
    // Achievements not completed are less than limit count.
    // then remove current list only.
    if(nonCompleteList.size() < ACHIEVEMENT_DEFINE::LIMIT_COUNT)
    {
		for (int index = 0; index < list.size(); index++)
			this->removeAchievementFromUserData(list.at(index));
        
        return;
    }
    
    // Current achievements are less than limit count.
    // then get new achievements until limit count.
	if (list.size() < ACHIEVEMENT_DEFINE::LIMIT_COUNT)
    {
		for (int count = 0; count < ACHIEVEMENT_DEFINE::LIMIT_COUNT - list.size(); count++)
            this->getNewRandomAchievement();
        
        return;
    }
    
    // There are enough achievements to skip.
	for (int count = 0; count < ACHIEVEMENT_DEFINE::LIMIT_COUNT; count++)
    {
		this->SkipAchievement(list.at(count));
    }
}

const sACHIEVEMENT_PARAM* CAchievementDataManager::SkipAchievement(int index)
{
    auto newAchievement = this->getNewRandomAchievement();
	this->removeAchievementFromUserData(index);

	return newAchievement;
}

const sACHIEVEMENT_PARAM* CAchievementDataManager::getNormalAchievementByIndex(int index) const
{
    auto data = m_NormalAchievementDataList.find(index);
    if(data == m_NormalAchievementDataList.end()) {
        CCLOG("Wrong character index : %d", index);
        CCASSERT(false, "Wrong character index");
        return nullptr;
    }
    return data->second;
}

const sACHIEVEMENT_PARAM* CAchievementDataManager::getHiddenAchievementByIndex(int index) const
{
    auto data = m_HiddenAchievementDataList.find(index);
    if(data == m_HiddenAchievementDataList.end()) {
        CCLOG("Wrong character index : %d", index);
        CCASSERT(false, "Wrong character index");
        return nullptr;
    }
    return data->second;
}

const sACHIEVEMENT_PARAM* CAchievementDataManager::getNewRandomAchievement()
{
    auto newList = getNonCompletedAchievementList();
    auto newAchievement = getNewRandomAchievementFromList(newList);
    CCLOG("Get new achievement %d", newAchievement->_index);
    CUserDataManager::Instance()->setUserData_ItemGet(USERDATA_KEY::ACHIEVEMENT_CUR_NORMAL_LIST, newAchievement->_index);
    return newAchievement;
}

const sACHIEVEMENT_PARAM* CAchievementDataManager::getNonCompleteAchievementFromCurrentList()
{
    auto dataMNG = CUserDataManager::Instance();
    auto currentList = dataMNG->getUserData_List(USERDATA_KEY::ACHIEVEMENT_CUR_NORMAL_LIST);
    for(auto index : currentList)
    {
        if(!dataMNG->getUserData_IsItemHave(USERDATA_KEY::ACHIEVEMENT_COM_NORMAL_LIST, index))
            return getNormalAchievementByIndex(index);
    }
    
    return nullptr;
}

void CAchievementDataManager::UpdateCurHiddenAchievementList()
{
    auto currentList  = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::ACHIEVEMENT_CUR_HIDDEN_LIST);
    auto completeList = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::ACHIEVEMENT_COM_HIDDEN_LIST);
    
    // newList = new achievements ( no complete, no current )
    auto newList      = DATA_MANAGER_UTILS::getMapByFunc([=](const sACHIEVEMENT_PARAM* data){
        bool isExist = false;
        int  newIndex= data->_index;
        for(auto index : currentList)
            if(index == newIndex) isExist = true;
        for(auto index : completeList)
            if(index == newIndex) isExist = true;
        
        return (isExist == false);
    }, m_HiddenAchievementDataList);
    
    if(!newList.size()) return;
    
    for(auto data : newList)
        CUserDataManager::Instance()->setUserData_ItemGet(USERDATA_KEY::ACHIEVEMENT_CUR_HIDDEN_LIST, (data.second)->_index);
}

const sACHIEVEMENT_PARAM* CAchievementDataManager::getNewRandomAchievementFromList(ACHIEVEMENT_LIST &list)
{
    auto size = list.size();
    if(size <= 0) {
        CCLOG("There is no achievement anymore.");
        return nullptr;
    }
    auto picked = list.begin();
    std::advance(picked, random<int>(0, int(list.size()-1)));
    
    CCLOG("Pick a achievement :: idx %d content %s",
          (picked->second)->_index,
          (picked->second)->_contents.c_str());
    
    return (picked->second);
}

ACHIEVEMENT_LIST CAchievementDataManager::getNonCompletedAchievementList()
{
    auto userDataMng = CUserDataManager::Instance();
    
    return DATA_MANAGER_UTILS::getMapByFunc([=](const sACHIEVEMENT_PARAM* data){
        
        if (userDataMng->getUserData_IsItemHave(USERDATA_KEY::ACHIEVEMENT_COM_NORMAL_LIST, data->_index)) return false;
		if (userDataMng->getUserData_IsItemHave(USERDATA_KEY::ACHIEVEMENT_CUR_NORMAL_LIST, data->_index)) return false;
		if (data->_isHighLevel) return false;
        return true;
    }, m_NormalAchievementDataList);
}

void CAchievementDataManager::removeAchievementFromUserData(int index)
{
    CCLOG("Remove achievement %d", index);
    CUserDataManager::Instance()->setUserData_ItemRemove(USERDATA_KEY::ACHIEVEMENT_CUR_NORMAL_LIST, index);
}

void CAchievementDataManager::getCurAchievementListByType(ARRAY_DATA& list, bool isHidden)
{
    std::string key = USERDATA_KEY::ACHIEVEMENT_CUR_NORMAL_LIST;
    if(isHidden)key = USERDATA_KEY::ACHIEVEMENT_CUR_HIDDEN_LIST;
    
    list = CUserDataManager::Instance()->getUserData_List(key);
}

int CAchievementDataManager::getMaterialValueByLevel(std::string key, int level)
{
    auto message = [=](std::string msg){
        MessageBox(StringUtils::format("%s key : %s", msg.c_str(), key.c_str()).c_str(), "WARNING");
    };
    
    auto data = m_ValueMap.find(key);
    if(data == m_ValueMap.end()){
        message("No mtrl value list in the map.");
        return 0;
    }
    
    auto list = data->second;
    if(!list.size()){
        message("No value on the list.");
        return 0;
    }
    
    if(list.size() <= level){
//        message("No more data.");
        return list.back();
    }
    
    return list.at(level);
}

void CAchievementDataManager::initETCChekerList()
{
    auto initChecker = [=](std::string key, const CHECKER& func){
        m_CheckerList.emplace(std::pair<std::string, CHECKER>(key, func));
    };
   
	initChecker(CHARACTER_RARE_COUNT,CC_CALLBACK_1(CAchievementClearChecker::characterRareCountCheck, m_Checker));
    initChecker(ROCKET_RARE_COUNT,   CC_CALLBACK_1(CAchievementClearChecker::rocketRareCountCheck   , m_Checker));
}

void CAchievementDataManager::initRewarderList()
{
    auto initRewarder = [=](std::string key, const REWARDER& func){
        m_RewarderList.emplace(std::pair<std::string, REWARDER>(key, func));
    };
    
   	initRewarder(REWARD_COIN,			  CC_CALLBACK_1(CAchievementRewarder::coinReward			  , m_Rewarder));
	initRewarder(REWARD_CHARACTER,		  CC_CALLBACK_1(CAchievementRewarder::characterReward       , m_Rewarder));
	initRewarder(REWARD_ROCKET,			  CC_CALLBACK_1(CAchievementRewarder::RocketReward          , m_Rewarder));
	initRewarder(REWARD_PET,			  CC_CALLBACK_1(CAchievementRewarder::PetReward             , m_Rewarder));

	initRewarder(REWARD_COIN_RANDOM,	  CC_CALLBACK_1(CAchievementRewarder::coinRewardRandom      , m_Rewarder));
	initRewarder(REWARD_CHARACTER_RANDOM, CC_CALLBACK_1(CAchievementRewarder::characterRewardRandom , m_Rewarder));
	initRewarder(REWARD_ROCKET_RANDOM,	  CC_CALLBACK_1(CAchievementRewarder::RocketRewardRandom    , m_Rewarder));
	initRewarder(REWARD_PET_RANDOM,		  CC_CALLBACK_1(CAchievementRewarder::PetRewardRandom       , m_Rewarder));
}

const Json::Value CAchievementDataManager::initAchievementWithDefaultValue(bool hidden,
																	   std::string key, 
																	   const Json::Value data)
{
	if (data.isNull())
	{
		if (!hidden)	 return m_NormalAchievementDefaultSet[key.c_str()];
		else			 return m_HiddenAchievementDefaultSet[key.c_str()];
	}
	return data;
}

void CAchievementDataManager::addAchievementToList(ACHIEVEMENT_LIST &list, 
											   const Json::Value& data, 
											   bool hiddenType)
{
	sACHIEVEMENT_PARAM* param = new sACHIEVEMENT_PARAM();

	param->_index			= data["index"].asInt();
	param->_hiddenType		= hiddenType;

	std::string content		= StringUtils::format(ACHIEVEMENT_DEFINE::NORMAL_CONTENT.c_str(), param->_index);
    std::string title       = "";
    if (hiddenType) {
        content             = StringUtils::format(ACHIEVEMENT_DEFINE::HIDDEN_CONTENT.c_str(), param->_index);
        title               = StringUtils::format(ACHIEVEMENT_DEFINE::HIDDEN_TITLE.c_str(),   param->_index);
    }
    
    param->_title           = title;
    param->_contents		= content;

	auto initData = [=](std::string key){
		return this->initAchievementWithDefaultValue(param->_hiddenType, key, data[key.c_str()]);
	};
	param->_checkerType		= (CHECKER_TYPE)initData("checkerType").asInt();
	param->_rewardKey		= initData("rewardKey").asString();
	param->_rewardValue		= initData("rewardValue").asInt();
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
        auto level = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::LEVEL);
        auto key   = materialKeyArray.asString();
        auto value = this->getMaterialValueByLevel(key, std::max(0, level - 1));
        
		param->_materialKeyList.emplace_back(materialKeyArray.asString());
        param->_materialValueList.emplace_back(value);
	}
	list.emplace(std::pair<int, const sACHIEVEMENT_PARAM*>(param->_index, param));
}

void CAchievementDataManager::addValueListToMap(const Json::Value data)
{
    const Json::Value materialKey = data["materialKey"];
    const Json::Value valueArray  = data["valueList"];
    std::vector<int>  valueList;
    
    for(auto value : valueArray)
        valueList.emplace_back(value.asInt());
    
    m_ValueMap.emplace(std::pair<std::string, std::vector<int>>(materialKey.asString(), valueList));
}

cocos2d::Sprite* CAchievementDataManager::getRewardSprite(std::string rewardKey,
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