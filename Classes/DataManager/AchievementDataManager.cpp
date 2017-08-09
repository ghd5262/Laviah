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
        if(param->size() <= PARAM_ACHIEVEMENT_NORMAL::NORMAL_STATE) return false;
        
        auto state = param->at(PARAM_ACHIEVEMENT_NORMAL::NORMAL_STATE);
        if(state == ACHIEVEMENT_STATE::FINISHED) return true;
        if(state == ACHIEVEMENT_STATE::RUNNING)  return true;
        
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
    auto state  = this->getAchievementStateByIndex(index, isHidden);
    
    if (state == ACHIEVEMENT_STATE::FINISHED)  return true;
    if (state == ACHIEVEMENT_STATE::COMPLETED) return true;

    std::string userDataKey = USERDATA_KEY::NORMAL_ACHIEVEMENT_LIST;
    if(isHidden){
        userDataKey         = USERDATA_KEY::HIDDEN_ACHIEVEMENT_LIST;
        
        //Return false, If the level got max.
        if(this->CompletedAllOfLevels(index)) return false;
    }

    const ACHIEVEMENT* achievementData = nullptr;
    if(isHidden) achievementData = this->getHiddenAchievementByIndex(index);
    else         achievementData = this->getNormalAchievementByIndex(index);
    
    auto userLevel = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::LEVEL);
    if(achievementData->_openLevel > userLevel) return false;
    if(!achievementData->_visibleType)          return false;

    auto levelData      = this->getCurLevelDataByIndex(index, isHidden);
    auto materialList   = levelData._materialList;
    auto checkerType    = levelData._checkerType;
    auto characterIndex = levelData._characterIndex;
    
    if(characterIndex > -1){
        if(characterIndex != GVALUE->CURRENT_CHARACTER) return false;
    }
    
    // If all of material in a ACHIEVEMENT_LEVEL has completed return true
    for(auto material : materialList)
    {
        auto key        = material._materialKey;
        auto value      = material._materialValue;
        auto valueList  = material._materialValues;
        auto isFit      = material._isFit;
        auto checker	= m_CheckerList.find(key);
        switch(checkerType){
            case CHECKER_TYPE::ETC:{
                auto curValue = 0;
                if (checker == m_CheckerList.end()) {
                    CCLOG("There is no checker as key %s", key.c_str());
                    CCASSERT(false, "There is no checker as key");
                    return false;
                }
                if (!checker->second(value, curValue))              return false;
            } break;
            case CHECKER_TYPE::GLOBAL_DATA:{
                if (!isHidden) value = this->getNormalAchievementMaterialValue(index);
                if (!m_Checker->checkWithGlobal(key, value, isFit))
                    return false;
            }break;
        
            case CHECKER_TYPE::SINGLE_DATA:
                if (!m_Checker->checkWithSingleUserData(key, value, isFit))
                    return false;
                break;
                
            case CHECKER_TYPE::ITEM_EXIST:{
                for(auto value : valueList)
                    if (!m_Checker->checkWithItemExist(key, value))
                        return false;
            } break;
            case CHECKER_TYPE::ITEM_COUNT:
                if (!m_Checker->checkWithCount(key, value))
                    return false;
                break;
                
            case CHECKER_TYPE::ITEM_PARAM:{
                auto itemIndex = material._itemIndex;
                if (!m_Checker->checkWithItemParam(key, itemIndex, 0, value, isFit))
                    return false;
            } break;

            case CHECKER_TYPE::CONTINUE:
                if (!m_Checker->checkWithContinuingType(key, value))
                    return false;
                break;
        }
    }
    
    // If the achievement has completed.
    auto paramIndex = 0;
    auto paramState = 0;
    if (isHidden){
        GVALUE->HIDDEN_ACHIEVEMENT_CLEAR_COUNT += 1;
        paramIndex  = PARAM_ACHIEVEMENT_HIDDEN::HIDDEN_STATE;
        paramState  = ACHIEVEMENT_STATE::COMPLETED;
    }
    else{
        GVALUE->NORMAL_ACHIEVEMENT_CLEAR_COUNT += 1;
        
        // Clear count +1
        CUserDataManager::Instance()->setUserData_NumberAdd(USERDATA_KEY::NORMAL_CLEAR_COUNT, 1);
        paramIndex  = PARAM_ACHIEVEMENT_NORMAL::NORMAL_STATE;
        paramState  = ACHIEVEMENT_STATE::FINISHED;
    }
    
    // Update user data.
    {
        // Change the state of achievement.
        CAchievementDataManager::setAchievementStateByIndex(index, paramState, isHidden);
    }
	return true;
}

