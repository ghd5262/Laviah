#include "GameStates.h"
#include "../../GameObject/ItemManager.h"
#include "../../GameObject/ObjectManager.h"
#include "../../GameObject/Player.h"
#include "../../DataManager/BulletPatternDataManager.h"
#include "../../MyUI/BonusTimeUI.h"
#include "../../Scene/GameScene.h"


void CNormalState::Enter(CObjectManager* objectMng){}

void CNormalState::Execute(CObjectManager* objectMng, float delta) {}

void CNormalState::Exit(CObjectManager* objectMng){}


void CBonusTimeState::Enter(CObjectManager* objectMng){}

void CBonusTimeState::Execute(CObjectManager* objectMng, float delta) {}

void CBonusTimeState::Exit(CObjectManager* objectMng){}
