#pragma once

#include "cocos2d.h"

namespace GLOBAL{
	static int BONUSTIME;
	static int ALIENGET;
	static int CHALLENGECLEAR;
	static int TOTALSCORE;
}

class CMyButton;
class CGameScene : public cocos2d::Layer
{
public:
	CREATE_FUNC(CGameScene);
	static cocos2d::Scene* createScene();
	virtual void update(float delta) override;

	void ResetGameScene();
	void BackToMenuScene();
	void OpenGamePausePopup();
	void GameStart();
    void GamePause();
	void GameResume();
	void ShowResult();
    void GameHelp();
	void WatchVideo();
    void GameExit();
//    void CountDown(int count, std::string finMent = "0", const std::function<void(void)> &func = nullptr);
//    void CountDownCancel();
    
	//getter & setter
	static CGameScene* getGameScene(){ return m_GameScene; }
	static cocos2d::NodeGrid* getGridWorld(){ return m_GridWorld; }

protected:
	virtual bool init() override;

private:
	CGameScene();
	virtual ~CGameScene();

//	void initGameSceneUI();
	void clearData();
	void createPausePopup();
	void createVideoPopup();
	void createResultPopup();
	void createHelpPopup();
    void createExitPopup();
    void turnDownSound();
    void turnUpSound();
//	void onPauseButton();
//	void offPauseButton();
//	void createTestItemButton();
	void initKeyboardListener();

private:
	static CGameScene* m_GameScene;
	static cocos2d::NodeGrid* m_GridWorld;

	cocos2d::Label* m_CountDownLabel;
	cocos2d::Size m_VisibleSize;

	bool m_KeyBoardSpace;
	int m_Count;
};