const ACHIEVEMENT* CAchievementDataManager::CompleteCheckRealTime(bool isHidden)
{
    ACHIEVEMENT_LIST achievementList;
    if(isHidden) achievementList = m_HiddenAchievementDataList;
    else         achievementList = this->getPickedAchievementList();
    
    if (achievementList.size() <= 0) return nullptr;

    for(auto achievement : achievementList)
    {
        auto index = achievement.second->_index;
        auto normalOriginCount = GVALUE->NORMAL_ACHIEVEMENT_CLEAR_COUNT;
        auto hiddenOriginCount = GVALUE->HIDDEN_ACHIEVEMENT_CLEAR_COUNT;
        if (this->CheckAchievementComplete(index, isHidden)){
            if (normalOriginCount < GVALUE->NORMAL_ACHIEVEMENT_CLEAR_COUNT)
                return this->getNormalAchievementByIndex(index);
            if (hiddenOriginCount < GVALUE->HIDDEN_ACHIEVEMENT_CLEAR_COUNT)
                return this->getHiddenAchievementByIndex(index);
        }
    }
    
	return nullptr;
}

void CAchievementDataManager::ResetNormalAchievements()
{
    auto list = CUserDataManager::Instance()->getUserData_ParamList(USERDATA_KEY::NORMAL_ACHIEVEMENT_LIST);
    for (auto data : list)
        this->setAchievementStateByIndex(data.first, ACHIEVEMENT_STATE::NON_COMPLETED, false);
}

void CAchievementDataManager::HiddenAchievementLevelUP(int index)
{
    // Hidden clear count +1
    CUserDataManager::Instance()->setUserData_NumberAdd(USERDATA_KEY::HIDDEN_CLEAR_COUNT, 1);
    
    // Update state of achievement to non-completed
    CAchievementDataManager::setAchievementStateByIndex(index,
                                                        ACHIEVEMENT_STATE::NON_COMPLETED,
                                                        true);
    
    // Get current level of achievement.
    auto level = CAchievementDataManager::getAchievementLevelByIndex(index, true);
    
    // Update level of achievement.
    CAchievementDataManager::setAchievementLevelByIndex(index, level+1, true);
}

bool CAchievementDataManager::CompletedAllOfLevels(int index)
{
    auto currentLevel = this->getAchievementLevelByIndex(index, true);
    auto maxLevel     = this->getAchievementMaxLevelByIndex(index, true);

    return (currentLevel > maxLevel);
}

bool CAchievementDataManager::ExistCompletedHiddenAchievement()
{
    auto key1   = USERDATA_KEY::HIDDEN_ACHIEVEMENT_LIST;
    auto key2   = PARAM_ACHIEVEMENT_HIDDEN::HIDDEN_STATE;
    auto key3   = ACHIEVEMENT_STATE::COMPLETED;
    return CUserDataManager::Instance()->getUserData_IsItemExistWithParam(key1, key2, key3);
}

