#pragma once
#include "../Common/HSHUtility.h"
#include "PluginFacebook/PluginFacebook.h"
#include "ui/UICheckBox.h"
#include "../json/json.h"
#include <vector>

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
typedef std::function<void(bool)> API_LISTENER;
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
    CC_SYNTHESIZE(std::function<void(const sdkbox::FBInvitableFriendsInfo&)>, m_InvitableFriendsListener, InvitableFriendsListener);
    CC_SYNTHESIZE(std::function<void(bool, std::string)>, m_LoginListener, LoginListener);
    CC_SYNTHESIZE(API_LISTENER, m_SaveScoreListener, SaveScoreListener);
    CC_SYNTHESIZE(API_LISTENER, m_MyInfoListener, MyInfoListener);
    CC_SYNTHESIZE(API_LISTENER, m_FriendListListener, FriendListListener);
    
    static void CheckFacebookStatus();
    static void RequestMyInfo();
    static void RequestFriendList();
    static void SaveScore(int score);
    
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
    void callAPIListener(bool succeed, API_LISTENER& listener);
    
    CFacebookManager()
    : m_InvitableFriendsListener(nullptr)
    , m_LoginListener(nullptr)
    , m_SaveScoreListener(nullptr)
    , m_MyInfoListener(nullptr)
    , m_FriendListListener(nullptr){};
    virtual ~CFacebookManager();
    
private:
    static CFacebookManager* m_Instance;
    FBFRIEND_LIST m_FBFriendList;
    FBUSER_PARAM* m_MyFacebookData;
};

//class SpriteEx;
//class CFacebookManager : public cocos2d::Node, sdkbox::FacebookListener
//{
//public:
//    static CFacebookManager* Instance();
//    
//    void onLoginClick(cocos2d::Ref* sender);
//    void onCheckStatus(cocos2d::Ref* sender);
//    void onGetMyInfo(cocos2d::Ref* sender);
//    void onGetMyFriends(cocos2d::Ref* sender);
//    void onCaptureScreen(cocos2d::Ref* sender);
//    void onShareLink(cocos2d::Ref* sender);
//    void onSharePhoto(cocos2d::Ref* sender);
//    void onDialogLink(cocos2d::Ref* sender);
//    void onDialogPhoto(cocos2d::Ref* sender);
//    void onRequestReadPermission(cocos2d::Ref* sender);
//    void onRequestPublishPermission(cocos2d::Ref* sender);
//    void onInviteFriends(cocos2d::Ref* sender);
//    void onInviteFriendsCustomUI(cocos2d::Ref* sender);
//    
//private:
//    void createTestMenu();
//    
//    void showInviteDialog();
//    void createInviteDialog();
//    void showInviteableFriends();
//    void onCheckBoxState(Ref*, cocos2d::ui::CheckBox::EventType);
//    void onButtonCancel(Ref*);
//    void onButtonInvite(Ref*);
//    
//    //Facebook callback
//    virtual void onLogin(bool isLogin, const std::string& msg) override;
//    virtual void onSharedSuccess(const std::string& message) override;
//    virtual void onSharedFailed(const std::string& message) override;
//    virtual void onSharedCancel() override;
//    virtual void onAPI(const std::string& key, const std::string& jsonData) override;
//    virtual void onPermission(bool isLogin, const std::string& msg) override;
//    virtual void onFetchFriends(bool ok, const std::string& msg) override;
//    virtual void onRequestInvitableFriends( const sdkbox::FBInvitableFriendsInfo& friends ) override;
//    virtual void onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg ) override;
//    virtual void onInviteFriendsResult( bool result, const std::string& msg ) override;
//    virtual void onGetUserInfo( const sdkbox::FBGraphUser& userInfo ) override;
//    
//protected:
//    virtual bool init() override;
//    
//private:
//    
//    CFacebookManager(){};
//    virtual ~CFacebookManager();
//    
//private:
//    static CFacebookManager* m_Instance;
//    
//    void afterCaptureScreen(bool, const std::string& outputFilename);
//    
//    std::string _captureFilename;
//    cocos2d::MenuItemLabel* _loginItem;
//    SpriteEx* _iconSprite;
//    Node* _inviteDialog;
//    std::string _inviteTitle;
//    std::string _inviteText;
//    std::vector<sdkbox::FBGraphUser> _inviteableUsers;
//    std::vector<int> _inviteUserIdxs;
//    cocos2d::EventListenerTouchOneByOne* _touchListener;
//};