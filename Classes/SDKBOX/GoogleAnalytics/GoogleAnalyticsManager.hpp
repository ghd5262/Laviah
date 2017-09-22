#pragma once
#include <iostream>
#include "cocos2d.h"
#include "PluginGoogleAnalytics/PluginGoogleAnalytics.h"
#include <map>
#include <chrono>

namespace GA_CATEGORY {
    const std::string GAME_PLAY = "GamePlay";
    const std::string RESULT    = "Result";
    const std::string COIN      = "Coin";
    const std::string WATCH_ADS = "WatchAds";
    const std::string SHARE     = "Share";
    const std::string REVIEW    = "Review";
    const std::string TIMER     = "Timer";
    const std::string OPTION    = "Option";
};

namespace GA_ACTION {
    
    const std::string COIN                  = "coin";
    const std::string COIN_USE_COSTUME      = "coin_use_costume";
    const std::string COIN_USE_REVIVE       = "coin_use_revive";
    const std::string COIN_USE_UPGRADE      = "coin_use_upgrade";
    const std::string COIN_GET_ACHIEVEMENT  = "coin_get_achievement";
    const std::string COIN_GET_INGAME       = "coin_get_ingame";
    const std::string COIN_GET_GOAL         = "coin_get_goal";
    const std::string COIN_GET_BONUS        = "coin_get_bonus";
    const std::string COIN_GET_FB_REWARD    = "coin_get_fb_reward";
    
    const std::string ADS_REVIVE            = "ads_revive";
    const std::string ADS_SKIP              = "ads_skip";
    const std::string ADS_BONUS             = "ads_bonus";
    
    const std::string SHARE_SCORE           = "share_score";
    const std::string SHARE_COSTUME         = "share_costume";
    
    const std::string REVIEW_LOVE_YES       = "review_love_yes";
    const std::string REVIEW_LOVE_NO        = "review_love_no";
    const std::string REVIEW_LOVE_NEVER     = "review_love_never";
    const std::string REVIEW_HATE_YES       = "review_hate_yes";
    const std::string REVIEW_HATE_NO        = "review_hate_no";
    const std::string REVIEW_HATE_NEVER     = "review_hate_never";
    
    const std::string TIMER_FIRST_DOWNLOAD  = "timer_first_download";
    const std::string TIMER_DOWNLOAD        = "timer_download";
    const std::string TIMER_RANKING_OPEN    = "timer_ranking_open";
    const std::string TIMER_WORLD_RANK_GET  = "timer_world_rank_get";
    
    const std::string OPTION_LOGIN          = "option_login";
    const std::string OPTION_AUTO_SAVE      = "option_auto_save";
    const std::string OPTION_DATA_SAVE      = "option_data_save";
    const std::string OPTION_DATA_LOAD      = "option_data_load";
    const std::string OPTION_LANGUAGE       = "option_language";
    const std::string OPTION_BGM            = "option_bgm";
    const std::string OPTION_EFFECT_SOUND   = "option_effect_sound";
    
    const std::string TOTAL_SCORE           = "result_total_score";
    const std::string STAR_SCORE            = "result_star_score";
    const std::string COIN_SCORE            = "result_coin_score";
    const std::string COMBO_SCORE           = "result_combo_score";
    
    const std::string LEVEL                 = "level_up";
    const std::string ITEM_UPGRADE          = "item_upgrade";
    const std::string END_PATTERN           = "end_pattern";
    const std::string END_BULLET            = "end_bullet";
    const std::string END_LEVEL             = "end_level";
    const std::string PLAY_CHARACTER        = "play_character";
    const std::string PLAY_COSTUME          = "play_costume";
    
    const std::string GOAL_SKIP             = "goal_skip";
    const std::string GOAL_COMPLETE         = "goal_complete";
    const std::string ACHIEVEMENT_COMPLETE  = "achievement_complete";
    const std::string SCORE_BY_LEVEL        = "score_by_level";
    const std::string STAR_COUNT_BY_LEVEL   = "star_count_by_level";
    const std::string STAR_ITEM_BY_LEVEL    = "star_item_by_level";
    const std::string COIN_ITEM_BY_LEVEL    = "coin_item_by_level";
    const std::string BARRIER_ITEM_BY_LEVEL = "barrier_item_by_level";
    const std::string MAGNET_ITEM_BY_LEVEL  = "magnet_item_by_level";
    const std::string GIANT_ITEM_BY_LEVEL   = "giant_item_by_level";
};

namespace GA_SCREEN {
    const std::string LOAD                  = "/load";
    const std::string TITLE                 = "/title";
    const std::string MENU                  = "/menu";
    
    const std::string OPTION                = "/menu/option";
    
    const std::string FACEBOOK_RANK         = "/menu/facebook_rank";
    const std::string FACEBOOK_LOGIN        = "/menu/facebook_login";
    const std::string FACEBOOK_REWARD       = "/menu/facebook_rank/reward";

    const std::string ACHIEVEMENT           = "/menu/achievement";
    const std::string SDKPLAY_RANK          = "/menu/achievement/sdkplay_rank";
    const std::string SDKPLAY_ACHIEVEMENT   = "/menu/achievement/sdkplay_achievement";
    
    const std::string CHARACTER             = "/menu/character";
    const std::string COSTUME               = "/menu/character/costume";
    const std::string SHARE_COSTUME         = "/menu/character/costume/share_costume";
    
    const std::string WORKSHOP              = "/menu/workshop";
    const std::string WORKSHOP_BUY          = "/menu/workshop/buy";
    
    const std::string INGAME                = "/ingame";
    
    const std::string PAUSE                 = "/ingame/pause";
    const std::string PAUSE_SKIP            = "/ingame/pause/skip";
    
    const std::string END                   = "/ingame/end";
    
    const std::string VIDEO                 = "/ingame/video";
    
    const std::string RESULT                = "/ingame/result";
    const std::string REWARD_BONUS          = "/ingame/result/reward_bonus";
    const std::string REWARD_FREE           = "/ingame/result/reward_free";
    const std::string REWARD_COSTUME        = "/ingame/result/reward_costume";
    const std::string REWARD_ACHIEVEMENT    = "/ingame/result/reward_achievement";
    const std::string SHARE_SCORE           = "/ingame/result/share_score";

    const std::string GOAL                  = "/ingame/goal";
    const std::string GOAL_SKIP             = "/ingame/goal/skip";
    const std::string REWARD_GOAL           = "/ingame/goal/reward";

    const std::string TUTORIAL              = "/ingame/tutorial";
    
    const std::string STAGE                 = "/ingame/stage";
    
    const std::string SCORE                 = "/ingame/score";
};

typedef std::map<std::string, std::chrono::system_clock::time_point> TIMER_LIST;

class CGoogleAnalyticsManager
{
public:
    static CGoogleAnalyticsManager* Instance();
    void Initialize();
    void StartTimer(std::string timerKey);
    void EndTimer(std::string timerKey);
    
    static void LogEventAction(std::string category, std::string action, int value = 0);
    static void LogEventValue(std::string category, std::string action, int value);
    static void LogEventValue(std::string category, std::string action, std::string value);
    static void LogEventCoin(std::string action, int value);

    static void LogSocial(std::string category,
                          std::string action,
                          std::string target);

    static void LogScreen(std::string title);
    static void LogScreen(std::string title, int index);

private:
    
    CGoogleAnalyticsManager();
    virtual ~CGoogleAnalyticsManager();
    
private:
    TIMER_LIST m_TimerList;
};
