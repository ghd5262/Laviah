#pragma once

#include "cocos2d.h"
#include <map>

enum ZORDER{
    BACKGROUND = -1,
    BULLET,
    PLANETZ,
    PLAYER,
    POPUP,
    SCREENFADE = 100,
};

struct sREWARD_DATA;
class CMyButton;
class CPopup;
class CCountDown;
class CGameScene : public cocos2d::Layer
{
    typedef std::function<void(void)>           VOID_CALLBACK;
	typedef std::function<void(cocos2d::Node*)> NODE_CALLBACK;
    
public:
	CREATE_FUNC(CGameScene);
	static cocos2d::Scene* createScene();
	virtual void update(float delta) override;

    void GameStart();
    void GamePause();
    void GameResume();
    void GameResult();
    void StageEnd();
    void GameEnd();
    void GameHelp();
    void GameExit(bool resume = false);
    
	void OpenGamePausePopup();
    void OpenGameMenuLayer();
    void OpenOptionPopup(int scrollIndex = 0);
    void OpenWorkshopPopup();
    void OpenCharacterPopup();
    void OpenCostumePopup(const VOID_CALLBACK& callback,
                          int index);
    void OpenRankPopup();
    void OpenRankUpPopup();
    void OpenPermRequestPopup(const VOID_CALLBACK& callback);
    void OpenFBTestPopup();
    void OpenAchievementPopup();
    void OpenGoalPopup();
    void OpenVideoPopup();
    void OpenSharePopup(const std::function<void(void)>& callback,
                        cocos2d::Texture2D* texture,
                        int sizeType, bool logo,
                        int score = 0);
    void OpenDownloadPopup();
    
    void OpenPlanetSelectPopup();
    
    void RandomCoin();
    void BonusTimeStart();
    void BonusTimeEnd();
    void ScreenFade(const VOID_CALLBACK& callback = nullptr);
    void MenuFadeIn();
    void MenuFadeOut();
    
    void turnDownSound();
    void turnUpSound();
    void pauseSound();
    void resumeSound();
    
    void getFreeReward();
    void getRankReward();
    
	CPopup* CreateAlertPopup();
	void Reward(std::function<void(bool)> exitCallback,
                std::vector<sREWARD_DATA> list,
                std::string title,
                int cost = 0,
                bool ufoEnable = false);

	//getter & setter
	static CGameScene* getGameScene(){ return m_GameScene; }
    static cocos2d::Layer* getEffectLayer(){ return m_EffectLayer; }
	static cocos2d::Layer* getZoomLayer(){ return m_ZoomLayer; }
    static cocos2d::Layer* getPopupLayer(){ return m_PopupLayer; }
    
	CC_SYNTHESIZE(cocos2d::Vec2, m_TouchPos, TouchPos);
    CC_SYNTHESIZE(bool, m_NeedTutorial, NeedTutorial);
    CC_SYNTHESIZE(bool, m_NeedReview, NeedReview);
    CC_SYNTHESIZE(bool, m_IsMenuLayerFront, IsMenuLayerFront);
    CC_SYNTHESIZE(bool, m_RewardAble, RewardAble);
    CC_SYNTHESIZE(bool, m_CheckBox, CheckBox);
    CC_SYNTHESIZE(long, m_DailyResetRemain, DailyResetRemain);
    CC_SYNTHESIZE(long, m_WeeklyResetRemain, WeeklyResetRemain);
    CC_SYNTHESIZE(long, m_GameStartTime, GameStartTime);
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
	void createResultPopup(bool isStageEnd = false);
    void createEndPopup();
	void createHelpPopup();
    void createExitPopup(bool resume);
    void createOptionPopup(int index);
    void createWorkshopPopup();
    void createCharacterPopup();
    void createCostumePopup(const VOID_CALLBACK& callback,
                            int index);
    void createPlanetSelectPopup();

    void createRankPopup();
    void createRankUpPopup();
    void createAchievementPopup();
    void createBonusTimeLayer();
	void createRandomCoin();
    void menuOpen();
    void removeBonusTimeLayer();
    void startTutorial();
    void freeRewardCheck();
    void dailyGoalResetCheck(bool serverCall = false);
    void facebookRankingResetCheck();
    void startAppreciatePlanet();
    void stopAppreciatePlanet();
    void createReviewPopup();

    // The following items are initialized only once.
    void initMemoryPool();
    void createFacebookManager();
    void createPopupLayer();
    void createEffectLayer();
    void createZoomLayer();
    void createBulletCreator();
    void createBackground();
    void createPlanet();
    void createPlayer();
    void createRocket();
    void createSlowPoint();
    void createCountDown();
    void createScreenFade();
    void createItemRanges();
    void createComboUI();
    void createMenuLayer();
    void createUILayer();
    void createRivalRankLayer();
    void createTutorialLayer();
    void createIntroUI();
    void createCaptureNode();
    void createBackKeyButton();
    void initKeyboardListener();
    void intro();
    
private:
	static CGameScene* m_GameScene;
    static cocos2d::Layer* m_EffectLayer;
	static cocos2d::Layer* m_ZoomLayer;
    static cocos2d::Layer* m_PopupLayer;
    cocos2d::LayerColor* m_ScreenFade;
    CPopup* m_UILayer;
    CPopup* m_MenuLayer;
    CPopup* m_BonusTimeLayer;
    CPopup* m_TutorialLayer;
    CPopup* m_RivalRankLayer;
    CCountDown* m_CountDown;
	cocos2d::Size m_VisibleSize;
    std::vector<cocos2d::Sprite*> m_IntroUIList;
    bool m_KeyBoardSpace;
    bool m_FirstCountDown;
    bool m_IntroTouchEnable;
};
