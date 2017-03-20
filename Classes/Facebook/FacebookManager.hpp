#pragma once
#include "../Common/HSHUtility.h"
#include "PluginFacebook/PluginFacebook.h"
#include "ui/UICheckBox.h"

class CFacebookManager : public cocos2d::Node, sdkbox::FacebookListener
{
public:
    static CFacebookManager* Instance();
    static void CheckFacebookStatus();
    
    CC_SYNTHESIZE(std::function<void(const sdkbox::FBInvitableFriendsInfo&)>, m_InvitableFriendsListener, InvitableFriendsListener);
    CC_SYNTHESIZE(std::function<void(bool, std::string)>, m_LoginListener, LoginListener);
    
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
    CFacebookManager()
    : m_InvitableFriendsListener(nullptr)
    , m_LoginListener(nullptr){};
    virtual ~CFacebookManager();
    
private:
    static CFacebookManager* m_Instance;
};