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
    initWithJson(m_NormalAchievementDataList, "normalAchievementList.json");
    initWithJson(m_HiddenAchievementDataList, "hiddenAchievementList.json");
}

CAchievementDataManager::~CAchievementDataManager()
{
    DATA_MANAGER_UTILS::mapDeleteAndClean(m_NormalAchievementDataList);
    DATA_MANAGER_UTILS::mapDeleteAndClean(m_HiddenAchievementDataList);
    CC_SAFE_DELETE(m_Checker);
    CC_SAFE_DELETE(m_Rewarder);
}

CAchievementDataManager* CAchievementDataManager::Instance()
{
    static CAchievementDataManager instance;
    return &instance;
}

void CAchievementDataManager::initWithJson(ACHIEVEMENT_LIST &list, std::string fileName)
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
    
	const Json::Value achievementArray    = root["achievements"];

    for (unsigned int count = 0; count < achievementArray.size(); ++count)
    {
        const Json::Value valueItem = achievementArray[count];
        this->addAchievementToList(list, valueItem);
    }
}

bool CAchievementDataManager::CheckCompleteAll()
{
    // Gets all of the achievement in the list.
    auto achievementList = CUserDataManager::Instance()->getUserData_ParamList(USERDATA_KEY::NORMAL_ACHIEVEMENT_LIST);
    
    // Gets a list of achievements in running or finished.
    auto list = DATA_MANAGER_UTILS::getMapByFunc([=](PARAM_DATA* param){
        if(param->size() <= USERDATA_PARAM_ACHIEVEMENT_NORMAL::STATE) return false;
        
        auto state = param->at(USERDATA_PARAM_ACHIEVEMENT_NORMAL::STATE);
        if(state == ACHIEVEMENT_NORMAL_STATE::FINISHED) return true;
        if(state == ACHIEVEMENT_NORMAL_STATE::RUNNING)  return true;
        
        return false;
    }, achievementList);
    
    // If the size of list is less than 3, Get 3 new achievements.
    if(list.size() < ACHIEVEMENT_DEFINE::LIMIT_COUNT) {
        this->getNewAchievements();
        return false;
    }
    
    // Check whether the list is complete.
    bool result = true;
    for (auto achievement : list){
        auto index = achievement.first;
        if (!this->CheckAchievementComplete(index, false)) result = false;
    }
    return result;
}

