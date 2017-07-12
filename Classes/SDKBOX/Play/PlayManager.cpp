#include "PlayManager.hpp"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "../../MyUI/MyButton.h"
#include "../../MyUI/UrlSprite.hpp"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/DataManagerUtils.h"
#include "../../Common/NoticeDefine.h"

using namespace cocos2d;
using namespace cocos2d::ui;

CPlayManager* CPlayManager::m_Instance = nullptr;

CPlayManager::~CPlayManager()
{
    this->ClearData();
    m_Instance = nullptr;
}

CPlayManager* CPlayManager::Instance()
{
    if(m_Instance != nullptr) return m_Instance;
    
    m_Instance = new(std::nothrow) CPlayManager();
    if (m_Instance && m_Instance->init())
    {
        m_Instance->autorelease();
        return m_Instance;
    }
    else
    {
        delete m_Instance;
        m_Instance = NULL;
        return NULL;
    }
}
void CPlayManager::Initialize()
{
    sdkbox::PluginSdkboxPlay::init();
}

void CPlayManager::Login()
{
    sdkbox::PluginSdkboxPlay::signin();
}

bool CPlayManager::IsLoggedIn()
{
    return sdkbox::PluginSdkboxPlay::isSignedIn();
}

void CPlayManager::DataLoad(std::string key)
{
    sdkbox::PluginSdkboxPlay::loadGameData(key);
}

void CPlayManager::DataSave(std::string key, std::string data)
{
    sdkbox::PluginSdkboxPlay::saveGameData(key, data);
}

void CPlayManager::onConnectionStatusChanged(int connection_status)
{
    Director::getInstance()->getScheduler()->schedule([=](float delta){
        if(sdkbox::PluginSdkboxPlay::isSignedIn())
            __NotificationCenter::getInstance()->postNotification(NOTICE::LOGIN_RESULT, NULL);
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

//    void CPlayManager::onIncrementalAchievementStep(const std::string& achievement_name,
//                                              int step) // DEPRECATED

//    void CPlayManager::onIncrementalAchievementStepError(const std::string& name,
//                                                   int steps,
//                                                   int error_code,
//                                                   const std::string& error_description ) // DEPRECATED

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

//    void CPlayManager::onSetSteps(const std::string& name,
//                            int steps) // DEPRECATED

//    void CPlayManager::onSetStepsError(const std::string& name,
//                                 int steps,
//                                 int error_code,
//                                 const std::string& error_description) // DEPRECATED

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
        CCLOG("%s failed : %s", action.c_str(), error.c_str());
        return;
    }
    
    CCLOG("%s succeed", action.c_str());
    if(action == "save"){
        
    }
    else if(action == "load"){
        CUserDataManager::Instance()->setUserData_CloudSaved(data);
    }
    else{
        
        return;
    }
}