bool CAchievementDataManager::IsHiddenAchievement(int index)
{
    auto data = this->getHiddenAchievementByIndex(index);
    if(data == nullptr) return false;
    if(data->_type != ACHIEVEMENT_TYPE::HIDDEN_TYPE)      return false;
    if(this->getAchievementLevelByIndex(index, true) > 0) return false;
    if(this->getAchievementStateByIndex(index, true) == ACHIEVEMENT_STATE::COMPLETED) return false;
    if(this->getAchievementStateByIndex(index, true) == ACHIEVEMENT_STATE::FINISHED)  return false;
    
    return true;
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
    // Change the state of achievement in the list picked to the non-completed.
    if(nonCompleteList.size() < ACHIEVEMENT_DEFINE::LIMIT_COUNT)
    {
        for (int index = 0; index < pickedList.size(); index++){
            auto iter = pickedList.begin();
            std::advance(iter, index);
            auto data = (iter->second);
            CAchievementDataManager::setAchievementStateByIndex(data->_index,
                                                                ACHIEVEMENT_STATE::NON_COMPLETED,
                                                                false);
        }
        
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
    for (int count = 0; count < ACHIEVEMENT_DEFINE::LIMIT_COUNT; count++){
        auto iter  = pickedList.begin();
        std::advance(iter, count);
        
        auto index = (iter->second)->_index;
        this->getNewRandomAchievement();
        this->setAchievementStateByIndex(index, ACHIEVEMENT_STATE::COMPLETED, false);
    }
}

const ACHIEVEMENT* CAchievementDataManager::getNormalAchievementByIndex(int index) const
{
    auto data = m_NormalAchievementDataList.find(index);
    if(data == m_NormalAchievementDataList.end()) {
        CCLOG("Wrong achievement index : %d", index);
        CCASSERT(false, "Wrong achievement index");
        return nullptr;
    }
    return data->second;
}

const ACHIEVEMENT* CAchievementDataManager::getHiddenAchievementByIndex(int index) const
{
    auto data = m_HiddenAchievementDataList.find(index);
    if(data == m_HiddenAchievementDataList.end()) {
        CCLOG("Wrong achievement index : %d", index);
        CCASSERT(false, "Wrong achievement index");
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
    CAchievementDataManager::setAchievementStateByIndex(index, ACHIEVEMENT_STATE::RUNNING, false);
    
    // Level initialize
    auto maxLevel = this->getAchievementMaxLevelByIndex(index, false);
    auto curLevel = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::LEVEL);
    if(curLevel > maxLevel) curLevel = maxLevel;
    CAchievementDataManager::setAchievementLevelByIndex(index, curLevel, false);
    
    return newAchievement;
}

// Do not call this function every frame.
const ACHIEVEMENT* CAchievementDataManager::getFirstFromNonCompleted()
{
    auto runningList    = this->getRunnnigAchievementList();
    if(!runningList.size()) return nullptr;
    auto iter           = runningList.begin();
    std::advance(iter, 0);
    return iter->second;
}

ACHIEVEMENT_LEVEL CAchievementDataManager::getCurLevelDataByIndex(int index, bool isHidden)
{
    int level = this->getAchievementLevelByIndex(index, isHidden);
    if(!isHidden) level = 0;
    return this->getLevelDataFromAchievement(index, level, isHidden);
}

std::string CAchievementDataManager::getAchievementTitleByIndex(int index)
{
    auto level = this->getAchievementLevelByIndex(index, true);
    if(this->CompletedAllOfLevels(index))
        level  = this->getAchievementMaxLevelByIndex(index, true);
    
    return this->getAchievementTitle(index, level);
}

std::string CAchievementDataManager::getAchievementTitle(int index, int level)
{
    auto origin   = StringUtils::format(ACHIEVEMENT_DEFINE::HIDDEN_TITLE.c_str(), index, 0);
    auto levelKey = StringUtils::format(ACHIEVEMENT_DEFINE::HIDDEN_TITLE.c_str(), index, level);
    auto title    = TRANSLATE(levelKey);
    
    if(title == "") title = TRANSLATE(origin) + StringUtils::format(" (%d)", level+1);
    return title;
}

std::string CAchievementDataManager::getAchievementContentsByIndex(int index, bool isHidden)
{
    auto level = this->getAchievementLevelByIndex(index, isHidden);
    auto max   = this->getAchievementMaxLevelByIndex(index, isHidden);
    level = std::min(max, level);
    return this->getAchievementContents(index, level, isHidden);
}

std::string CAchievementDataManager::getAchievementContents(int index, int level, bool isHidden)
{
    std::string  key = ACHIEVEMENT_DEFINE::NORMAL_CONTENT;
    if(isHidden) key = ACHIEVEMENT_DEFINE::HIDDEN_CONTENT;
    
    auto origin    = StringUtils::format(key.c_str(), index, 0);
    auto levelKey  = StringUtils::format(key.c_str(), index, level);
    auto contents  = TRANSLATE(levelKey);
    
    if(contents == "") contents = TRANSLATE(origin);
    
    auto levelData    = this->getCurLevelDataByIndex(index, isHidden);
    int contentsValue = 0;
    if(isHidden) contentsValue = levelData._contentsValue;
    else         contentsValue = this->getNormalAchievementMaterialValue(index);

    return StringUtils::format(contents.c_str(), contentsValue);
}

ACHIEVEMENT_LIST CAchievementDataManager::getHiddenAchievementList() const
{
    return m_HiddenAchievementDataList;
}

ACHIEVEMENT_LIST CAchievementDataManager::getNonCompletedAchievementList()
{
    return DATA_MANAGER_UTILS::getMapByFunc([=](const ACHIEVEMENT* data){
        
        auto state = this->getAchievementStateByIndex(data->_index, false);
        auto level = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::LEVEL);
        auto levelData = this->getCurLevelDataByIndex(data->_index, false);
        auto character = levelData._characterIndex;
        auto exist = true;
        if(character > -1)
            exist = CUserDataManager::Instance()->getUserData_IsItemExist(USERDATA_KEY::CHARACTER_LIST, character);
        if(state != ACHIEVEMENT_STATE::NON_COMPLETED) return false;
        if(level < data->_openLevel) return false;
        if(!data->_visibleType) return false;
        if(!exist) return false;
        
        return true;
    }, m_NormalAchievementDataList);
}

