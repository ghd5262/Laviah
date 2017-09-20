#include "GoogleAnalyticsManager.hpp"
#include "../../DataManager/GameMetaDataManager.hpp"
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace std;
using namespace std::chrono;

CGoogleAnalyticsManager::CGoogleAnalyticsManager(){}

CGoogleAnalyticsManager::~CGoogleAnalyticsManager(){}

CGoogleAnalyticsManager* CGoogleAnalyticsManager::Instance()
{
    static CGoogleAnalyticsManager instance;
    return &instance;
}

void CGoogleAnalyticsManager::Initialize()
{
    sdkbox::PluginGoogleAnalytics::init();
}

void CGoogleAnalyticsManager::StartTimer(std::string timerKey)
{
    if(!META_DATA("GA_EVENT_ENABLE").asBool()) return;

    auto currentTime = system_clock::now();
    auto found = m_TimerList.find(timerKey);
    if(found != m_TimerList.end())
        m_TimerList.at(timerKey) = currentTime;
    else
        m_TimerList.emplace(std::pair<std::string, system_clock::time_point>(timerKey,
                                                                             currentTime));
}

void CGoogleAnalyticsManager::EndTimer(std::string timerKey)
{
    if(!META_DATA("GA_EVENT_ENABLE").asBool()) return;

    auto found = m_TimerList.find(timerKey);
    system_clock::time_point savedSec;
    if(found != m_TimerList.end())
        savedSec = found->second;
    else return;
    
    auto current  = system_clock::now();
    auto milliSec = duration_cast<milliseconds>(current - savedSec);
    
    sdkbox::PluginGoogleAnalytics::logTiming(GA_CATEGORY::TIMER,
                                             milliSec.count(),
                                             timerKey,
                                             timerKey);
}

void CGoogleAnalyticsManager::LogEvent(std::string category,
                                       std::string action,
                                       std::string label,
                                       int value)
{
    if(!META_DATA("GA_EVENT_ENABLE").asBool()) return;

    sdkbox::PluginGoogleAnalytics::logEvent(category, action, label, value);
}

void CGoogleAnalyticsManager::LogSocial(std::string category,
                                        std::string action,
                                        std::string target)
{
    if(!META_DATA("GA_EVENT_ENABLE").asBool()) return;

    sdkbox::PluginGoogleAnalytics::logSocial(category, action, target);
}

void CGoogleAnalyticsManager::LogScreen(std::string title)
{
    if(!META_DATA("GA_EVENT_ENABLE").asBool()) return;
    
    sdkbox::PluginGoogleAnalytics::logScreen(title);
}

void CGoogleAnalyticsManager::LogDimension(int key, int value)
{
    if(!META_DATA("GA_EVENT_ENABLE").asBool()) return;
    
    auto str = StringUtils::format("%d", value);
    sdkbox::PluginGoogleAnalytics::setDimension(key, str);
}

void CGoogleAnalyticsManager::LogMetric(int key, int value)
{
    if(!META_DATA("GA_EVENT_ENABLE").asBool()) return;

    auto str = StringUtils::format("%d", value);
    sdkbox::PluginGoogleAnalytics::setMetric(key, str);
}
