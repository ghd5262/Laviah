#pragma once

#include "cocos2d.h"

class CUIManager;
class CMyButton;
class CPlanet;
class CPlayer;
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
	void GamePause();
	void GameResume();
	void GameEnd();

	//getter & setter
	static CGameScene* getGameScene(){ return m_GameScene; }
	static cocos2d::NodeGrid* getGridWorld(){ return m_GridWorld; }

protected:
	virtual bool init() override;

private:
	void countDownLabel();
	bool initVariable();
	void InitGameSceneUI();
	void menuCloseCallback(cocos2d::Ref* pSender);

private:
	static CGameScene* m_GameScene;
	static cocos2d::NodeGrid* m_GridWorld;

	CMyButton* m_PauseBtn;
	cocos2d::Label* m_CountDownLabel;
	
	bool m_KeyBoardL;
	bool m_KeyBoardR;
	int m_Count;
};
