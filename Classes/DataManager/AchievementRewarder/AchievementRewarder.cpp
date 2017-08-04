#include "AchievementRewarder.hpp"
#include "../UserDataManager.h"
#include "../AchievementDataManager.hpp"
#include "../WorkshopItemDataManager.h"
#include "../CharacterDataManager.h"
#include "../CostumeDataManager.hpp"
#include "../RocketDataManager.hpp"

sREWARD_DATA CAchievementRewarder::coinReward(sREWARD_DATA data) {
	CCLOG("COIN REWARD %d", data._value);
	CUserDataManager::Instance()->CoinUpdate(data._value);
	data._key = ACHIEVEMENT_REWARD_KEY::REWARD_COIN;

	return data;
}

sREWARD_DATA CAchievementRewarder::characterReward(sREWARD_DATA data) {
	CCLOG("CHARACTER REWARD %d", data._value);
    CUserDataManager::Instance()->setUserData_ItemExist(USERDATA_KEY::CHARACTER_LIST, data._value);
	data._key = ACHIEVEMENT_REWARD_KEY::REWARD_CHARACTER;

	return data;
}

sREWARD_DATA CAchievementRewarder::costumeReward(sREWARD_DATA data) {
    CCLOG("COSTUME REWARD %d", data._value);
    CUserDataManager::Instance()->setUserData_ItemExist(USERDATA_KEY::COSTUME_LIST, data._value);
    data._key = ACHIEVEMENT_REWARD_KEY::REWARD_COSTUME;
    
    return data;
}

sREWARD_DATA CAchievementRewarder::rocketReward(sREWARD_DATA data) {
	CCLOG("ROCKET REWARD %d", data._value);
	CUserDataManager::Instance()->setUserData_ItemExist(USERDATA_KEY::ROCKET_LIST, data._value);
    data._key = ACHIEVEMENT_REWARD_KEY::REWARD_ROCKET;

	return data;
}

sREWARD_DATA CAchievementRewarder::petReward(sREWARD_DATA data) {
	CCLOG("PET REWARD %d", data._value);
	CUserDataManager::Instance()->setUserData_ItemExist(USERDATA_KEY::PET_LIST, data._value);
    data._key = ACHIEVEMENT_REWARD_KEY::REWARD_PET;

	return data;
}

sREWARD_DATA CAchievementRewarder::coinRewardRandom(sREWARD_DATA data) {
	data._value = cocos2d::random<int>(data._value, int(data._value * 1.5f));
    if(cocos2d::random<int>(0, 100) < 3) data._value *= 10;
	CCLOG("COIN REWARD RADNOM %d", data._value);
	return coinReward(data);
}

sREWARD_DATA CAchievementRewarder::characterRewardRandom(sREWARD_DATA data) {
    
	auto characterData = CCharacterDataManager::Instance()->getNewRandomCharacter();
	if (characterData == nullptr)
        return coinRewardRandom(sREWARD_DATA(ACHIEVEMENT_REWARD_KEY::REWARD_COIN_RANDOM, 0));

	data._value = characterData->_index;
    
	CCLOG("CHARACTER REWARD RADNOM %d", data._value);
	return characterReward(data);
}

sREWARD_DATA CAchievementRewarder::costumeRewardRandom(sREWARD_DATA data) {
    
    auto costumeData = CCostumeDataManager::Instance()->getNewRandomCostume();
    if (costumeData == nullptr)
        return coinRewardRandom(sREWARD_DATA(ACHIEVEMENT_REWARD_KEY::REWARD_COIN_RANDOM, 0));
    
    data._value = costumeData->_index;
    
    CCLOG("COSTUME REWARD RADNOM %d", data._value);
    return costumeReward(data);
}

sREWARD_DATA CAchievementRewarder::rocketRewardRandom(sREWARD_DATA data) {

    auto rocketData = CRocketDataManager::Instance()->getNewRandomRocket();
    if (rocketData == nullptr)
        return coinRewardRandom(sREWARD_DATA(ACHIEVEMENT_REWARD_KEY::REWARD_COIN_RANDOM, 0));
    
    data._value = rocketData->_idx;

	CCLOG("ROCKET REWARD RADNOM %d", data._value);
	return rocketReward(data);
}

sREWARD_DATA CAchievementRewarder::petRewardRandom(sREWARD_DATA data) {

	CCLOG("PET REWARD RADNOM %d", data._value);
	return petReward(data);
}
