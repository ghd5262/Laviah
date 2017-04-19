#include "ChallengeRewarder.hpp"
#include "../UserDataManager.h"
#include "../ChallengeDataManager.hpp"
#include "../WorkshopItemDataManager.h"
#include "../CharacterDataManager.h"
#include "../RocketDataManager.hpp"

sREWARD_DATA CChallengeRewarder::coinReward(sREWARD_DATA data) {
	CCLOG("COIN REWARD %d", data._value);
	CUserDataManager::Instance()->CoinUpdate(data._value);
	data._key = CHALLENGE_REWARD_KEY::REWARD_COIN;

	return data;
}

sREWARD_DATA CChallengeRewarder::characterReward(sREWARD_DATA data) {
	CCLOG("CHARACTER REWARD %d", data._value);
	CUserDataManager::Instance()->setUserData_ItemGet(USERDATA_KEY::CHARACTER_LIST, data._value);
	data._key = CHALLENGE_REWARD_KEY::REWARD_CHARACTER;

	return data;
}

sREWARD_DATA CChallengeRewarder::RocketReward(sREWARD_DATA data) {
	CCLOG("ROCKET REWARD %d", data._value);
	CUserDataManager::Instance()->setUserData_ItemGet(USERDATA_KEY::ROCKET_LIST, data._value);
	return data;
}

sREWARD_DATA CChallengeRewarder::PetReward(sREWARD_DATA data) {
	CCLOG("PET REWARD %d", data._value);
	CUserDataManager::Instance()->setUserData_ItemGet(USERDATA_KEY::PET_LIST, data._value);
	return data;
}

sREWARD_DATA CChallengeRewarder::coinRewardRandom(sREWARD_DATA data) {
	data._value = cocos2d::random<int>(data._value, data._value + 100);
	CCLOG("COIN REWARD RADNOM %d", data._value);
	return coinReward(data);
}

sREWARD_DATA CChallengeRewarder::characterRewardRandom(sREWARD_DATA data) {
    
	auto characterData = CCharacterDataManager::Instance()->getNewRandomCharacter();
	if (characterData == nullptr) return coinRewardRandom(sREWARD_DATA(CHALLENGE_REWARD_KEY::REWARD_COIN_RANDOM, 0));

	data._value = characterData->_idx;
    
	CCLOG("CHARACTER REWARD RADNOM %d", data._value);
	return characterReward(data);
}

sREWARD_DATA CChallengeRewarder::RocketRewardRandom(sREWARD_DATA data) {

    auto rocketData = CRocketDataManager::Instance()->getNewRandomRocket();
    if (rocketData == nullptr) return coinRewardRandom(sREWARD_DATA(CHALLENGE_REWARD_KEY::REWARD_COIN_RANDOM, 0));
    
    data._value = rocketData->_idx;

	CCLOG("ROCKET REWARD RADNOM %d", data._value);
	return RocketReward(data);
}

sREWARD_DATA CChallengeRewarder::PetRewardRandom(sREWARD_DATA data) {

	CCLOG("PET REWARD RADNOM %d", data._value);
	return PetReward(data);
}