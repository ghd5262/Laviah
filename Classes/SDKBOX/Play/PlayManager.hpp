#pragma once
#include "PluginSdkboxPlay/PluginSdkboxPlay.h"
//#include "../../Common/HSHUtility.h"
//#include "../../json/json.h"
#include <vector>

class CPlayManager : public cocos2d::Node, sdkbox::SdkboxPlayListener
{
public:
    static CPlayManager* Instance();
    void Initialize();
    void Login();
    bool IsLoggedIn();
    void DataLoad(std::string key);
    void DataSave(std::string key, std::string data);
    void ClearData();
    
protected:
    virtual bool init() override;
    
private:
    // Callbacks
    virtual void onConnectionStatusChanged(int connection_status) override;
    
    virtual void onScoreSubmitted(const std::string& leaderboard_name,
                                  long score,
                                  bool maxScoreAllTime,
                                  bool maxScoreWeek,
                                  bool maxScoreToday) override;
    
    virtual void onIncrementalAchievementUnlocked(const std::string& achievement_name) override;
//    virtual void onIncrementalAchievementStep(const std::string& achievement_name,
//                                              int step) override; // DEPRECATED
    
//    virtual void onIncrementalAchievementStepError(const std::string& name,
//                                                   int steps,
//                                                   int error_code,
//                                                   const std::string& error_description ) override; // DEPRECATED
    
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
    
//    virtual void onSetSteps(const std::string& name,
//                            int steps) override; // DEPRECATED
    
//    virtual void onSetStepsError(const std::string& name,
//                                 int steps,
//                                 int error_code,
//                                 const std::string& error_description) override; // DEPRECATED
    
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
                                        const std::string& json_with_score_entries ) override;
    
    virtual void onPlayerCenteredScoresError(const std::string& leaderboard_name,
                                             int time_span,
                                             int collection_type,
                                             int error_code,
                                             const std::string& error_description) override;
    
    virtual void onScoreSubmitted(const std::string& leaderboard_name,
                                  int score,
                                  bool maxScoreAllTime,
                                  bool maxScoreWeek,
                                  bool maxScoreToday);
    
    virtual void onGameData(const std::string& action,
                            const std::string& name,
                            const std::string& data,
                            const std::string& error) override;
    
    
    CPlayManager(){};
    virtual ~CPlayManager();
    
private:
    static CPlayManager* m_Instance;
};
