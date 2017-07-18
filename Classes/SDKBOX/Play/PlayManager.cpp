#include "PlayManager.hpp"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/DataManagerUtils.h"

using namespace cocos2d;
using namespace cocos2d::ui;

CPlayManager::CPlayManager()
: m_LoginListener(nullptr)
, m_LogoutListener(nullptr)
, m_DataLoadListener(nullptr)
, m_DataSaveListener(nullptr){}

CPlayManager::~CPlayManager(){}

CPlayManager* CPlayManager::Instance()
{
    static CPlayManager instance;
    return &instance;
}

void CPlayManager::Initialize()
{
    sdkbox::PluginSdkboxPlay::setListener(this);
    sdkbox::PluginSdkboxPlay::init();
}

void CPlayManager::Login(VOID_LISTENER listener)
{
    this->setLoginListener(listener);
    sdkbox::PluginSdkboxPlay::signin();
}

void CPlayManager::Logout(VOID_LISTENER listener)
{
    if(!this->IsLoggedIn()) return;
    
    this->setLogoutListener(listener);
    sdkbox::PluginSdkboxPlay::signout();
}

bool CPlayManager::IsLoggedIn()
{
    return sdkbox::PluginSdkboxPlay::isSignedIn();
}

void CPlayManager::DataLoad(DATA_LISTENER listener, std::string key)
{
    if(!this->IsLoggedIn()) return;
    
    this->setDataLoadListener(listener);
    sdkbox::PluginSdkboxPlay::loadGameData(key);
}

void CPlayManager::DataSave(DATA_LISTENER listener, std::string key, std::string data)
{
    if(!this->IsLoggedIn()) return;
    
    CCLOG("Data Save %s", data.c_str());
    this->setDataSaveListener(listener);
    sdkbox::PluginSdkboxPlay::saveGameData(key, data);
}

void CPlayManager::OpenLeaderboard()
{
    if(!this->IsLoggedIn()) return;
    
    sdkbox::PluginSdkboxPlay::showAllLeaderboards();
}

void CPlayManager::OpenAchievement()
{
    if(!this->IsLoggedIn()) return;
    
    sdkbox::PluginSdkboxPlay::showAchievements();
}

void CPlayManager::ScoreSave(std::string key, int score)
{
    if(!this->IsLoggedIn()) return;
    
    sdkbox::PluginSdkboxPlay::submitScore(key, score);
}

void CPlayManager::callVoidListener(VOID_LISTENER& listener)
{
    if(listener)
    {
        listener();
        listener = nullptr;
    }
}

void CPlayManager::callDataListener(DATA_LISTENER& listener, std::string data)
{
    if(listener)
    {
        listener(data);
        listener = nullptr;
    }
}

void CPlayManager::onConnectionStatusChanged(int connection_status)
{
    CCLOG("connection status change: %d", connection_status);
    Director::getInstance()->getScheduler()->schedule([=](float delta){
        switch (connection_status) {
            case sdkbox::GPS_CONNECTED:     this->callVoidListener(m_LoginListener);  break;
            case sdkbox::GPS_DISCONNECTED:  this->callVoidListener(m_LogoutListener); break;
            default:{
                MessageBox("Login canceled.", "Notice");
                m_LoginListener  = nullptr;
                m_LogoutListener = nullptr;
            } break;
        }
    }, Director::getInstance(), 0.f, 0, 0.f, false, "LoginSucceed");
}

void CPlayManager::onScoreSubmitted(const std::string& leaderboard_name,
                                    long score,
                                    bool maxScoreAllTime,
                                    bool maxScoreWeek,
                                    bool maxScoreToday)
{
    
}

void CPlayManager::onIncrementalAchievementUnlocked(const std::string& achievement_name)
{
    
}

void CPlayManager::onIncrementalAchievementStep(const std::string& achievement_name,
                                                double step )
{
    
}

void CPlayManager::onIncrementalAchievementStepError(const std::string& name,
                                                     double steps,
                                                     int error_code,
                                                     const std::string& error_description )
{
    
}

void CPlayManager::onAchievementUnlocked(const std::string& achievement_name,
                                         bool newlyUnlocked )
{
    
}

void CPlayManager::onAchievementUnlockError(const std::string& achievement_name,
                                            int error_code,
                                            const std::string& error_description )
{
    
}

void CPlayManager::onAchievementsLoaded(bool reload_forced,
                                        const std::string& json_achievements_info )
{
    
}

void CPlayManager::onSetSteps(const std::string& name,
                              double steps)
{
    
}

void CPlayManager::onSetStepsError(const std::string& name,
                                   double steps,
                                   int error_code,
                                   const std::string& error_description)
{
    
}

void CPlayManager::onReveal(const std::string& name)
{
    
}

void CPlayManager::onRevealError(const std::string& name,
                                 int error_code,
                                 const std::string& error_description)
{
    
}

void CPlayManager::onMyScore(const std::string& leaderboard_name,
                             int time_span,
                             int collection_type,
                             long score)
{
    
}

void CPlayManager::onMyScoreError(const std::string& leaderboard_name,
                                  int time_span,
                                  int collection_type,
                                  int error_code,
                                  const std::string& error_description)
{
    
}

void CPlayManager::onPlayerCenteredScores(const std::string& leaderboard_name,
                                          int time_span,
                                          int collection_type,
                                          const std::string& json_with_score_entries )
{
    
}

void CPlayManager::onPlayerCenteredScoresError(const std::string& leaderboard_name,
                                               int time_span,
                                               int collection_type,
                                               int error_code,
                                               const std::string& error_description)
{
    
}

void CPlayManager::onScoreSubmitted(const std::string& leaderboard_name,
                                    int score,
                                    bool maxScoreAllTime,
                                    bool maxScoreWeek,
                                    bool maxScoreToday)
{
    
}

void CPlayManager::onGameData(const std::string& action,
                              const std::string& name,
                              const std::string& data,
                              const std::string& error)
{
    if(error.length())
    {
        MessageBox(error.c_str(), "Notice");
        CCLOG("%s failed : %s", action.c_str(), error.c_str());
        return;
    }
    
    CCLOG("%s succeed", action.c_str());
    
    if(action == "save")        this->callDataListener(m_DataSaveListener, data);
    else if(action == "load")   this->callDataListener(m_DataLoadListener, data);
    else                        {}
}