bool CAchievementDataManager::CheckAchievementComplete(int index, bool isHidden)
{
    int checkLevel = 0;
    std::string userDataKey = "";
    if(!isHidden) {
        userDataKey = USERDATA_KEY::NORMAL_ACHIEVEMENT_LIST;
        auto state  = this->getAchievementStateByIndex(index);
        if (state == ACHIEVEMENT_NORMAL_STATE::FINISHED)  return true;
        if (state == ACHIEVEMENT_NORMAL_STATE::COMPLETED) return true;
    }
    else {
        userDataKey = USERDATA_KEY::HIDDEN_ACHIEVEMENT_LIST;
        checkLevel  = this->getAchievementLevelByIndex(index);
        auto maxLevel = this->getAchievementMaxLevelByIndex(index, isHidden);
        
        //Return false, If the level got max.
        if(checkLevel >= maxLevel) return false;
    }
    
    const ACHIEVEMENT* achievementData = nullptr;
    if (isHidden) achievementData = this->getHiddenAchievementByIndex(index);
    else          achievementData = this->getNormalAchievementByIndex(index);
    
    auto levelData      = this->getLevelDataFromAchievement(index, checkLevel, isHidden);
    auto materialList   = levelData._materialList;
    auto checkerType    = levelData._checkerType;
    
    // If all of material in a ACHIEVEMENT_LEVEL has completed return true
    for(auto material : materialList)
    {
        auto key        = material._materialKey;
        auto paramList  = material._materialValues;
        auto mtrlValue  = paramList.at(0);
        auto checker	= m_CheckerList.find(key);

        switch(checkerType){
            case CHECKER_TYPE::ETC:
                if (checker == m_CheckerList.end()) {
                    CCLOG("There is no checker as key %s", key.c_str());
                    CCASSERT(false, "There is no checker as key");
                    return false;
                }
                if (!checker->second(mtrlValue)) return false;
                break;
            case CHECKER_TYPE::GLOBAL_DATA:
                if (!m_Checker->checkWithGlobal(key, mtrlValue))            return false; break;
        
            case CHECKER_TYPE::SINGLE_DATA:
                if (!m_Checker->checkWithSingleUserData(key, mtrlValue))    return false; break;
                
            case CHECKER_TYPE::ITEM_EXIST:
                if (!m_Checker->checkWithItemExist(key, mtrlValue))         return false; break;
                
            case CHECKER_TYPE::ITEM_COUNT:
                if (!m_Checker->checkWithCount(key, mtrlValue))             return false; break;
                
            case CHECKER_TYPE::ITEM_PARAM:{
                auto itemIndex = paramList.at(0);
                for(int paramIndex = 1; paramIndex < paramList.size(); paramIndex++){
                    auto paramValue = paramList.at(paramIndex);
                    if (!m_Checker->checkWithItemParam(key, itemIndex, paramIndex-1, paramValue))
                        return false;
                }
            }break;

            case CHECKER_TYPE::CONTINUE:
                if (!m_Checker->checkWithContinuingType(key, mtrlValue))    return false; break;
        }
    }
    
    //If the achievement has completed.
    std::string clearCountKey = "";
    if (isHidden){
        CUserDataManager::Instance()->setUserData_ItemParam(userDataKey, index,
                                                            USERDATA_PARAM_ACHIEVEMENT_HIDDEN::LEVEL,
                                                            checkLevel+1);
        GLOBAL->HIDDEN_ACHIEVEMENT_CLEAR_COUNT += 1;
        clearCountKey = USERDATA_KEY::HIDDEN_CLEAR_COUNT;
    }
    else{
        CUserDataManager::Instance()->setUserData_ItemParam(userDataKey, index,
                                                            USERDATA_PARAM_ACHIEVEMENT_NORMAL::STATE,
                                                            ACHIEVEMENT_NORMAL_STATE::FINISHED);
        GLOBAL->NORMAL_ACHIEVEMENT_CLEAR_COUNT += 1;
        clearCountKey = USERDATA_KEY::NORMAL_CLEAR_COUNT;
    }
    
    // Clear count +1
    {
        auto clearCount = CUserDataManager::Instance()->getUserData_Number(clearCountKey);
        CUserDataManager::Instance()->setUserData_Number(clearCountKey, clearCount+1);
    }
	return true;
}

