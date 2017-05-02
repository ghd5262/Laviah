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
    void OpenRankPopup();
    void OpenRankUpPopup();
    void OpenFBTestPopup();
    void OpenAchievementPopup();
    void GameStart();
    void GamePause();
	void GameResume();
	void GameResult();
    void GameEnd();
    void GameHelp();
	void WatchVideo();
	void ShowAchievement();
    void GameExit(bool resume = false);
    void RandomCoin();
    void BonusTimeStart();
    void BonusTimeEnd();
    void ScreenFade(const FADE_CALLBACK& callback = nullptr);
    void MenuFadeIn();
    void MenuFadeOut();
    
	CPopup* CreateAlertPopup();
	CPopup* Reward();

	//getter & setter
	static CGameScene* getGameScene(){ return m_GameScene; }
	static cocos2d::Layer* getZoomLayer(){ return m_ZoomLayer; }

	CC_SYNTHESIZE(cocos2d::Vec2, m_TouchPos, TouchPos);
    CC_SYNTHESIZE(bool, m_NeedTutorial, NeedTutorial);
    
protected:
	virtual bool init() override;

private:
	CGameScene();
	virtual ~CGameScene();

	void clearData();
    void cleanGlobalData();
	void createPausePopup();
	void createVideoPopup();
	void createGoalPopup();
	void createResultPopup();
    void createEndPopup();
	void createHelpPopup();
    void createExitPopup(bool resume);
    void createOptionPopup(int index);
    void createWorkshopPopup();
    void createCharacterSelectPopup();
    void createRankPopup();
    void createRankUpPopup();
    void createAchievementPopup();
    void createBonusTimeLayer();
	void createRandomCoin();
    void removeBonusTimeLayer();
    void turnDownSound();
    void turnUpSound();
	void initKeyboardListener();
    void startTutorial();
    void getFreeReward();
    
    // The following items are initialized only once.
    void initMemoryPool();
    void createFacebookManager();
    void createZoomLayer();
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
    void createRivalRankLayer();
    void createTutorialLayer();
    void setTimestamp();
    
private:
	static CGameScene* m_GameScene;
	static cocos2d::Layer* m_ZoomLayer;
    cocos2d::LayerColor* m_ScreenFade;
    CPopup* m_UILayer;
    CPopup* m_MenuLayer;
    CPopup* m_BonusTimeLayer;
    CPopup* m_TutorialLayer;
    CPopup* m_RivalRankLayer;
    CCountDown* m_CountDown;
	cocos2d::Size m_VisibleSize;
	bool m_KeyBoardSpace;
};
