#pragma once
#include <iostream>
#include "cocos2d.h"
#include "PluginSdkboxPlay/PluginSdkboxPlay.h"
#include <vector>
#include <map>

struct LEADERBOARD{
    int _rank;
    int _dailyScore;
    int _allTimeScore;
    
    LEADERBOARD()
    : _rank(0)
    , _dailyScore(0)
    , _allTimeScore(0){}
};

typedef std::function<void(void)>           VOID_LISTENER;
typedef std::function<void(std::string)>    DATA_LISTENER;
typedef std::map<std::string, LEADERBOARD*> LEADERBOARDS;
class CPlayManager : public sdkbox::SdkboxPlayListener
{
public:
    static CPlayManager* Instance();
    void Initialize();
    void Login(VOID_LISTENER listener);
    void Logout(VOID_LISTENER listener);
    bool IsLoggedIn();
    void DataLoad(DATA_LISTENER listener, std::string key);
    void DataSave(DATA_LISTENER listener, std::string key, std::string data);
    void OpenLeaderboard();
    void OpenAchievement();
    void ScoreSave(VOID_LISTENER listener, std::string key, int score);
    void ScoreLoad(VOID_LISTENER listener, std::string key,
                   int score, int time_span);
    bool IsNewHighScore(std::string key, int score, int time_span);
    void AchievementComplete(std::string key);
    
    LEADERBOARD* getLeaderboardData(std::string key);
    
    CC_SYNTHESIZE(VOID_LISTENER, m_LoginListener,     LoginListener);
    CC_SYNTHESIZE(VOID_LISTENER, m_LogoutListener,    LogoutListener);
    CC_SYNTHESIZE(VOID_LISTENER, m_SaveScoreListener, SaveScoreListener);
    CC_SYNTHESIZE(VOID_LISTENER, m_LoadScoreListener, LoadScoreListener);
    CC_SYNTHESIZE(DATA_LISTENER, m_DataLoadListener,  DataLoadListener);
    CC_SYNTHESIZE(DATA_LISTENER, m_DataSaveListener,  DataSaveListener);

private:
    void callVoidListener(VOID_LISTENER& listener);
    void callDataListener(DATA_LISTENER& listener, std::string data);
    void saveScoreToMap(std::string key, int score,
                        int time_span, int rank = 0);
    
    // Callbacks
    virtual void onConnectionStatusChanged(int connection_status) override;
    
    virtual void onScoreSubmitted(const std::string& leaderboard_name,
                                  long score,
                                  bool maxScoreAllTime,
                                  bool maxScoreWeek,
                                  bool maxScoreToday) override;
    
    virtual void onIncrementalAchievementUnlocked(const std::string& achievement_name) override;
    
    virtual void onIncrementalAchievementStep(const std::string& achievement_name,
                                              double step ) override;
    
    virtual void onIncrementalAchievementStepError(const std::string& name,
                                                   double steps,
                                                   int error_code,
                                                   const std::string& error_description ) override;
    
    virtual void onAchievementUnlocked(const std::string& achievement_name,
                                       bool newlyUnlocked ) override;
    
    virtual void onAchievementUnlockError(const std::string& achievement_name,
                                          int error_code,
                                          const std::string& error_description ) override;
    
    virtual void onAchievementsLoaded(bool reload_forced,
                                      const std::string& json_achievements_info ) override;
    
    virtual void onSetSteps(const std::string& name,
                            double steps) override;
    
    virtual void onSetStepsError(const std::string& name,
                                 double steps,
                                 int error_code,
                                 const std::string& error_description) override;
    
    virtual void onReveal(const std::string& name) override;
    
    virtual void onRevealError(const std::string& name,
                               int error_code,
                               const std::string& error_description) override;
    
    virtual void onMyScore(const std::string& leaderboard_name,
                           int time_span,
                           int collection_type,
                           long score) override;
    
    virtual void onMyScoreError(const std::string& leaderboard_name,
                                int time_span,
                                int collection_type,
                                int error_code,
                                const std::string& error_description) override;
    
    virtual void onPlayerCenteredScores(const std::string& leaderboard_name,
                                        int time_span,
                                        int collection_type,
                                        const std::string& jsonData) override;
    
    virtual void onPlayerCenteredScoresError(const std::string& leaderboard_name,
                                             int time_span,
                                             int collection_type,
                                             int error_code,
                                             const std::string& error_description) override;
    
    virtual void onGameData(const std::string& action,
                            const std::string& name,
                            const std::string& data,
                            const std::string& error) override;
    
    
    CPlayManager();
    virtual ~CPlayManager();
    
private:
    LEADERBOARDS m_Leaderboards;
};