const ACHIEVEMENT* CAchievementDataManager::CompleteCheckRealTime(bool isHidden)
{
    ACHIEVEMENT_LIST achievementList;
    if(isHidden) achievementList = this->getHiddenAchievementList();
    else         achievementList = this->getPickedAchievementList();
    
    if (achievementList.size() <= 0) return nullptr;

    for(auto achievement : achievementList)
    {
        auto index = achievement.second->_index;
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
//    CUserDataManager::Instance()->setUserData_ItemRemoveAll(USERDATA_KEY::ACHIEVEMENT_COM_NORMAL_LIST);
}

sREWARD_DATA CAchievementDataManager::Reward(int index)
{
//    auto achievementData = getNormalAchievementByIndex(index);
//    
//    auto key = achievementData->_rewardKey;
//    auto rewardValue = achievementData->_rewardValue;
//    
//	return this->RewardByKey(key, rewardValue);
}

sREWARD_DATA CAchievementDataManager::RewardByKey(std::string key, int value)
{
	auto rewarder = m_RewarderList.find(key);
	if (rewarder == std::end(m_RewarderList)) return sREWARD_DATA();

	return rewarder->second(sREWARD_DATA(key, value)); // call function by reward type.
}

int CAchievementDataManager::NonCompleteAchievementExist()
{
    return this->getNonCompletedAchievementList().size();
}

void CAchievementDataManager::getNewAchievements()
{
    auto pickedList      = this->getPickedAchievementList();
    auto nonCompleteList = this->getNonCompletedAchievementList();
    
    // If achievements not completed are less than limit count,
    // remove current achievements too.
    if(nonCompleteList.size() < ACHIEVEMENT_DEFINE::LIMIT_COUNT)
    {
		for (int index = 0; index < pickedList.size(); index++)
			this->removeAchievementFromUserData(pickedList.at(index)->_index);
        
        return;
    }
    
    // If current achievements are less than limit count.
    // get new achievements in the limit count.
	if (pickedList.size() < ACHIEVEMENT_DEFINE::LIMIT_COUNT)
    {
		for (int count = 0; count < ACHIEVEMENT_DEFINE::LIMIT_COUNT - pickedList.size(); count++)
            this->getNewRandomAchievement();
        
        return;
    }
    
    // If there are enough count of achievements to skip. (to get new achievements)
	for (int count = 0; count < ACHIEVEMENT_DEFINE::LIMIT_COUNT; count++)
		this->SkipAchievement(pickedList.at(count)->_index); // Get new achievements.
}

const ACHIEVEMENT* CAchievementDataManager::SkipAchievement(int index)
{
    auto newAchievement = this->getNewRandomAchievement();
	this->removeAchievementFromUserData(index);

	return newAchievement;
}

const ACHIEVEMENT* CAchievementDataManager::getNormalAchievementByIndex(int index) const
{
    auto data = m_NormalAchievementDataList.find(index);
    if(data == m_NormalAchievementDataList.end()) {
        CCLOG("Wrong character index : %d", index);
        CCASSERT(false, "Wrong character index");
        return nullptr;
    }
    return data->second;
}

const ACHIEVEMENT* CAchievementDataManager::getHiddenAchievementByIndex(int index) const
{
    auto data = m_HiddenAchievementDataList.find(index);
    if(data == m_HiddenAchievementDataList.end()) {
        CCLOG("Wrong character index : %d", index);
        CCASSERT(false, "Wrong character index");
        return nullptr;
    }
    return data->second;
}

const ACHIEVEMENT* CAchievementDataManager::getNewRandomAchievement()
{
    auto newList        = this->getNonCompletedAchievementList();
    auto newAchievement = this->getNewRandomAchievementFromList(newList);
    auto index          = newAchievement->_index;
    CCLOG("Get new achievement %d", index);
    
    // State initialize
    CUserDataManager::Instance()->setUserData_ItemParam(USERDATA_KEY::NORMAL_ACHIEVEMENT_LIST, index,
                                                        USERDATA_PARAM_ACHIEVEMENT_NORMAL::STATE,
                                                        ACHIEVEMENT_NORMAL_STATE::RUNNING);
    return newAchievement;
}

const ACHIEVEMENT* CAchievementDataManager::getFirstFromNonCompleted()
{
    auto runningList    = this->getRunnnigAchievementList();
    if(!runningList.size()) return nullptr;
    
    return runningList.at(0);
}

ACHIEVEMENT_LEVEL CAchievementDataManager::getCurLevelDataByIndex(int index, bool isHidden)
{
    int level = 0;
    if(isHidden) level = this->getAchievementLevelByIndex(index);
    else         level = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::LEVEL);

    return this->getLevelDataFromAchievement(index, level, isHidden);
}

std::string CAchievementDataManager::getAchievementTitleByIndex(int index)
{
    auto level = this->getAchievementLevelByIndex(index);
    auto title = StringUtils::format(ACHIEVEMENT_DEFINE::HIDDEN_TITLE.c_str(), index, level);
    return TRANSLATE(title);
}

