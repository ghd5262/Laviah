#include "FacebookManager.hpp"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "ui/UIScrollView.h"
#include "ui/UICheckBox.h"
#include "ui/UIButton.h"
#include "../json/json.h"

using namespace sdkbox;
using namespace cocos2d;
using namespace cocos2d::ui;

CFacebookManager* CFacebookManager::m_Instance = nullptr;

CFacebookManager::~CFacebookManager()
{
    m_Instance = nullptr;
}

CFacebookManager* CFacebookManager::Instance()
{
    if(m_Instance != nullptr) return m_Instance;
    
    m_Instance = new(std::nothrow) CFacebookManager();
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

void CFacebookManager::CheckFacebookStatus()
{
    CCLOG("##FB> permission list: ");
    for (auto& permission : PluginFacebook::getPermissionList())
    {
        CCLOG("##FB>> permission %s", permission.data());
    }
    CCLOG("##FB> access token: %s", PluginFacebook::getAccessToken().c_str());
    CCLOG("##FB> user id: %s", PluginFacebook::getUserID().c_str());
    CCLOG("##FB> FBSDK version: %s", PluginFacebook::getSDKVersion().c_str());
}

// on "init" you need to initialize your instance
bool CFacebookManager::init()
{
    if (!Node::init()) return false;
    
    PluginFacebook::setListener(this);
    sdkbox::PluginFacebook::init();
    
    return true;
}

/*********************
 * Facebook callbacks
 *********************/
void CFacebookManager::onLogin(bool isLogin, const std::string& error)
{
    if(m_LoginListener) {
        m_LoginListener(isLogin, error);
        m_LoginListener = nullptr;
    }
}

void CFacebookManager::onAPI(const std::string& tag, const std::string& jsonData)
{
    CCLOG("##FB onAPI: tag -> %s, json -> %s", tag.c_str(), jsonData.c_str());
    if (tag == "__fetch_picture_tag__") {
//        picojson::value v;
//        picojson::parse(v, jsonData);
//        std::string url = v.get("data").get("url").to_str();
        
//        Json::Value  root;
//        Json::Reader reader;
//        std::string  jsonDataStr = jsonData;
//        
//        jsonDataStr              = jsonDataStr.substr(0, jsonDataStr.rfind("}") + 1);
//        bool parsingSuccessful   = reader.parse(jsonDataStr, root);
//        if (!parsingSuccessful)
//        {
//            CCASSERT(false, MakeString("parser failed : \n %s", jsonDataStr.c_str()).c_str());
//            return;
//        }
//        std::string  url         = root
        
//        CCLOG("picture's url = %s", url.data());
//        
//        _iconSprite->updateWithUrl(url);
    }
}

void CFacebookManager::onSharedSuccess(const std::string& message)
{
    CCLOG("##FB onSharedSuccess:%s", message.c_str());
    MessageBox(message.c_str(), "share success");
}

void CFacebookManager::onSharedFailed(const std::string& message)
{
    CCLOG("##FB onSharedFailed:%s", message.c_str());
    MessageBox(message.c_str(), "share failed");
}

void CFacebookManager::onSharedCancel()
{
    CCLOG("##FB onSharedCancel");
    MessageBox("", "share cancel");
}

void CFacebookManager::onPermission(bool isLogin, const std::string& error)
{
    CCLOG("##FB onPermission: %d, error: %s", isLogin, error.c_str());
    
    std::string title = "permission ";
    title.append((isLogin ? "success" : "failed"));
    MessageBox(error.c_str(), title.c_str());
}

void CFacebookManager::onFetchFriends(bool ok, const std::string& msg)
{
    CCLOG("##FB %s: %d = %s", __FUNCTION__, ok, msg.data());
    
    MenuItemFont::setFontSize(20);
    static Menu *menu = Menu::create();
    menu->setPositionY(20);
    menu->cleanup();
    
    const std::vector<sdkbox::FBGraphUser>& friends = PluginFacebook::getFriends();
    for (int i = 0; i < friends.size(); i++)
    {
        const sdkbox::FBGraphUser& user = friends.at(i);
        CCLOG("##FB> -------------------------------");
        CCLOG("##FB>> %s", user.uid.data());
        CCLOG("##FB>> %s", user.firstName.data());
        CCLOG("##FB>> %s", user.lastName.data());
        CCLOG("##FB>> %s", user.name.data());
        CCLOG("##FB>> %s", user.isInstalled ? "app is installed" : "app is not installed");
        CCLOG("##FB");
        
        MenuItemFont *item = MenuItemFont::create(user.name, [=](Ref*) {
            sdkbox::FBAPIParam params;
            params["redirect"] = "false";
            params["type"] = "small";
            std::string url(user.uid + "/picture");
            PluginFacebook::api(url, "GET", params, "__fetch_picture_tag__");
        });
        menu->addChild(item);
    }
    if (!menu->getParent()) {
        menu->alignItemsHorizontally();
        addChild(menu);
    }
    
    MessageBox("", "fetch friends");
}

void CFacebookManager::onRequestInvitableFriends( const FBInvitableFriendsInfo& friends )
{
    if(m_InvitableFriendsListener) {
        m_InvitableFriendsListener(friends);
        m_InvitableFriendsListener = nullptr;
    }
}

void CFacebookManager::onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg )
{
    CCLOG("on invite friends with invite ids %s= '%s'", result ? "ok":"error", msg.c_str());
}

void CFacebookManager::onInviteFriendsResult( bool result, const std::string& msg )
{
    CCLOG("on invite friends %s = '%s'", result ? "ok":"error", msg.c_str());
}

void CFacebookManager::onGetUserInfo( const sdkbox::FBGraphUser& userInfo )
{
    CCLOG("facebook id : %s", userInfo.getUserId().c_str());
    CCLOG("name        : %s", userInfo.getName().c_str());
    CCLOG("last name   : %s", userInfo.getFirstName().c_str());
    CCLOG("first name  : %s", userInfo.getLastName().c_str());
    CCLOG("email       : %s", userInfo.getEmail().c_str());
    CCLOG("installed   : %s", userInfo.isInstalled ? "installed" : "non-installed");
}