// Do not call this function every frame.
ACHIEVEMENT_LIST CAchievementDataManager::getPickedAchievementList() const
{
    return DATA_MANAGER_UTILS::getMapByFunc([=](const ACHIEVEMENT* data){
        
        auto state = this->getAchievementStateByIndex(data->_index, false);
        if(state == ACHIEVEMENT_STATE::RUNNING)  return true;
        if(state == ACHIEVEMENT_STATE::FINISHED) return true;
        return false;
    }, m_NormalAchievementDataList);
}

// Do not call this function every frame.
ACHIEVEMENT_LIST CAchievementDataManager::getRunnnigAchievementList() const
{
    return DATA_MANAGER_UTILS::getMapByFunc([=](const ACHIEVEMENT* data){
        
        auto state = this->getAchievementStateByIndex(data->_index, false);
        if(state == ACHIEVEMENT_STATE::RUNNING)  return true;
        return false;
    }, m_NormalAchievementDataList);
}

int CAchievementDataManager::getAchievementMaxLevelByIndex(int index, bool isHidden)
{
    const ACHIEVEMENT* data = nullptr;
    int maxLevel = 0;
    if(isHidden) {
        data = this->getHiddenAchievementByIndex(index);
        maxLevel = std::max((int)data->_levelList.size()-1, 0);
    }
    else{
        data = this->getNormalAchievementByIndex(index);
        try {
            auto levelData = data->_levelList.at(0);
            try {
                auto materialData = levelData._materialList.at(0);
                maxLevel          = materialData._materialValues.size()-1;
            } catch (...) {
                CCLOG("There are no material datas in the list.");
                CCASSERT(false, "There are no material datas in the list.");
            }
        } catch (...) {
            CCLOG("There are no level datas in the list.");
            CCASSERT(false, "There are no level datas in the list.");
        }
    }
    return maxLevel;
}

int CAchievementDataManager::getHiddenAchievementCurrentValue(int index)
{
    auto achievementData = this->getHiddenAchievementByIndex(index);
    if(!achievementData->_visibleType) return false;
    
    auto levelData      = this->getCurLevelDataByIndex(index, true);
    auto materialList   = levelData._materialList;
    auto checkerType    = levelData._checkerType;
    auto characterIndex = levelData._characterIndex;
    auto curValue       = 0;

    if(characterIndex > -1){
        if(characterIndex != GVALUE->CURRENT_CHARACTER) return 0;
    }
    // If all of material in a ACHIEVEMENT_LEVEL has completed return true
    for(auto material : materialList)
    {
        auto key        = material._materialKey;
        auto value      = material._materialValue;
        auto valueList  = material._materialValues;
        auto checker	= m_CheckerList.find(key);
        switch(checkerType){
            case CHECKER_TYPE::ETC:
                if (checker == m_CheckerList.end()) {
                    CCLOG("There is no checker as key %s", key.c_str());
                    CCASSERT(false, "There is no checker as key");
                    return false;
                }
                checker->second(value, curValue);
                break;
            case CHECKER_TYPE::GLOBAL_DATA:
                curValue += GVALUE->getVariable(key);
                break;
                
            case CHECKER_TYPE::SINGLE_DATA:
                curValue += CUserDataManager::Instance()->getUserData_Number(key);
                break;
                
            case CHECKER_TYPE::ITEM_EXIST:{
                for(auto value : valueList)
                    curValue += CUserDataManager::Instance()->getUserData_IsItemExist(key, value);
            } break;
                
            case CHECKER_TYPE::ITEM_COUNT:
                curValue += CUserDataManager::Instance()->getUserData_ParamList(key).size();
                break;
                
            case CHECKER_TYPE::ITEM_PARAM:{
                auto itemIndex = material._itemIndex;
                curValue += CUserDataManager::Instance()->getUserData_ParamData(key, itemIndex, 0, 0);
            }break;
                
            case CHECKER_TYPE::CONTINUE: break;
        }
    }
    return curValue;
}

