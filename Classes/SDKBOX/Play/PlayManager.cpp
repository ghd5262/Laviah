#include "PlayManager.hpp"
#include "../SDKBoxHeaders.h"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/DataManagerUtils.h"
#include "../../json/json.h"
#include "../../Download/DownloadManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;

CPlayManager::CPlayManager()
: m_LoginListener(nullptr)
, m_LogoutListener(nullptr)
, m_DataLoadListener(nullptr)
, m_DataSaveListener(nullptr){}

CPlayManager::~CPlayManager(){
    DATA_MANAGER_UTILS::mapDeleteAndClean(m_Leaderboards);
}

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
    CGoogleAnalyticsManager::LogScreen(GA_SCREEN::SDKPLAY_RANK);
}

void CPlayManager::OpenAchievement()
{
    if(!this->IsLoggedIn()) return;
    
    sdkbox::PluginSdkboxPlay::showAchievements();
    CGoogleAnalyticsManager::LogScreen(GA_SCREEN::SDKPLAY_ACHIEVEMENT);
}

void CPlayManager::ScoreSave(VOID_LISTENER listener, std::string key, int score)
{
#if(DEBUGING)
    return;
#endif
    if(!this->IsLoggedIn()) return;
 
//    this->setSaveScoreListener(listener);
//    if(!this->IsNewHighScore(key, score, sdkbox::TIME_SCOPE::DAY)) {
//        this->callVoidListener(m_SaveScoreListener);
//        return;
//    }
//    
    if(this->IsNewHighScore(key, score, sdkbox::TIME_SCOPE::DAY))
        sdkbox::PluginSdkboxPlay::submitScore(key, score);
}

void CPlayManager::ScoreLoad(VOID_LISTENER listener, std::string key, int score, int time_span)
{
    if(!this->IsLoggedIn()) return;

    this->setLoadScoreListener(listener);
//    if(!this->IsNewHighScore(key, score, time_span)) {
//        this->callVoidListener(m_LoadScoreListener);
//        return;
//    }
    sdkbox::PluginSdkboxPlay::getPlayerCenteredScores(key, time_span, sdkbox::PLAYER_SCOPE::GLOBAL, 1);
}

bool CPlayManager::IsNewHighScore(std::string key, int score, int time_span)
{
    auto saved = 0;
    auto iter  = m_Leaderboards.find(key);
    if(iter == m_Leaderboards.end())      return true;
    switch (time_span) {
        case sdkbox::TIME_SCOPE::DAY:       saved = iter->second->_dailyScore;   break;
        case sdkbox::TIME_SCOPE::ALL_TIME:  saved = iter->second->_allTimeScore; break;
    }
    
    return (saved < score);
}

void CPlayManager::AchievementComplete(std::string key)
{
    sdkbox::PluginSdkboxPlay::incrementAchievement(key, 100);
}

