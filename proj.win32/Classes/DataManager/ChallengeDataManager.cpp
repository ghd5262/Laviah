#include "ChallengeDataManager.hpp"
#include "UserDataManager.h"
#include "../json/json.h"
#include "../Common/StringUtility.h"

using namespace cocos2d;

const static std::string KEY_FILE_NAME = "challengeKeyList.json";
const static std::string DATA_FILE_NAME = "challengeList.json";

CChallengeDataManager::CChallengeDataManager()
{
    initKeyListWithJson(KEY_FILE_NAME);
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
}

CChallengeDataManager* CChallengeDataManager::Instance()
{
    static CChallengeDataManager instance;
    return &instance;
}

void CChallengeDataManager::initKeyListWithJson(std::string fileName)
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
    
    
    const Json::Value challengeMaterialArray = root["challengeMaterialList"];
    const Json::Value challengeRewardArray = root["challengeRewardList"];
    
    auto initList = [=](KEY_LIST &list, const Json::Value array){
        for(auto key : array)
        {
            list.emplace_back(key.asString());
        }
    };
    
    initList(m_MaterialKeyList, challengeMaterialArray);
    initList(m_RewardKeyList, challengeRewardArray);
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
        
        sCHALLENGE_PARAM challengeInfo;
        
        challengeInfo._index        = list.size();
        challengeInfo._level        = challenge["level"].asInt();
        challengeInfo._oneTime      = challenge["oneTime"].asBool();
        challengeInfo._contents     = challenge["contents"].asString();
        const Json::Value materialArray  = challenge["material"];
        const Json::Value rewardArray    = challenge["reward"];
        
        initList(challengeInfo._materialList, m_MaterialKeyList, materialArray);
        initList(challengeInfo._rewardList,   m_RewardKeyList,   rewardArray);
		initChallengeClearChecker(challengeInfo._materialList);

        list.emplace_back(new sCHALLENGE_PARAM(challengeInfo));
    }
}
//const std::string COIN_SCORE = "COIN_SCORE";
//const std::string STAR_SCORE = "STAR_SCORE";
//const std::string RUN_SCORE = "RUN_SCORE";
//
//const std::string BEST_SCORE = "BEST_SCORE";
//const std::string BEST_COMBO = "BEST_COMBO";
//
//const std::string CHARACTER_COLLECT = "CHARACTER_COLLECT";
//const std::string ROCKET_COLLECT = "ROCKET_COLLECT";
//
//const std::string CHARACTER_COUNT = "CHARACTER_COUNT";
//const std::string ROCKET_COUNT = "ROCKET_COUNT";
//
//const std::string USER_LEVEL = "USER_LEVEL";
//const std::string WORKSHOP_LEVEL = "WORKSHOP_LEVEL";
//
//const std::string COMBO = "COMBO";
//const std::string COIN = "COIN";
//const std::string ITEM_USE = "ITEM_USE";
void CChallengeDataManager::initChallengeClearChecker(MATERIAL_LIST &list)
{
	for (auto mtrl : list)
	{
		switch (str2int<mtrl.first.c_str(), 0>::value)
		{
		case str2int<CHALLENGE_DATA_KEY::COIN_SCORE.c_str(), 0>::value:
		{

		} break;

		}
	}
}

void CChallengeDataManager::UpdateCurrentState(std::string key, int value)
{
	auto state = m_CurrentState.find(key);
	if (state == std::end(m_CurrentState))
		this->addMaterialToCurrentState(key, value);

	state->second += value;

	auto list = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHALLENGE_CUR_LIST);

	for (auto idx : *list){
		if(this->checkCurrentChallengeComplete(idx))
			CUserDataManager::Instance()->setUserData_ItemGet(USERDATA_KEY::CHALLENGE_COM_LIST, idx);
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

void CChallengeDataManager::addMaterialToCurrentState(std::string key, int value)
{
	m_CurrentState.emplace(std::pair<std::string, int>(key, value));
}

bool CChallengeDataManager::checkCurrentChallengeComplete(int index)
{
	auto challengeData = this->getChallengeByIndex(index);
	for (auto mtrl : challengeData->_materialList)
	{
		/*auto state = m_CurrentState.find(mtrl.first);

		if (state == std::end(m_CurrentState))	return false;
		if (state->second < mtrl.second)		return false;*/
	}
	return true;
}
