#include "FacebookManager.hpp"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "../../MyUI/MyButton.h"
#include "../../MyUI/UrlSprite.hpp"
#include "../../DataManager/DataManagerUtils.h"
#include "../../json/json.h"
#include "../../Download/DownloadManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;

CFacebookManager* CFacebookManager::m_Instance = nullptr;

CFacebookManager::~CFacebookManager()
{
    CCLOG("Facebook destroied.");
    this->ClearData();
    CC_SAFE_DELETE(m_MyFacebookData);
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

int CFacebookManager::getMyRank()
{
    auto userList = getFBUserList();
    
    // get my ranking.
    auto iter     = std::find_if(userList.begin(), userList.end(), [=](FB_PARAM_PAIR data){
        return data.second->_userId == m_MyFacebookData->_userId;
    });
    return (int)std::distance(userList.begin(), iter);
}

int CFacebookManager::getRankByScore(int score)
{
    auto userList = getFBUserList();

    // get sequence of score.
    auto iter     = std::find_if(userList.begin(), userList.end(), [=](FB_PARAM_PAIR data){
        return data.second->_score < score;
    });
    return (int)std::distance(userList.begin(), iter);
}

const FBUSER_PARAM* CFacebookManager::getFriendByRank(int rank)
{
    auto userList = getFBUserList();
    
    if(userList.size() > rank)  return userList.at(rank).second;
    else                        return m_MyFacebookData;
}

FB_USER_LIST CFacebookManager::getFBUserList() 
{
    auto userList = FB_USER_LIST(m_FBFriendList.begin(), m_FBFriendList.end());
    
    // sort friend list included me.
    userList.emplace_back(FB_PARAM_PAIR(m_MyFacebookData->_userId, m_MyFacebookData));
    std::sort(userList.begin(), userList.end(), [=](FB_PARAM_PAIR dataA, FB_PARAM_PAIR dataB ){
        return (dataA.second->_score > dataB.second->_score);
    });
    
    return userList;
}

void CFacebookManager::Login(API_LISTENER listener)
{
    this->setLoginListener(listener);
    if(sdkbox::PluginFacebook::isLoggedIn()){
        this->onLogin(true, "");
        return;
    }
    
    std::vector<std::string> permissions;
    //    permissions.push_back(sdkbox::FB_PERM_READ_EMAIL);
    permissions.push_back(sdkbox::FB_PERM_READ_PUBLIC_PROFILE);
    permissions.push_back(sdkbox::FB_PERM_READ_USER_FRIENDS);
    sdkbox::PluginFacebook::login(permissions);
}

void CFacebookManager::RequestPermission(API_LISTENER listener, std::string id)
{
    this->setPermissionListener(listener);
    if(CFacebookManager::IsPermissionAllowed(id)){
        this->onPermission(true, "");
        return;
    }
    
    if(id == sdkbox::FB_PERM_READ_USER_FRIENDS)
        sdkbox::PluginFacebook::requestReadPermissions({sdkbox::FB_PERM_READ_USER_FRIENDS});
    else if(id == sdkbox::FB_PERM_PUBLISH_POST)
        sdkbox::PluginFacebook::requestPublishPermissions({sdkbox::FB_PERM_PUBLISH_POST});
}

bool CFacebookManager::SaveNodeToFile(cocos2d::Node* node)
{
    auto image   = utils::captureNode(node);
    auto path    = FileUtils::getInstance()->getWritablePath() + "remoteImage/" + "FBCapture.png";
    auto succeed = image->saveToFile(path);
    if(!succeed){
        CCLOG("Capture failed. Can not save to file.");
        return false;
    }
 
    m_FacebookCapture = path;
    return true;
}

void CFacebookManager::ClearData()
{
    DATA_MANAGER_UTILS::mapDeleteAndClean(m_FBFriendList);
}

void CFacebookManager::CheckFacebookStatus()
{
    CCLOG("##FB> permission list: ");
    for (auto& permission : sdkbox::PluginFacebook::getPermissionList())
    {
        CCLOG("##FB>> permission %s", permission.data());
    }
    CCLOG("##FB> access token: %s", sdkbox::PluginFacebook::getAccessToken().c_str());
    CCLOG("##FB> user id: %s", sdkbox::PluginFacebook::getUserID().c_str());
    CCLOG("##FB> FBSDK version: %s", sdkbox::PluginFacebook::getSDKVersion().c_str());
}

void CFacebookManager::RequestMyInfo()
{
    sdkbox::FBAPIParam params;
    params["fields"] = "id,name,first_name,last_name,picture.type(large){is_silhouette,url},installed,scores{score}";
    sdkbox::PluginFacebook::api("/me", "GET", params, FACEBOOK_DEFINE::TAG_API_ME);
}

void CFacebookManager::RequestFriendList()
{
    sdkbox::FBAPIParam params;
    params["fields"] = "id,name,first_name,last_name,picture.type(large){is_silhouette,url},installed,scores{score}";
    params["limit"] = "30";
    sdkbox::PluginFacebook::api("/me/friends", "GET", params, FACEBOOK_DEFINE::TAG_API_FRIENDS);
}

void CFacebookManager::SaveScore(int score)
{
#if(DEBUGING || TEST_BUILD)
    return;
#endif
    sdkbox::FBAPIParam params;
    params["score"] = StringUtils::format("%d", score);
    sdkbox::PluginFacebook::api("/me/scores", "POST", params, FACEBOOK_DEFINE::TAG_API_SAVE_SCORE);
}

bool CFacebookManager::IsScoresEnabled()
{
    if(!CFacebookManager::Instance()->getMyDataInitialized())   return false;
    if(!sdkbox::PluginFacebook::isLoggedIn())                   return false;
    if(!CFacebookManager::IsPermissionAllowed(sdkbox::FB_PERM_READ_USER_FRIENDS))   return false;
    if(!CFacebookManager::IsPermissionAllowed(sdkbox::FB_PERM_PUBLISH_POST))        return false;
    
    return true;
}

bool CFacebookManager::IsPermissionAllowed(std::string id)
{
    for(auto permission : sdkbox::PluginFacebook::getPermissionList())
        if(permission == id)
            return true;
    return false;
}

void CFacebookManager::OpenPhotoShareDialog(std::string title)
{
    auto captured = CFacebookManager::Instance()->getFacebookCapture();
    if (!captured.empty() && FileUtils::getInstance()->isFileExist(captured))
    {
        sdkbox::FBShareInfo info;
        info.type  = sdkbox::FB_PHOTO;
        info.title = title;
        info.image = captured;
        sdkbox::PluginFacebook::dialog(info);
    }
    else
        MessageBox("capture first", "Failed");
}

void CFacebookManager::OpenLinkShareDialog(std::string title, std::string text)
{
    sdkbox::FBShareInfo info;
    info.type  = sdkbox::FB_LINK;
    info.link  = CDownloadManager::Instance()->getAppUrl();
    info.title = title;
    info.text  = text;
    info.image = META_DATA("FACEBOOK_SHARE_IMAGE_URL").asString();
    sdkbox::PluginFacebook::dialog(info);
}

void CFacebookManager::OpenInviteDialog()
{
    sdkbox::PluginFacebook::inviteFriends(CDownloadManager::Instance()->getAppUrl(),
                                          META_DATA("FACEBOOK_SHARE_IMAGE_URL").asString());
}

// on "init" you need to initialize your instance
bool CFacebookManager::init()
{
    if (!Node::init()) return false;
    
    this->setContentSize(_director->getWinSize());
    this->setPosition(this->getContentSize() / 2);
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    sdkbox::PluginFacebook::setListener(this);

    m_MyFacebookData = new FBUSER_PARAM();
    
    CDownloadManager::IsNetworkConnected([=](bool isConnected){
        if(isConnected){
            CFacebookManager::RequestMyInfo();
            CFacebookManager::RequestFriendList();
        }
    });
    
    return true;
}

void CFacebookManager::onLogin(bool isLogin, const std::string& error)
{
    if(!isLogin) {
        std::string title = "login ";
        title.append((isLogin ? "success" : "failed"));
        MessageBox(title.c_str(), "Notice");
        m_LoginListener = nullptr;
        return;
    }
    
    this->callAPIListener(m_LoginListener);
}

void CFacebookManager::onAPI(const std::string& tag, const std::string& jsonData)
{
    CCLOG("##FB onAPI: tag -> %s, json -> %s", tag.c_str(), jsonData.c_str());
    Json::Value  root;
    Json::Reader reader;
    std::string  jsonDataStr = jsonData;
    
    jsonDataStr              = jsonDataStr.substr(0, jsonDataStr.rfind("}") + 1);
    bool parsingSuccessful   = reader.parse(jsonDataStr, root);
    if (!parsingSuccessful)
    {
        CCASSERT(false, MakeString("parser failed : \n %s", jsonDataStr.c_str()).c_str());
        return;
    }

    // if tag is "me" set my info of facebook
    if(tag == FACEBOOK_DEFINE::TAG_API_ME){
        m_MyDataInitialized = true;
        this->initFacebookUserDataByJson(m_MyFacebookData, root);
        this->callAPIListener(m_MyInfoListener);
    }
    
    // if tag is "friends" set friends info of facebook
    else if(tag == FACEBOOK_DEFINE::TAG_API_FRIENDS){
        auto userArray = root["data"];
        for(auto user : userArray){
            auto userData = createNewFriendData(user["id"].asString());
            this->initFacebookUserDataByJson(const_cast<FBUSER_PARAM*>(userData), user);
        }
        this->callAPIListener(m_FriendListListener);
    }
    
    // if tag is "save score" call listener
    else if(tag == FACEBOOK_DEFINE::TAG_API_SAVE_SCORE){
        this->RequestMyInfo();
        this->setMyInfoListener([=](){
            this->RequestFriendList();
            this->setFriendListListener([=](){
                this->callAPIListener(m_SaveScoreListener);
            });
        });
    }
}

void CFacebookManager::onSharedSuccess(const std::string& message)
{
    CCLOG("##FB onSharedSuccess:%s", message.c_str());
    MessageBox("Share succeed.", "Notice");
}

void CFacebookManager::onSharedFailed(const std::string& message)
{
    CCLOG("##FB onSharedFailed:%s", message.c_str());
    MessageBox("Share failed.", "Notice");
}

void CFacebookManager::onSharedCancel()
{
    CCLOG("##FB onSharedCancel");
    MessageBox("Share canceled.", "Notice");
}

void CFacebookManager::onPermission(bool isLogin, const std::string& error)
{
    CCLOG("##FB onPermission: %d, error: %s", isLogin, error.c_str());
    
    if(!isLogin) {
        std::string title = "permission ";
        title.append((isLogin ? "success" : "failed"));
        MessageBox(title.c_str(), "Notice");
        m_PermissionListener = nullptr;
        return;
    }

    this->callAPIListener(m_PermissionListener);
}

void CFacebookManager::onFetchFriends(bool ok, const std::string& msg)
{
    CCLOG("##FB %s: %d = %s", __FUNCTION__, ok, msg.data());

    const std::vector<sdkbox::FBGraphUser>& friends = sdkbox::PluginFacebook::getFriends();
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
        
//        CMyButton::create()
//        ->addEventListener([=](Node* sender){
//            sender->removeFromParent();
//            sdkbox::FBAPIParam params;
//            params["redirect"] = "false";
//            params["type"] = "small";
//            std::string url(user.uid + "/picture");
//            sdkbox::PluginFacebook::api(url, "GET", params, "__fetch_picture_tag__");
//        })
//        ->setButtonSingleUse(true)
//        ->setLayer(LayerColor::create(COLOR::BRIGHTGRAY_ALPHA, 430, 430))
//        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
//        ->setButtonPosition(this->getContentSize() / 2)
//        ->show(this);
    }
    
    MessageBox("Fetch friends.", "Notice");
}