int CAchievementDataManager::getNormalAchievementCurrentValue(int index)
{
    auto data           = this->getCurLevelDataByIndex(index, false);
    auto characterIndex = data._characterIndex;
    if(characterIndex > -1){
        if(characterIndex != GVALUE->CURRENT_CHARACTER) return 0;
    }
    try {
        auto materialData = data._materialList.at(0);
        return GVALUE->getVariable(materialData._materialKey);
    } catch (...) {
        CCLOG("There are no material datas in the list.");
        CCASSERT(false, "There are no material datas in the list.");
    }
}

int CAchievementDataManager::getNormalAchievementMaterialValue(int index)
{
    auto maxLevel = this->getAchievementMaxLevelByIndex(index, false);
    auto level    = this->getAchievementLevelByIndex(index, false);
    auto data     = this->getCurLevelDataByIndex(index, false);
    if(level > maxLevel) level = maxLevel;
    auto value    = 0;
    try {
        auto materialData = data._materialList.at(0);
        value = materialData._materialValues.at(level);
    } catch (...) {
        CCLOG("There are no material datas in the list.");
        CCASSERT(false, "There are no material datas in the list.");
    }

    return value;
}

int CAchievementDataManager::getNormalAchievementRewardValue(int index)
{
    auto level = this->getAchievementLevelByIndex(index, false);
    auto data  = this->getCurLevelDataByIndex(index, false);
    return data._rewardValue * (level + 1);
}

int CAchievementDataManager::getAchievementLevelByIndex(int index, bool isHidden)
{
    auto key = USERDATA_KEY::NORMAL_ACHIEVEMENT_LIST;
    int  param = PARAM_ACHIEVEMENT_NORMAL::NORMAL_LEVEL;
    if(isHidden) {
        key = USERDATA_KEY::HIDDEN_ACHIEVEMENT_LIST;
        param = PARAM_ACHIEVEMENT_HIDDEN::HIDDEN_LEVEL;
    }
    
    return CUserDataManager::Instance()->getUserData_ParamData(key, index, param, 0);
}

int CAchievementDataManager::getAchievementStateByIndex(int index, bool isHidden)
{
    std::string userDataKey = USERDATA_KEY::NORMAL_ACHIEVEMENT_LIST;
    int         paramIndex  = PARAM_ACHIEVEMENT_NORMAL::NORMAL_STATE;
    
    if(isHidden) {
        userDataKey = USERDATA_KEY::HIDDEN_ACHIEVEMENT_LIST;
        paramIndex  = PARAM_ACHIEVEMENT_HIDDEN::HIDDEN_STATE;
    }
    return CUserDataManager::Instance()->getUserData_ParamData(userDataKey, index,
                                                               paramIndex, 0);
}

void CAchievementDataManager::setAchievementStateByIndex(int index, int state, bool isHidden)
{
    std::string userDataKey = USERDATA_KEY::NORMAL_ACHIEVEMENT_LIST;
    int         paramIndex  = PARAM_ACHIEVEMENT_NORMAL::NORMAL_STATE;
    
    if(isHidden) {
        userDataKey = USERDATA_KEY::HIDDEN_ACHIEVEMENT_LIST;
        paramIndex  = PARAM_ACHIEVEMENT_HIDDEN::HIDDEN_STATE;
    }
    
    CUserDataManager::Instance()->setUserData_ItemParam(userDataKey, index, paramIndex, state);
}