std::string CAchievementDataManager::getAchievementContentsByIndex(int index, bool isHidden)
{
    std::string  key = ACHIEVEMENT_DEFINE::NORMAL_CONTENT;
    if(isHidden) key = ACHIEVEMENT_DEFINE::HIDDEN_CONTENT;
    
    auto levelData   = this->getCurLevelDataByIndex(index, isHidden);
    auto contentsKey = StringUtils::format(key.c_str(), index);
    return StringUtils::format(TRANSLATE(contentsKey).c_str(), levelData._contentsValue);
}

ACHIEVEMENT_LIST CAchievementDataManager::getHiddenAchievementList() const
{
    return m_HiddenAchievementDataList;
}

ACHIEVEMENT_LIST CAchievementDataManager::getNonCompletedAchievementList() const
{
    return DATA_MANAGER_UTILS::getMapByFunc([=](const ACHIEVEMENT* data){
        
        auto state = this->getAchievementStateByIndex(data->_index);
        if(state == ACHIEVEMENT_NORMAL_STATE::NONE_COMPLETED) return true;
        return false;
    }, m_NormalAchievementDataList);
}

ACHIEVEMENT_LIST CAchievementDataManager::getPickedAchievementList() const
{
    return DATA_MANAGER_UTILS::getMapByFunc([=](const ACHIEVEMENT* data){
        
        auto state = this->getAchievementStateByIndex(data->_index);
        if(state == ACHIEVEMENT_NORMAL_STATE::RUNNING)  return true;
        if(state == ACHIEVEMENT_NORMAL_STATE::FINISHED) return true;
        return false;
    }, m_NormalAchievementDataList);
}

ACHIEVEMENT_LIST CAchievementDataManager::getRunnnigAchievementList() const
{
    return DATA_MANAGER_UTILS::getMapByFunc([=](const ACHIEVEMENT* data){
        
        auto state = this->getAchievementStateByIndex(data->_index);
        if(state == ACHIEVEMENT_NORMAL_STATE::RUNNING)  return true;
        return false;
    }, m_NormalAchievementDataList);
}

int CAchievementDataManager::getAchievementMaxLevelByIndex(int index, bool isHidden)
{
    const ACHIEVEMENT* data = nullptr;
    if(isHidden) data = this->getHiddenAchievementByIndex(index);
    else         data = this->getNormalAchievementByIndex(index);
    
    return (int)data->_levelList.size()-1;
}

int CAchievementDataManager::getAchievementLevelByIndex(int index)
{
    return CUserDataManager::Instance()->getUserData_ParamData(USERDATA_KEY::HIDDEN_ACHIEVEMENT_LIST,
                                                               index,
                                                               USERDATA_PARAM_ACHIEVEMENT_HIDDEN::LEVEL, 0);
}

int CAchievementDataManager::getAchievementStateByIndex(int index)
{
    return CUserDataManager::Instance()->getUserData_ParamData(USERDATA_KEY::NORMAL_ACHIEVEMENT_LIST,
                                                               index,
                                                               USERDATA_PARAM_ACHIEVEMENT_NORMAL::STATE, 0);
}

const ACHIEVEMENT* CAchievementDataManager::getNewRandomAchievementFromList(ACHIEVEMENT_LIST &list)
{
    auto size = list.size();
    if(size <= 0) {
        CCLOG("There is no achievement anymore.");
        return nullptr;
    }
    auto picked = list.begin();
    std::advance(picked, random<int>(0, int(list.size()-1)));
    
    CCLOG("Pick a achievement :: idx %d", (picked->second)->_index);
    
    return (picked->second);
}

ACHIEVEMENT_LEVEL CAchievementDataManager::getLevelDataFromAchievement(int index, int level, bool isHidden)
{
    const ACHIEVEMENT* achievement = nullptr;
    if(isHidden) achievement = this->getHiddenAchievementByIndex(index);
    else         achievement = this->getNormalAchievementByIndex(index);
    
    if(achievement->_levelList.size() <= level){
        CCLOG("There is no %d level data indexed %d and hidden %d", level, index, isHidden);
        CCASSERT(false, "There is no level data");
    }
    
    return achievement->_levelList.at(level);
}


