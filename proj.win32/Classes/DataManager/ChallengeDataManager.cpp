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
        
        sCHALLENGE_PARAM* challengeInfo = new sCHALLENGE_PARAM();
        
        challengeInfo->_index        = list.size();
        challengeInfo->_level        = challenge["level"].asInt();
        challengeInfo->_oneTime      = challenge["oneTime"].asBool();
        challengeInfo->_contents     = challenge["contents"].asString();
        const Json::Value materialArray  = challenge["material"];
        const Json::Value rewardArray    = challenge["reward"];
        
        initList(challengeInfo->_materialList, m_MaterialKeyList, materialArray);
        initList(challengeInfo->_rewardList,   m_RewardKeyList,   rewardArray);
		this->initChallengeClearChecker(challengeInfo);

        list.emplace_back(challengeInfo);
    }
}

void CChallengeDataManager::initChallengeClearChecker(sCHALLENGE_PARAM* data)
{
    auto addChecker = [=](CChallengeClearChecker* checker, int mtrlValue){
        data->_challengeCheckerList.emplace_back(CHECKER(checker));
        checker->setMtrlValue(mtrlValue);
    };
    
	for (auto mtrl : data->_materialList)
	{
		switch (str2int(mtrl.first.c_str()))
		{
            case str2int(COIN_SCORE):           addChecker(new coinScoreCheck(),        mtrl.second); break;
            case str2int(STAR_SCORE):           addChecker(new starScoreCheck(),        mtrl.second); break;
            case str2int(RUN_SCORE):            addChecker(new runScoreCheck(),         mtrl.second); break;
            case str2int(BEST_SCORE):           addChecker(new bestScoreCheck(),        mtrl.second); break;
            case str2int(BEST_COMBO):           addChecker(new bestComboCheck(),        mtrl.second); break;
            case str2int(CHARACTER_COLLECT):    addChecker(new characterCollectCheck(), mtrl.second); break;
            case str2int(ROCKET_COLLECT):       addChecker(new rocketCollectCheck(),    mtrl.second); break;
            case str2int(CHARACTER_COUNT):      addChecker(new characterCountCheck(),   mtrl.second); break;
            case str2int(ROCKET_COUNT):         addChecker(new rocketCountCheck(),      mtrl.second); break;
            case str2int(USER_LEVEL):           addChecker(new userLevelCheck(),        mtrl.second); break;
            case str2int(WORKSHOP_LEVEL):       addChecker(new workshopLevelCheck(),    mtrl.second); break;
            case str2int(COMBO):                addChecker(new comboCheck(),            mtrl.second); break;
            case str2int(COIN):                 addChecker(new coinCheck(),             mtrl.second); break;
            case str2int(ITEM_USE):             addChecker(new itemUseCheck(),          mtrl.second); break;
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
    
    if(challengeData->_challengeCheckerList.size() == 0) return false;
    
	for (auto checker : challengeData->_challengeCheckerList)
	{
        if(!checker->Check())
            return false;
	}
	return true;
}
