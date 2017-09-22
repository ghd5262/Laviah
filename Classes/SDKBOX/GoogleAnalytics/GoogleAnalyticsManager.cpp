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

void CGoogleAnalyticsManager::LogEventAction(std::string category, std::string action, int value/* = 0*/)
{
    if(!META_DATA("GA_EVENT_ENABLE").asBool()) return;
    
    sdkbox::PluginGoogleAnalytics::logEvent(category, action, "", value);
}

void CGoogleAnalyticsManager::LogEventValue(std::string category, std::string action, int value)
{
    if(!META_DATA("GA_EVENT_ENABLE").asBool()) return;

    auto valueString = StringUtils::format("%04d", value);
    sdkbox::PluginGoogleAnalytics::logEvent(category, action, valueString, 0);
}

void CGoogleAnalyticsManager::LogEventValue(std::string category, std::string action, std::string value)
{
    if(!META_DATA("GA_EVENT_ENABLE").asBool()) return;
    
    sdkbox::PluginGoogleAnalytics::logEvent(category, action, value, 0);
}

void CGoogleAnalyticsManager::LogEventCoin(std::string action, int value)
{
    if(!META_DATA("GA_EVENT_ENABLE").asBool()) return;

    sdkbox::PluginGoogleAnalytics::logEvent(GA_CATEGORY::COIN, action, "", value);
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

void CGoogleAnalyticsManager::LogScreen(std::string title, int index)
{
    if(!META_DATA("GA_EVENT_ENABLE").asBool()) return;
    
    title += StringUtils::format("/%04d", index);
    sdkbox::PluginGoogleAnalytics::logScreen(title);
}
