#pragma once

#include "cocos2d.h"

namespace GLOBAL{
	static int BONUSTIME;
	static int ALIENGET;
	static int CHALLENGECLEAR;
	static int TOTALSCORE;
    static int STARSCORE;
    static int COINSCORE;
    static int RUNSCORE;
}

class CMyButton;
class CPopup;
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
    void turnDownSound();
    void turnUpSound();
	void initKeyboardListener();

private:
	static CGameScene* m_GameScene;
	static cocos2d::NodeGrid* m_GridWorld;
    CPopup* m_UILayer;
	cocos2d::Size m_VisibleSize;
	bool m_KeyBoardSpace;
};