LEADERBOARD* CPlayManager::getLeaderboardData(std::string key)
{
    auto iter = m_Leaderboards.find(key);
    if(iter != m_Leaderboards.end())
        return iter->second;
    return nullptr;
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

void CPlayManager::saveScoreToMap(std::string key, int score, int time_span, int rank/* = 0*/)
{
    LEADERBOARD* leaderboard = nullptr;
    auto iter = m_Leaderboards.find(key);
    if(iter == m_Leaderboards.end()){
        leaderboard = new LEADERBOARD();
        m_Leaderboards.emplace(std::pair<std::string, LEADERBOARD*>(key, leaderboard));
    }
    else leaderboard = iter->second;
    
    switch (time_span) {
        case sdkbox::TIME_SCOPE::DAY:
            leaderboard->_dailyScore = score; break;
        case sdkbox::TIME_SCOPE::ALL_TIME:
            leaderboard->_allTimeScore = score; break;
    }
    if(rank) leaderboard->_rank = rank;
}

void CPlayManager::onConnectionStatusChanged(int connection_status)
{
    CCLOG("connection status change: %d", connection_status);
    Director::getInstance()->getScheduler()->schedule([=](float delta){
        switch (connection_status) {
            case sdkbox::GPS_CONNECTED:     this->callVoidListener(m_LoginListener);  break;
            case sdkbox::GPS_DISCONNECTED:  this->callVoidListener(m_LogoutListener); break;
            default:{
//                MessageBox("Login canceled.", "Notice");
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
    CCLOG("score save successfully name %s, score %ld", leaderboard_name.c_str(), score);
    this->saveScoreToMap(leaderboard_name, (int)score, sdkbox::TIME_SCOPE::DAY);
//    this->callVoidListener(m_SaveScoreListener);
}

void CPlayManager::onIncrementalAchievementUnlocked(const std::string& achievement_name){
    CCLOG("sdkbox play %s", __FUNCTION__);
}

void CPlayManager::onIncrementalAchievementStep(const std::string& achievement_name,
                                                double step ){
    CCLOG("sdkbox play %s", __FUNCTION__);
}

void CPlayManager::onIncrementalAchievementStepError(const std::string& name,
                                                     double steps,
                                                     int error_code,
                                                     const std::string& error_description ){
    CCLOG("sdkbox play %s", __FUNCTION__);
    CCLOG("erro description : %s", error_description.c_str());
}

void CPlayManager::onAchievementUnlocked(const std::string& achievement_name,
                                         bool newlyUnlocked ){
    CCLOG("sdkbox play %s", __FUNCTION__);
}

void CPlayManager::onAchievementUnlockError(const std::string& achievement_name,
                                            int error_code,
                                            const std::string& error_description ){}

void CPlayManager::onAchievementsLoaded(bool reload_forced,
                                        const std::string& json_achievements_info ){}

void CPlayManager::onSetSteps(const std::string& name,
                              double steps){}

void CPlayManager::onSetStepsError(const std::string& name,
                                   double steps,
                                   int error_code,
                                   const std::string& error_description){}

void CPlayManager::onReveal(const std::string& name){}

void CPlayManager::onRevealError(const std::string& name,
                                 int error_code,
                                 const std::string& error_description){}

void CPlayManager::onMyScore(const std::string& leaderboard_name,
                             int time_span,
                             int collection_type,
                             long score){}

void CPlayManager::onMyScoreError(const std::string& leaderboard_name,
                                  int time_span,
                                  int collection_type,
                                  int error_code,
                                  const std::string& error_description){}
/*{
 *      "display_rank"          : string,
 *      "display_score"         : string,
 *      "rank"                  : number,   // long
 *      "score"                 : number,   // long,
 *      "holder_display_name"   : string,
 *      "hires_imageuri"        : string,    // content:// protocol
 *      "lowres_imageuri"       : string,
 *      "tag"                   : string,
 *      "timestamp_millis"      : long
 *    }[{"display_rank":"1","display_score":"9,720","hires_imageuri":"","holder_display_name":"Me","lowres_imageuri":"","rank":1.000000,"score":9720.000000,"tag":"","timestamp_millis":523568545792.000000}]
 */
void CPlayManager::onPlayerCenteredScores(const std::string& leaderboard_name,
                                          int time_span,
                                          int collection_type,
                                          const std::string& jsonData)
{
    Json::Value root;
    Json::Reader reader;
    
    std::string fileData = jsonData;
    size_t pos  = fileData.rfind("]");
    fileData    = fileData.substr(0, pos + 1);
    
    bool parsingSuccessful = reader.parse(fileData, root);
    if (!parsingSuccessful)
    {
        CCASSERT(false, MakeString("parser failed : \n %s", fileData.c_str()).c_str());
        return;
    }
    CCLOG("Leaderboard JSON : \n %s\n", fileData.c_str());
    
    const Json::Value array = root;
    if(array.size()){
        auto data  = array[unsigned(0)];
        auto score = data["score"].asDouble();
        auto rank  = data["rank"].asDouble();
        
        this->saveScoreToMap(leaderboard_name, score, time_span, rank);
    }
    this->callVoidListener(m_LoadScoreListener);
}

void CPlayManager::onPlayerCenteredScoresError(const std::string& leaderboard_name,
                                               int time_span,
                                               int collection_type,
                                               int error_code,
                                               const std::string& error_description)
{
    CCLOG("Player center scores error code : %d - %s", error_code, error_description.c_str());
}

void CPlayManager::onGameData(const std::string& action,
                              const std::string& name,
                              const std::string& data,
                              const std::string& error)
{
    if(error.length())
    {
        std::string errorMsg = TRANSLATE("DATA_SAVE_FAILED");
        MessageBox(errorMsg.c_str(), "Notice");
        CCLOG("%s failed : %s", action.c_str(), error.c_str());
        return;
    }
    
    CCLOG("%s succeed", action.c_str());
    
    if(action == "save")        this->callDataListener(m_DataSaveListener, data);
    else if(action == "load")   this->callDataListener(m_DataLoadListener, data);
    else                        {}
}
