#pragma once

#include "cocos2d.h"

class CMyButton;
class CGameScene : public cocos2d::Layer
{
public:
	CREATE_FUNC(CGameScene);
	static cocos2d::Scene* createScene();
	virtual ~CGameScene();

	virtual void update(float delta) override;

	void resetGameScene();
	void backToMenuScene();
	void EnableTouch();
	void AbleTouch();
	void GameStart();
    void OpenGamePausePopup();
	void GameResume();
	void GameEnd();
    void GameExit();
    void GameHelp();
	void watchVideo();
    void CountDown(int count, std::string finMent = "0", const std::function<void(void)> &func = nullptr);
    void CountDownCancel();
    
	//getter & setter
	static CGameScene* getGameScene(){ return m_GameScene; }
	static cocos2d::NodeGrid* getGridWorld(){ return m_GridWorld; }

protected:
	virtual bool init() override;

private:
	void clearData();
    void GamePause();
	bool initVariable();
	void InitGameSceneUI();

private:
	static CGameScene* m_GameScene;
	static cocos2d::NodeGrid* m_GridWorld;

	CMyButton* m_PauseBtn;
	cocos2d::Label* m_CountDownLabel;
	
	bool m_KeyBoardL;
	bool m_KeyBoardR;
	int m_Count;
};
