#include "AchievementRewarder.hpp"
#include "../UserDataManager.h"
#include "../AchievementDataManager.hpp"
#include "../WorkshopItemDataManager.h"
#include "../CharacterDataManager.h"
#include "../RocketDataManager.hpp"

sREWARD_DATA CAchievementRewarder::coinReward(sREWARD_DATA data) {
	CCLOG("COIN REWARD %d", data._value);
	CUserDataManager::Instance()->CoinUpdate(data._value);
	data._key = ACHIEVEMENT_REWARD_KEY::REWARD_COIN;

	return data;
}

sREWARD_DATA CAchievementRewarder::characterReward(sREWARD_DATA data) {
	CCLOG("CHARACTER REWARD %d", data._value);
	CUserDataManager::Instance()->setUserData_ItemGet(USERDATA_KEY::CHARACTER_LIST, data._value);
	data._key = ACHIEVEMENT_REWARD_KEY::REWARD_CHARACTER;

	return data;
}

sREWARD_DATA CAchievementRewarder::RocketReward(sREWARD_DATA data) {
	CCLOG("ROCKET REWARD %d", data._value);
	CUserDataManager::Instance()->setUserData_ItemGet(USERDATA_KEY::ROCKET_LIST, data._value);
	return data;
}

sREWARD_DATA CAchievementRewarder::PetReward(sREWARD_DATA data) {
	CCLOG("PET REWARD %d", data._value);
	CUserDataManager::Instance()->setUserData_ItemGet(USERDATA_KEY::PET_LIST, data._value);
	return data;
}

sREWARD_DATA CAchievementRewarder::coinRewardRandom(sREWARD_DATA data) {
	data._value = cocos2d::random<int>(data._value, data._value + 50);
	CCLOG("COIN REWARD RADNOM %d", data._value);
	return coinReward(data);
}

sREWARD_DATA CAchievementRewarder::characterRewardRandom(sREWARD_DATA data) {
    
	auto characterData = CCharacterDataManager::Instance()->getNewRandomCharacter();
	if (characterData == nullptr) return coinRewardRandom(sREWARD_DATA(ACHIEVEMENT_REWARD_KEY::REWARD_COIN_RANDOM, 0));

	data._value = characterData->_idx;
    
	CCLOG("CHARACTER REWARD RADNOM %d", data._value);
	return characterReward(data);
}

sREWARD_DATA CAchievementRewarder::RocketRewardRandom(sREWARD_DATA data) {

    auto rocketData = CRocketDataManager::Instance()->getNewRandomRocket();
    if (rocketData == nullptr) return coinRewardRandom(sREWARD_DATA(ACHIEVEMENT_REWARD_KEY::REWARD_COIN_RANDOM, 0));
    
    data._value = rocketData->_idx;

	CCLOG("ROCKET REWARD RADNOM %d", data._value);
	return RocketReward(data);
}

sREWARD_DATA CAchievementRewarder::PetRewardRandom(sREWARD_DATA data) {

	CCLOG("PET REWARD RADNOM %d", data._value);
	return PetReward(data);
}