void CAchievementDataManager::setAchievementLevelByIndex(int index, int level, bool isHidden)
{
    std::string userDataKey = USERDATA_KEY::NORMAL_ACHIEVEMENT_LIST;
    int         paramIndex  = PARAM_ACHIEVEMENT_NORMAL::NORMAL_LEVEL;

    if(isHidden) {
        userDataKey = USERDATA_KEY::HIDDEN_ACHIEVEMENT_LIST;
        paramIndex  = PARAM_ACHIEVEMENT_HIDDEN::HIDDEN_LEVEL;
    }
    
    CUserDataManager::Instance()->setUserData_ItemParam(userDataKey, index, paramIndex, level);
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

    auto            maxLevel = this->getAchievementMaxLevelByIndex(index, isHidden);
    if(maxLevel < 0) {
        CCLOG("There is no data as level : %d, index : %d, hidden : %d", level, index, isHidden);
        CCASSERT(false, "There is no data as level");
    }
    
    if(maxLevel < level){
        if(0 <= maxLevel)
            return achievement->_levelList.at(maxLevel);
    }
    try {
        return achievement->_levelList.at(level);
    } catch (...) {
        CCLOG("There is no level data - level : %d, index : %d, hidden : %d", level, index, isHidden);
        CCASSERT(false, "Wrong level");
    }
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
   
	initChecker(CHARACTER_RARE_COUNT,CC_CALLBACK_2(CAchievementClearChecker::characterRareCountCheck, m_Checker));
    initChecker(ROCKET_RARE_COUNT,   CC_CALLBACK_2(CAchievementClearChecker::rocketRareCountCheck   , m_Checker));
}

void CAchievementDataManager::initRewarderList()
{
    auto initRewarder = [=](std::string key, const REWARDER& func){
        m_RewarderList.emplace(std::pair<std::string, REWARDER>(key, func));
    };
    
   	initRewarder(REWARD_COIN,			  CC_CALLBACK_1(CAchievementRewarder::coinReward			, m_Rewarder));
	initRewarder(REWARD_CHARACTER,		  CC_CALLBACK_1(CAchievementRewarder::characterReward       , m_Rewarder));
    initRewarder(REWARD_COSTUME,		  CC_CALLBACK_1(CAchievementRewarder::costumeReward         , m_Rewarder));
	initRewarder(REWARD_ROCKET,			  CC_CALLBACK_1(CAchievementRewarder::rocketReward          , m_Rewarder));
	initRewarder(REWARD_PET,			  CC_CALLBACK_1(CAchievementRewarder::petReward             , m_Rewarder));

	initRewarder(REWARD_COIN_RANDOM,	  CC_CALLBACK_1(CAchievementRewarder::coinRewardRandom      , m_Rewarder));
	initRewarder(REWARD_CHARACTER_RANDOM, CC_CALLBACK_1(CAchievementRewarder::characterRewardRandom , m_Rewarder));
    initRewarder(REWARD_COSTUME_RANDOM,   CC_CALLBACK_1(CAchievementRewarder::costumeRewardRandom   , m_Rewarder));
	initRewarder(REWARD_ROCKET_RANDOM,	  CC_CALLBACK_1(CAchievementRewarder::rocketRewardRandom    , m_Rewarder));
	initRewarder(REWARD_PET_RANDOM,		  CC_CALLBACK_1(CAchievementRewarder::petRewardRandom       , m_Rewarder));
}

void CAchievementDataManager::addAchievementToList(ACHIEVEMENT_LIST &list, 
											   const Json::Value &data)
{
    if(data["index"].isNull()) return;
    
	ACHIEVEMENT* achievement    = new ACHIEVEMENT();

	achievement->_index         = data["index"].asInt();
    achievement->_openLevel     = data["openLevel"].asInt();
    achievement->_visibleType   = data["visible"].asBool();
	achievement->_type          = (ACHIEVEMENT_TYPE)data["type"].asInt();
    
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
    level._characterIndex   = data["characterIndex"].asInt();
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
    material._itemIndex     = data["itemIndex"].asInt();
    material._materialValue = data["materialValue"].asInt();
    material._isFit         = data["isFit"].asBool();
    material._materialKey   = data["materialKey"].asString();
    auto values             = data["materialValues"].asString();
    auto valueList          = StringUtility::split(values, ',');
    for(auto value : valueList)
        material._materialValues.emplace_back(StringUtility::toNumber<int>(value));
    
    list.emplace_back(material);
}
