#pragma once
#include <vector>
#include "../Common/HSHUtility.h"
#include "../AI/StateMachine.h"
#include "../DataManager/CharacterDataManager.h"
#include "../DataManager/RocketDataManager.hpp"
/*------------------------------ObjectManager 클래스설명----------------------------------
*
* CMover를 상속받는 모든 클래스를 Execute및 Remove하는 함수이다.
* 현재 주의해야 할 사항은 Remove함수가 Delete함수를 호출하는 구조이다.
* 이유는 Delete에서 removeFromParent()와 operator delete를 호출하여 소멸자를 호출하기 위함
*
*----------------------------------------------------------------------------------------*/
class CBullet;
class CPlanet;
class CPlayer;
class CRocket;
class CBackGround;
class CBulletCreator;
class CItemManager;
class CBulletPatternDataManager;
class CItemRange;
class CObjectManager
{
    // 회전 가속도 
    const float ROTATE_ACCEL_MAX = 0.5f;
public:
    static CObjectManager* Instance();
    
#if(USE_MEMORY_POOLING)
    void AddBullet(void* bullet);
#else
    void AddBullet(CBullet* bullet);
#endif
    void Execute(float delta);
	void RotationObject(float dir);
    void BonusTimeTouchEvent(float dir);
    void SpeedControl(float duration, float speed);
    void ChangeCharacter();
    void ChangeRocket();
    void ChangeState(CState<CObjectManager>* newState)
    { m_FSM->ChangeState(newState); };
    void StartBonusTime();
    void EndBonusTime();
    void ZoomIn();
    void ZoomIn2();
    void ZoomOut();
    void InitTutorialStep();
    void Share();

    
	// 초기화
	void Clear();
    
    // 게임 종료 시점에 호출된다. RemoveAllBullet(), RemoveAllShooter() 호출함
    void RemoveAllObject();
    void ReturnToMemoryBlockAll();

	//getter & setter
    std::vector<CBullet*>* getBulletList(){ return &m_BulletList; }
    CC_SYNTHESIZE(CPlanet*, m_Planet, Planet);
	CC_SYNTHESIZE(CPlayer*, m_Player, Player);
    CC_SYNTHESIZE(CRocket*, m_Rocket, Rocket);
    CC_SYNTHESIZE(CBulletCreator*, m_BulletCreator, BulletCreator);
	CC_SYNTHESIZE(CBackGround*, m_Background, Background);
	CC_SYNTHESIZE(CItemRange*, m_BarrierItemRange, BarrierItemRange);
	CC_SYNTHESIZE(CItemRange*, m_StarItemRange, StarItemRange);
	CC_SYNTHESIZE(CItemRange*, m_CoinItemRange, CoinItemRange);
	CC_SYNTHESIZE(float, m_PatternTimer, PatternTimer);
    CC_SYNTHESIZE(float, m_LevelTimer, LevelTimer);
    CC_SYNTHESIZE(float, m_Delta, Delta);
	CC_SYNTHESIZE(bool, m_IsGamePause, IsGamePause);
	CC_SYNTHESIZE(const sCHARACTER_PARAM*, m_CharacterParam, CharacterParam);
    CC_SYNTHESIZE(const sROCKET_PARAM*, m_RocketParam, RocketParam);
    CC_SYNTHESIZE(int, m_GameLevel, GameLevel);
    
private:
	void createBulletByTimer(float delta);
    void inGameUpdate(float delta);
    void inMenuUpdate(float delta);
    void inBonusGameUpdate();
    void removeAllBullet();	// Delete함수 호출! 이유는 구현부에~
    void bulletListExecute();
    void bulletListRotate();
    void setGameLevelByTimer();
    void zoom(cocos2d::Node* obj,
              cocos2d::Vec2 zoomPos,
              float zoomSize);
    
    CObjectManager();
	~CObjectManager(){};
    
    struct sLEVEL_BALANCE{
        float _time;
        int   _level;
        bool  _below;
        sLEVEL_BALANCE()
        : _time(0.f)
        , _level(0)
        , _below(false){}
        
        sLEVEL_BALANCE(float time, int level, bool below = false)
        : _time(time)
        , _level(level)
        , _below(below){}
    };

private:
    std::shared_ptr<CStateMachine<CObjectManager>> m_FSM;
	std::vector<CBullet*> m_BulletList;
    std::vector<sLEVEL_BALANCE> m_LevelList;
    cocos2d::Node* m_SpeedController;
    CItemManager* m_ItemManager;
    CBulletPatternDataManager* m_PatternManager;
    float m_RotationSpeed;
};