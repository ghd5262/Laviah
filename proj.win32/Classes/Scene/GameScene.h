#pragma once

#include "cocos2d.h"
#include <map>
namespace GLOBAL{
    static std::string BONUSTIME        = "BONUSTIME";
	static std::string ALIENGET         = "ALIENGET";
	static std::string CHALLENGECLEAR   = "CHALLENGECLEAR";
	static std::string TOTALSCORE       = "TOTALSCORE";
    static std::string STARSCORE        = "STARSCORE";
    static std::string COINSCORE        = "COINSCORE";
    static std::string RUNSCORE         = "RUNSCORE";
}

enum ZORDER{
    BACKGROUND = -1,
    BULLET,
    PLANET,
    PLAYER,
    POPUP,
};

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
    
    int  getGlobalValue(std::string key);
    void setGlobalValue(std::string key, int value);
    void addGlobalValue(std::string key, int value);
    
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
    void removeUILayer();
    void turnDownSound();
    void turnUpSound();
	void initKeyboardListener();

private:
	static CGameScene* m_GameScene;
	static cocos2d::NodeGrid* m_GridWorld;
    CPopup* m_UILayer;
	cocos2d::Size m_VisibleSize;
    std::map<std::string, int> m_GlobalVariable;
	bool m_KeyBoardSpace;
};
