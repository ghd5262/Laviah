#pragma once
#include "../Common/HSHUtility.h"
#include "PluginFacebook/PluginFacebook.h"
#include "ui/UICheckBox.h"
//
//class CFacebookManager : public cocos2d::Node, sdkbox::FacebookListener
//{
//public:
//    static CFacebookManager* Instance();
//    static void CheckFacebookStatus();
//    
//    CC_SYNTHESIZE(std::function<void(const sdkbox::FBInvitableFriendsInfo&)>, m_InvitableFriendsListener, InvitableFriendsListener);
//    CC_SYNTHESIZE(std::function<void(bool, std::string)>, m_LoginListener, LoginListener);
//    
//private:
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
//    CFacebookManager()
//    : m_InvitableFriendsListener(nullptr)
//    , m_LoginListener(nullptr){};
//    virtual ~CFacebookManager();
//    
//private:
//    static CFacebookManager* m_Instance;
//};

class SpriteEx;
class CFacebookManager : public cocos2d::Node, sdkbox::FacebookListener
{
public:
    static CFacebookManager* Instance();
    
    void onLoginClick(cocos2d::Ref* sender);
    void onCheckStatus(cocos2d::Ref* sender);
    void onGetMyInfo(cocos2d::Ref* sender);
    void onGetMyFriends(cocos2d::Ref* sender);
    void onCaptureScreen(cocos2d::Ref* sender);
    void onShareLink(cocos2d::Ref* sender);
    void onSharePhoto(cocos2d::Ref* sender);
    void onDialogLink(cocos2d::Ref* sender);
    void onDialogPhoto(cocos2d::Ref* sender);
    void onRequestReadPermission(cocos2d::Ref* sender);
    void onRequestPublishPermission(cocos2d::Ref* sender);
    void onInviteFriends(cocos2d::Ref* sender);
    void onInviteFriendsCustomUI(cocos2d::Ref* sender);
    
private:
    void createTestMenu();
    
    void showInviteDialog();
    void createInviteDialog();
    void showInviteableFriends();
    void onCheckBoxState(Ref*, cocos2d::ui::CheckBox::EventType);
    void onButtonCancel(Ref*);
    void onButtonInvite(Ref*);
    
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
    
    CFacebookManager(){};
    virtual ~CFacebookManager();
    
private:
    static CFacebookManager* m_Instance;
    
    void afterCaptureScreen(bool, const std::string& outputFilename);
    
    std::string _captureFilename;
    cocos2d::MenuItemLabel* _loginItem;
    SpriteEx* _iconSprite;
    Node* _inviteDialog;
    std::string _inviteTitle;
    std::string _inviteText;
    std::vector<sdkbox::FBGraphUser> _inviteableUsers;
    std::vector<int> _inviteUserIdxs;
    cocos2d::EventListenerTouchOneByOne* _touchListener;
};