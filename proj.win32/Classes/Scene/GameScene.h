#pragma once

#include "cocos2d.h"
#include <map>

enum ZORDER{
    BACKGROUND = -1,
    BULLET,
    PLANET,
    PLAYER,
    POPUP,
    SCREENFADE,
};

class CMyButton;
class CPopup;
class CCountDown;
class CGameScene : public cocos2d::Layer
{
    typedef std::function<void(void)> FADE_CALLBACK;
	typedef std::function<void(cocos2d::Node*)> NODE_CALLBACK;
    
public:
	CREATE_FUNC(CGameScene);
	static cocos2d::Scene* createScene();
	virtual void update(float delta) override;

	void OpenGamePausePopup();
    void OpenGameMenuLayer();
    void OpenOptionPopup(int scrollIndex = 0);
    void OpenWorkshopPopup();
    void OpenCharacterSelectPopup();
	void GameStart();
    void GamePause();
	void GameResume();
	void GameResult();
    void GameEnd();
    void GameHelp();
	void WatchVideo();
	void ShowChallenge();
    void GameExit(bool resume = false);
    void RandomCoin();
    void BonusTimeStart();
    void BonusTimeEnd();
    void ScreenFade(const FADE_CALLBACK& callback = nullptr);
	CPopup* CreateAlertPopup();
	CPopup* Reward();

	//getter & setter
	static CGameScene* getGameScene(){ return m_GameScene; }
	static cocos2d::NodeGrid* getGridWorld(){ return m_GridWorld; }

	CC_SYNTHESIZE(cocos2d::Vec2, m_TouchPos, TouchPos);

protected:
	virtual bool init() override;

private:
	CGameScene();
	virtual ~CGameScene();

	void clearData();
    void cleanGlobalData();
	void createPausePopup();
	void createVideoPopup();
	void createChallengePopup();
	void createResultPopup();
    void createEndPopup();
	void createHelpPopup();
    void createExitPopup(bool resume);
    void createOptionPopup(int index);
    void createWorkshopPopup();
    void createCharacterSelectPopup();
    void createBonusTimeLayer();
	void createRandomCoin();
    void removeBonusTimeLayer();
    void turnDownSound();
    void turnUpSound();
	void initKeyboardListener();
    
    // The following items are initialized only once.
    void initMemoryPool();
    void createBulletCreator();
    void createBackground();
    void createPlanet();
    void createPlayer();
    void createRocket();
    void createCountDown();
    void createScreenFade();
    void createItemRanges();
    void createComboUI();
    void createMenuLayer();
    void createUILayer();
    void createTutorialLayer();

private:
	static CGameScene* m_GameScene;
	static cocos2d::NodeGrid* m_GridWorld;
    cocos2d::LayerColor* m_ScreenFade;
    CPopup* m_UILayer;
    CPopup* m_MenuLayer;
    CPopup* m_BonusTimeLayer;
    CPopup* m_TutorialLayer;
    CCountDown* m_CountDown;
	cocos2d::Size m_VisibleSize;
	bool m_KeyBoardSpace;
};
