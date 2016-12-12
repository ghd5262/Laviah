#pragma once

#include "cocos2d.h"
#include <map>

enum ZORDER{
    BACKGROUND = -1,
    BULLET,
    PLANET,
    PLAYER,
    POPUP,
};

class CMyButton;
class CPopup;
class CCountDown;
class CGameScene : public cocos2d::Layer
{
public:
	CREATE_FUNC(CGameScene);
	static cocos2d::Scene* createScene();
	virtual void update(float delta) override;

//	void ResetGameScene();
//	void BackToMenuScene();
	void OpenGamePausePopup();
    void OpenGameMenuLayer();
	void GameStart();
    void GamePause();
	void GameResume();
	void GameResult();
    void GameHelp();
	void WatchVideo();
    void GameExit();

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
	void createResultPopup();
	void createHelpPopup();
    void createExitPopup();
    void createMenuLayer();
    void createUILayer();
	void createRandomCoin();
    void removeUILayer();
    void turnDownSound();
    void turnUpSound();
	void initKeyboardListener();

private:
	static CGameScene* m_GameScene;
	static cocos2d::NodeGrid* m_GridWorld;
    CPopup* m_UILayer;
    CCountDown* m_CountDown;
	cocos2d::Size m_VisibleSize;
    std::map<std::string, int> m_GlobalVariable;
	bool m_KeyBoardSpace;
};
