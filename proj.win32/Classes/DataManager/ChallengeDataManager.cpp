#include "ChallengeDataManager.hpp"
#include "UserDataManager.h"
#include "../json/json.h"

using namespace cocos2d;

const static std::string FILE_NAME = "challengeKeyList.json";
const static std::string FILE_NAME = "challengeList.json";

CChallengeDataManager::CChallengeDataManager()
{
    initWithJson(m_CallengeDataList, FILE_NAME);
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

	auto initMaterial = [=](sCHALLENGE_PARAM& data, std::string key, const Json::Value value){
		data._materialList.emplace(STD_PAIR(key, value[key].asInt()));
	};

	auto initReward = [=](sCHALLENGE_PARAM& data, std::string key, const Json::Value value){
		data._rewardList.emplace(STD_PAIR(key, value[key].asInt()));
	};

    for (unsigned int count = 0; count < challengeArray.size(); ++count)
    {
        const Json::Value challenge = challengeArray[count];
        
        sCHALLENGE_PARAM challengeInfo;
        
        challengeInfo._index        = list.size();
        challengeInfo._level        = challenge["level"].asInt();
        challengeInfo._oneTime      = challenge["oneTime"].asBool();
        challengeInfo._contents     = challenge["contents"].asString();
        const Json::Value materialList  = challenge["material"];
        const Json::Value rewardList    = challenge["reward"];
        
		initMaterial(challengeInfo, "coinScore", materialList);
		initMaterial(challengeInfo, "starScore", materialList);
		initMaterial(challengeInfo, "runScore", materialList);
		initMaterial(challengeInfo, "bestScore", materialList);
		initMaterial(challengeInfo, "bestCombo", materialList);
		initMaterial(challengeInfo, "combo", materialList);
		initMaterial(challengeInfo, "characterCount", materialList);

		initReward(challengeInfo, "coin", rewardList);
        
        list.emplace_back(new sCHALLENGE_PARAM(challengeInfo));
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
		auto state = m_CurrentState.find(mtrl.first);

		if (state == std::end(m_CurrentState))	return false;
		if (state->second < mtrl.second)		return false;
	}
	return true;
}