void CAchievementDataManager::removeAchievementFromUserData(int index)
{
    CCLOG("Remove achievement %d", index);
//    CUserDataManager::Instance()->setUserData_ItemRemove(USERDATA_KEY::ACHIEVEMENT_CUR_NORMAL_LIST, index);
}

void CAchievementDataManager::getAchievementParamListByType(PARAM_DATA_ARRAY& list, bool isHidden)
{
    std::string key = USERDATA_KEY::NORMAL_ACHIEVEMENT_LIST;
    if(isHidden)key = USERDATA_KEY::HIDDEN_ACHIEVEMENT_LIST;
    
    list = CUserDataManager::Instance()->getUserData_ParamList(key);
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
    
   	initRewarder(REWARD_COIN,			  CC_CALLBACK_1(CAchievementRewarder::coinReward			, m_Rewarder));
	initRewarder(REWARD_CHARACTER,		  CC_CALLBACK_1(CAchievementRewarder::characterReward       , m_Rewarder));
	initRewarder(REWARD_ROCKET,			  CC_CALLBACK_1(CAchievementRewarder::RocketReward          , m_Rewarder));
	initRewarder(REWARD_PET,			  CC_CALLBACK_1(CAchievementRewarder::PetReward             , m_Rewarder));

	initRewarder(REWARD_COIN_RANDOM,	  CC_CALLBACK_1(CAchievementRewarder::coinRewardRandom      , m_Rewarder));
	initRewarder(REWARD_CHARACTER_RANDOM, CC_CALLBACK_1(CAchievementRewarder::characterRewardRandom , m_Rewarder));
	initRewarder(REWARD_ROCKET_RANDOM,	  CC_CALLBACK_1(CAchievementRewarder::RocketRewardRandom    , m_Rewarder));
	initRewarder(REWARD_PET_RANDOM,		  CC_CALLBACK_1(CAchievementRewarder::PetRewardRandom       , m_Rewarder));
}

void CAchievementDataManager::addAchievementToList(ACHIEVEMENT_LIST &list, 
											   const Json::Value &data)
{
    if(data["index"].isNull()) return;
    
	ACHIEVEMENT* achievement    = new ACHIEVEMENT();

	achievement->_index         = data["index"].asInt();
    achievement->_visibleType   = data["visible"].asBool();
	achievement->_hiddenType    = data["hidden"].asBool();
    
    auto levelArray             = data["level"];
    for(auto levelData : levelArray){
        if(levelData.isNull())
            continue;
        this->addLevelToAchievement(achievement->_levelList, levelData);
    }
    list.emplace(std::pair<int, const ACHIEVEMENT*>(achievement->_index, achievement));
}

void CAchievementDataManager::addLevelToAchievement(ACHIEVEMENT_LEVEL_LIST &list, const Json::Value &data)
{
    if(data["rewardKey"].isNull()) return;
    
    ACHIEVEMENT_LEVEL level;
    
    level._rewardKey        = data["rewardKey"].asString();
    level._rewardValue      = data["rewardValue"].asInt();
    level._contentsValue    = data["contentsValue"].asInt();
    level._checkerType      = (CHECKER_TYPE)data["checkerType"].asInt();
    
    auto materialArray      = data["material"];
    for(auto materialData : materialArray){
        if(materialData.isNull())
            continue;
        this->addMaterialToLevel(level._materialList, materialData);
    }
    list.emplace_back(level);
}

void CAchievementDataManager::addMaterialToLevel(MATERIAL_LIST &list, const Json::Value &data)
{
    if(data["materialKey"].isNull()) return;
    
    ACHIEVEMENT_MATERIAL material;
    material._materialKey   = data["materialKey"].asString();
    auto valueList          = data["materialValue"];
    
    for(auto value : valueList){
        if(value.isNull())
            continue;
        material._materialValues.emplace_back(value.asInt());
    }
    
    list.emplace_back(material);
}