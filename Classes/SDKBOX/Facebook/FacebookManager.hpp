#pragma once
#include "cocos2d.h"
#include "PluginFacebook/PluginFacebook.h"
#include <vector>

namespace Json{
    class Value;
}

namespace FACEBOOK_DEFINE {
    static const std::string TAG_API_ME         = "me";
    static const std::string TAG_API_FRIENDS    = "friendList";
    static const std::string TAG_API_SAVE_SCORE = "saveScore";
}

struct FBUSER_PARAM{
    std::string _userId;
    std::string _name;
    std::string _firstName;
    std::string _lastName;
    std::string _url;
    bool _silhouette;
    bool _installed;
    int _score;
};

typedef std::map<std::string, const FBUSER_PARAM*> FBFRIEND_LIST;
typedef std::pair<std::string, const FBUSER_PARAM*> FB_PARAM_PAIR;
typedef std::vector<FB_PARAM_PAIR> FB_USER_LIST;
typedef std::function<void(void)> API_LISTENER;
class CFacebookManager : public cocos2d::Node, sdkbox::FacebookListener
{
public:
    static CFacebookManager* Instance();

    int getMyRank();
    int getRankByScore(int score);
    const FBUSER_PARAM* getFriendByRank(int rank);
    FB_USER_LIST getFBUserList();
    FBFRIEND_LIST getFBFriendList() const { return m_FBFriendList; }
    const FBUSER_PARAM* getMyFacebookData() const { return m_MyFacebookData; }
    void Login(API_LISTENER listener);
    void RequestPermission(API_LISTENER listener, std::string id);
    bool SaveNodeToFile(cocos2d::Node* node);
    
    void ClearData();
    
    CC_SYNTHESIZE(std::function<void(const sdkbox::FBInvitableFriendsInfo&)>, m_InvitableFriendsListener, InvitableFriendsListener);
    CC_SYNTHESIZE(API_LISTENER, m_LoginListener,        LoginListener);
    CC_SYNTHESIZE(API_LISTENER, m_SaveScoreListener,    SaveScoreListener);
    CC_SYNTHESIZE(API_LISTENER, m_MyInfoListener,       MyInfoListener);
    CC_SYNTHESIZE(API_LISTENER, m_FriendListListener,   FriendListListener);
    CC_SYNTHESIZE(API_LISTENER, m_PermissionListener,   PermissionListener);
    CC_SYNTHESIZE(std::string,  m_FacebookCapture,      FacebookCapture);

    static void CheckFacebookStatus();
    static void RequestMyInfo();
    static void RequestFriendList();
    static void SaveScore(int score);
    static bool IsScoresEnabled();
    static bool IsPermissionAllowed(std::string id);
    static void OpenPhotoShareDialog(std::string title);
    static void OpenLinkShareDialog(std::string title, std::string text);
    static void OpenInviteDialog();

private:
    //Facebook callback
    virtual void onLogin(bool isLogin, const std::string& msg) override;
    virtual void onSharedSuccess(const std::string& message) override;
    virtual void onSharedFailed(const std::string& message) override;
    virtual void onSharedCancel() override;
    virtual void onAPI(const std::string& key, const std::string& jsonData) override;
    virtual void onPermission(bool isLogin, const std::string& msg) override;
    virtual void onFetchFriends(bool ok, const std::string& msg) override;
    virtual void onRequestInvitableFriends( const sdkbox::FBInvitableFriendsInfo& friends ) override;
    virtual void onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg ) override;
    virtual void onInviteFriendsResult( bool result, const std::string& msg ) override;
    virtual void onGetUserInfo( const sdkbox::FBGraphUser& userInfo ) override;
    
protected:
    virtual bool init() override;
    
private:
    void initFacebookUserDataByJson(FBUSER_PARAM* param,
                                    const Json::Value& data);
    const FBUSER_PARAM* createNewFriendData(std::string id);
    void callAPIListener(API_LISTENER& listener);
    
    CFacebookManager()
    : m_InvitableFriendsListener(nullptr)
    , m_LoginListener(nullptr)
    , m_SaveScoreListener(nullptr)
    , m_MyInfoListener(nullptr)
    , m_FriendListListener(nullptr)
    , m_PermissionListener(nullptr)
    , m_FacebookCapture(""){};
    virtual ~CFacebookManager();
    
private:
    static CFacebookManager* m_Instance;
    FBFRIEND_LIST m_FBFriendList;
    FBUSER_PARAM* m_MyFacebookData;
};