void CFacebookManager::onRequestInvitableFriends( const sdkbox::FBInvitableFriendsInfo& friends )
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
    
//    CFacebookManager::RequestMyInfo();
//    CFacebookManager::RequestFriendList();
}

const FBUSER_PARAM* CFacebookManager::createNewFriendData(std::string id)
{
    auto data = m_FBFriendList.find(id);
    if(data == m_FBFriendList.end())
        m_FBFriendList.emplace(std::pair<std::string, const FBUSER_PARAM*>(id, new FBUSER_PARAM()));
    
    return (m_FBFriendList.find(id))->second;
}

void CFacebookManager::initFacebookUserDataByJson(FBUSER_PARAM* param,
                                                  const Json::Value& data)
{
    param->_userId          = data["id"].asString();
    param->_name            = data["name"].asString();
    param->_firstName       = data["first_name"].asString();
    param->_lastName        = data["last_name"].asString();
    param->_installed       = data["installed"].asBool();
    
    auto picture            = data["picture"]["data"];
    param->_url             = picture["url"].asString();
    param->_silhouette      = picture["is_silhouette"].asBool();
    
    auto score              = data["scores"]["data"];
    if(!score.isNull()){
        for(auto user : score){
            if(param->_userId == user["user"]["id"].asString())
                param->_score = user["score"].asInt();
        }
    }
}

void CFacebookManager::callAPIListener(API_LISTENER& listener)
{
    if(listener)
    {
        listener();
        listener = nullptr;
    }
}
