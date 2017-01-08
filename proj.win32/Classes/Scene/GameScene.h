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
    
public:
	CREATE_FUNC(CGameScene);
	static cocos2d::Scene* createScene();
	virtual void update(float delta) override;

	void OpenGamePausePopup();
    void OpenGameMenuLayer();
	void GameStart();
    void GamePause();
	void GameResume();
	void GameResult();
    void GameHelp();
	void WatchVideo();
	void ShowChallenge();
    void GameExit();
    void RandomCoin();
    void BonusTimeStart();
    void BonusTimeEnd();
    void ScreenFade(const FADE_CALLBACK& callback = nullptr);
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
	void createHelpPopup();
    void createExitPopup();
    void createMenuLayer();
    void createUILayer();
    void createBonusTimeLayer();
	void createRandomCoin();
    void removeUILayer();
    void removeBonusTimeLayer();
    void turnDownSound();
    void turnUpSound();
	void initKeyboardListener();

private:
	static CGameScene* m_GameScene;
	static cocos2d::NodeGrid* m_GridWorld;
    cocos2d::LayerColor* m_ScreenFade;
    CPopup* m_UILayer;
    CPopup* m_BonusTimeLayer;
    CCountDown* m_CountDown;
	cocos2d::Size m_VisibleSize;
	bool m_KeyBoardSpace;
};
