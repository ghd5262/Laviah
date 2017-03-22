#pragma once
#include "../Popup.h"
#include "PluginFacebook/PluginFacebook.h"
#include "ui/UICheckBox.h"
#include <vector>

class CFacebookAPITestPopup : public CPopup
{
public:
    static CFacebookAPITestPopup* create();
    
protected:
    virtual bool init() override;
    
private:
    void End(cocos2d::Node* sender);
    void Login(cocos2d::Node* sender);
    void CheckStatus(cocos2d::Node* sender);
    void GetMyInfo(cocos2d::Node* sender);
    void GetMyFriends(cocos2d::Node* sender);
    void CaptureScreen(cocos2d::Node* sender);
    void ShareLink(cocos2d::Node* sender);
    void SharePhoto(cocos2d::Node* sender);
    void DialogLink(cocos2d::Node* sender);
    void DialogPhoto(cocos2d::Node* sender);
    void RequestReadPermission(cocos2d::Node* sender);
    void RequestPublishPermission(cocos2d::Node* sender);
    void InviteFriends(cocos2d::Node* sender);
    void InviteFriendsCustomUI(cocos2d::Node* sender);
    void GetMyScore(cocos2d::Node* sender);
    void GetFriendsScore(cocos2d::Node* sender);
    void SetMyScore(cocos2d::Node* sender);
    
    void showInviteDialog();
    void createInviteDialog();
    void onCheckBoxState(Ref* sender, cocos2d::ui::CheckBox::EventType event);
    void showInviteableFriends();
    
    CFacebookAPITestPopup()
    : m_FacebookCapture("")
    , m_InviteTitle("")
    , m_InviteContent(""){};
    virtual ~CFacebookAPITestPopup(){};
    
private:
    std::string m_FacebookCapture;
    std::string m_InviteTitle;
    std::string m_InviteContent;
    std::vector<int> m_InviteUserList;
    std::vector<sdkbox::FBGraphUser> m_InviteableUsers;
};