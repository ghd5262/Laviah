#include "FacebookManager.hpp"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "ui/UIScrollView.h"
#include "ui/UICheckBox.h"
#include "ui/UIButton.h"
#include "../MyUI/MyButton.h"
#include "../MyUI/UrlSprite.hpp"
#include "../DataManager/DataManagerUtils.h"

using namespace cocos2d;
using namespace cocos2d::ui;

CFacebookManager* CFacebookManager::m_Instance = nullptr;

CFacebookManager::~CFacebookManager()
{
    DATA_MANAGER_UTILS::mapDeleteAndClean(m_FBFriendList);
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
    
    // get sequence of me.
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
    params["fields"] = "id,name,first_name,last_name,picture{is_silhouette,url},installed,scores{score}";
    sdkbox::PluginFacebook::api("/me", "GET", params, FACEBOOK_DEFINE::TAG_API_ME);
}

void CFacebookManager::RequestFriendList()
{
    sdkbox::FBAPIParam params;
    params["fields"] = "id,name,first_name,last_name,picture{is_silhouette,url},installed,scores{score}";
    params["limit"] = "30";
    sdkbox::PluginFacebook::api("/me/friends", "GET", params, FACEBOOK_DEFINE::TAG_API_FRIENDS);
}

void CFacebookManager::SaveScore(int score)
{
    sdkbox::FBAPIParam params;
    params["score"] = StringUtils::format("%d", score);
    sdkbox::PluginFacebook::api("/me/scores", "POST", params, FACEBOOK_DEFINE::TAG_API_SAVE_SCORE);
}

// on "init" you need to initialize your instance
bool CFacebookManager::init()
{
    if (!Node::init()) return false;
    
    this->setContentSize(_director->getWinSize());
    this->setPosition(this->getContentSize() / 2);
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    sdkbox::PluginFacebook::setListener(this);
    sdkbox::PluginFacebook::init();

    m_MyFacebookData = new FBUSER_PARAM();
    
    this->RequestMyInfo();
    this->RequestFriendList();
    
    return true;
}

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
        this->initFacebookUserDataByJson(m_MyFacebookData, root);
        this->callAPIListener(true, m_MyInfoListener);
    }
    
    // if tag is "friends" set friends info of facebook
    else if(tag == FACEBOOK_DEFINE::TAG_API_FRIENDS){
        auto userArray = root["data"];
        for(auto user : userArray){
            auto userData = createNewFriendData(user["id"].asString());
            this->initFacebookUserDataByJson(const_cast<FBUSER_PARAM*>(userData), user);
        }
        this->callAPIListener(true, m_FriendListListener);
    }
    
    // if tag is "save score" call listener
    else if(tag == FACEBOOK_DEFINE::TAG_API_SAVE_SCORE){
        this->RequestMyInfo();
        this->setMyInfoListener([=](bool succeed){
            this->RequestFriendList();
            this->setFriendListListener([=](bool succeed){
                this->callAPIListener(true, m_SaveScoreListener);
            });
        });
    }
    
    
    
    
//    if (tag == "__fetch_picture_tag__") {
    
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
//        Json::Value  data        = root["data"];
//        std::string  url         = data["url"].asString();
//        
//        CCLOG("picture's url = %s", url.data());
        
//        auto btn = CMyButton::create()
//        ->addEventListener([=](Node* sender){
//            sender->removeFromParent();
//        })
//        ->setButtonSingleUse(true)
//        ->setLayer(LayerColor::create(COLOR::BRIGHTGRAY_ALPHA, 430, 430))
//        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
//        ->setButtonPosition(this->getContentSize() / 2)
//        ->show(this);
//        
//        CUrlSprite::create()
//        ->setUrl(url)
//        ->setSize(Size(400, 400))
//        ->build(btn)
//        ->setPosition(btn->getContentSize() / 2);
//    }
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
    
    MessageBox("", "fetch friends");
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

void CFacebookManager::callAPIListener(bool succeed, API_LISTENER& listener)
{
    if(listener)
    {
        listener(succeed);
        listener = nullptr;
    }
}

//#define TAG_MENU 1
//#define TAG_SCROLLVIEW 2
//#define TAG_CHECKBOX 1000
//
//class SpriteEx : public Sprite
//{
//public:
//    static SpriteEx* createWithUrl(const std::string& url) {
//        SpriteEx* sprite = new SpriteEx();
//        sprite->autorelease();
//        sprite->init();
//        sprite->updateWithUrl(url);
//        return sprite;
//    }
//    static SpriteEx* create() {
//        SpriteEx* sprite = new SpriteEx();
//        sprite->autorelease();
//        sprite->init();
//        return sprite;
//    }
//    
//    virtual bool init() {
//        _image_size.x = 0;
//        _image_size.y = 0;
//        return Sprite::init();
//    }
//    
//    void setSize(Vec2 s) {
//        _image_size = s;
//    }
//    
//    void updateWithUrl(const std::string& url) {
//        network::HttpRequest* request = new network::HttpRequest();
//        request->setUrl(url.data());
//        request->setRequestType(network::HttpRequest::Type::GET);
//        request->setResponseCallback([=](network::HttpClient* client, network::HttpResponse* response) {
//            if (!response->isSucceed()) {
//                CCLOG("ERROR, remote sprite load image failed");
//                return ;
//            }
//            
//            std::vector<char> *buffer = response->getResponseData();
//            Image img;
//            img.initWithImageData(reinterpret_cast<unsigned char*>(&(buffer->front())), buffer->size());
//            
//            if (0)
//            {
//                // save image file to device.
//                std::string path = FileUtils::getInstance()->getWritablePath()+"p.png";
//                CCLOG("save image path = %s", path.data());
//                bool ret = img.saveToFile(path);
//                CCLOG("save file %s", ret ? "success" : "failure");
//                
//                this->initWithFile(path);
//            } else {
//                
//                // create sprite with texture
//                Texture2D *texture = new Texture2D();
//                texture->autorelease();
//                texture->initWithImage(&img);
//                
//                this->initWithTexture(texture);
//                if (0 != _image_size.x) {
//                    auto size = getContentSize();
//                    setScaleX(_image_size.x/size.width);
//                    setScaleY(_image_size.y/size.height);
//                }
//            }
//        });
//        network::HttpClient::getInstance()->send(request);
//        request->release();
//    }
//    
//private:
//    cocos2d::Vec2 _image_size;
//};
//
//static void checkFaceBookStatus()
//{
//    CCLOG("##FB> permission list: ");
//    for (auto& permission : PluginFacebook::getPermissionList())
//    {
//        CCLOG("##FB>> permission %s", permission.data());
//    }
//    CCLOG("##FB> access token: %s", PluginFacebook::getAccessToken().c_str());
//    CCLOG("##FB> user id: %s", PluginFacebook::getUserID().c_str());
//    CCLOG("##FB> FBSDK version: %s", PluginFacebook::getSDKVersion().c_str());
//}

//
//CFacebookManager* CFacebookManager::m_Instance = nullptr;
//
//CFacebookManager::~CFacebookManager()
//{
//    m_Instance = nullptr;
//}
//
//CFacebookManager* CFacebookManager::Instance()
//{
//    if(m_Instance != nullptr) return m_Instance;
//    
//    m_Instance = new(std::nothrow) CFacebookManager();
//    if (m_Instance && m_Instance->init())
//    {
//        m_Instance->autorelease();
//        return m_Instance;
//    }
//    else
//    {
//        delete m_Instance;
//        m_Instance = NULL;
//        return NULL;
//    }
//}
//
//
//// on "init" you need to initialize your instance
//bool CFacebookManager::init()
//{
//    if (!Node::init()) return false;
//    
//    CCLOG("Sample Startup");
//    
//    _inviteDialog = nullptr;
//    _inviteTitle = "SDKBox Invite";
//    _inviteText = "SDKBox (www.sdkbox.com)";
//    _inviteableUsers.clear();
//    _inviteUserIdxs.clear();
//    
//    _iconSprite = SpriteEx::create();
//    _iconSprite->setPosition(_director->getWinSize() / 2);
//    addChild(_iconSprite);
//    
//    // add test menu
//    createTestMenu();
//    
//    PluginFacebook::setListener(this);
//    sdkbox::PluginFacebook::init();
//    
//    //    std::string url = "https://fbcdn-profile-a.akamaihd.net/hprofile-ak-prn2/v/t1.0-1/p320x320/537914_116355241828348_695849812_n.jpg?oh=848e9b8ad6257177ce7eee047405e7ce&oe=5878E7CD&__gda__=1484964719_334d018e76a5b5463849b573c95f90e6";
//    //    Sprite* sp = SpriteEx::createWithUrl(url);
//    //    sp ->setPosition(100, 100);
//    //    addChild(sp);
//    
//    return true;
//}
//
//void CFacebookManager::createTestMenu()
//{
//    MenuItemFont::setFontName("sans");
//    MenuItemFont::setFontSize(40);
//    
//    _captureFilename = "";
//    Size size = Director::getInstance()->getWinSize();
//    
//    // ui
//    {
//        std::string loginStat;
//        if (PluginFacebook::isLoggedIn())
//        {
//            loginStat = "Logout";
//        }
//        else
//        {
//            loginStat = "Login";
//        }
//        
////        auto button = CMyButton::create()
////        ->addEventListener([=](Ref* sender){
////            this->onLoginClick(sender);
////        })
////        ->setButtonNormalImage("resultPopup_1.png")
////        ->setContents("Login")
////        ->setFont(Color3B::WHITE, 50)
////        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
////        ->show(this);
//        
////        _loginItem = MenuItemFont::create(loginStat, CC_CALLBACK_1(CFacebookManager::onLoginClick, this));
//        
////        auto menu = Menu::create(_loginItem,
////                                 MenuItemFont::create("check status", CC_CALLBACK_1(CFacebookManager::onCheckStatus, this)),
////                                 MenuItemFont::create("requestReadPermission", CC_CALLBACK_1(CFacebookManager::onRequestReadPermission, this)),
////                                 MenuItemFont::create("requestPublishPermission", CC_CALLBACK_1(CFacebookManager::onRequestPublishPermission, this)),
////                                 MenuItemFont::create("capture screen", CC_CALLBACK_1(CFacebookManager::onCaptureScreen, this)),
////                                 MenuItemFont::create("share link", CC_CALLBACK_1(CFacebookManager::onShareLink, this)),
////                                 MenuItemFont::create("share photo", CC_CALLBACK_1(CFacebookManager::onSharePhoto, this)),
////                                 MenuItemFont::create("dialog link", CC_CALLBACK_1(CFacebookManager::onDialogLink, this)),
////                                 MenuItemFont::create("dialog photo(on device)", CC_CALLBACK_1(CFacebookManager::onDialogPhoto, this)),
////                                 MenuItemFont::create("my info", CC_CALLBACK_1(CFacebookManager::onGetMyInfo, this)),
////                                 MenuItemFont::create("my friends", CC_CALLBACK_1(CFacebookManager::onGetMyFriends, this)),
////                                 MenuItemFont::create("invite friends", CC_CALLBACK_1(CFacebookManager::onInviteFriends, this)),
////                                 MenuItemFont::create("invite friends (Custom Dialog)", CC_CALLBACK_1(CFacebookManager::onInviteFriendsCustomUI, this)),
////                                 NULL);
//        
////        menu->alignItemsVerticallyWithPadding(20);
////        menu->setPosition(Vec2(size.width/2, size.height/2));
////        menu->setTag(TAG_MENU);
////        addChild(menu);
//        
////        Label *userLabel = Label::createWithSystemFont("user: aydghli_riceberg_1435809241@tfbnw.net", "sans", 32);
////        userLabel->setPosition(Vec2(size.width/2, size.height - 30));
////        addChild(userLabel);
////        
////        Label *pwLabel = Label::createWithSystemFont("password: 123456", "sans", 32);
////        pwLabel->setPosition(Vec2(size.width/2, size.height - 30*2));
////        addChild(pwLabel);
//    }
//}
//
//void CFacebookManager::showInviteDialog() {
//    if (nullptr == _inviteDialog) {
//        createInviteDialog();
//        addChild(_inviteDialog, 99);
//    } else {
//        _inviteDialog->setVisible(true);
//    }
//    
//    FBAPIParam param;
//    PluginFacebook::requestInvitableFriends(param);
//    
//    //disable menu
//    auto n = dynamic_cast<Menu*>(getChildByTag(TAG_MENU));
//    if (nullptr != n) {
//        n->setEnabled(false);
//    }
//}
//
//void CFacebookManager::onCheckBoxState(Ref* r, cocos2d::ui::CheckBox::EventType event) {
//    auto cb = dynamic_cast<cocos2d::ui::CheckBox*>(r);
//    
//    int idx = cb->getTag() - TAG_CHECKBOX;
//    if (cocos2d::ui::CheckBox::EventType::SELECTED == event) {
//        bool bExist = false;
//        for (auto i : _inviteUserIdxs) {
//            if (i == idx) {
//                bExist = true;
//            }
//        }
//        if (!bExist) {
//            _inviteUserIdxs.push_back(idx);
//        }
//    } else if (cocos2d::ui::CheckBox::EventType::UNSELECTED == event) {
//        int pos = 0;
//        for (auto i : _inviteUserIdxs) {
//            if (i == idx) {
//                _inviteUserIdxs.erase(_inviteUserIdxs.begin() + pos);
//            }
//            pos++;
//        }
//    }
//}
//
//void CFacebookManager::onButtonCancel(Ref*) {
//    _inviteDialog->setVisible(false);
//    auto n = dynamic_cast<Menu*>(getChildByTag(TAG_MENU));
//    if (nullptr != n) {
//        n->setEnabled(true);
//    }
//}
//
//void CFacebookManager::onButtonInvite(Ref*) {
//    if (_inviteUserIdxs.size() < 1) {
//        CCLOG("Must select one friend at least");
//        return;
//    }
//    std::vector<std::string> invite_ids;
//    for(auto i : _inviteUserIdxs) {
//        invite_ids.push_back(_inviteableUsers[i].getUserId());
//    }
//    sdkbox::PluginFacebook::inviteFriendsWithInviteIds(invite_ids, _inviteTitle, _inviteText);
//}
//
//void CFacebookManager::createInviteDialog() {
//    int width = 0;
//    int height = 0;
//    auto winsize = Director::getInstance()->getWinSize();
//    
//    width = winsize.width * 4 / 5;
//    height = winsize.height * 3 / 5;
//    
//    _inviteDialog = new Node();
//    _inviteDialog->setPosition(winsize.width/2, winsize.height/2);
//    
//    //title bg
//    auto sp = Sprite::create("ui/blue_button03.png");
//    auto size = sp->getContentSize();
//    sp->setScaleX(width/size.width);
//    sp->setScaleY(1.5);
//    sp->setPosition(0, height/2 + size.height * 1.5 / 2 - 15);
//    _inviteDialog->addChild(sp);
//    //title text
//    auto label = Label::createWithSystemFont("Invite Friends", "sans", 35);
//    label->setPosition(0, height/2 + 20);
//    _inviteDialog->addChild(label);
//    
//    //bg
//    auto bgU = Sprite::create("ui/dropdownTop.png");
//    auto heightT = bgU->getContentSize().height;
//    auto scaleX = width/bgU->getContentSize().width;
//    bgU->setScaleX(scaleX);
//    bgU->setPosition(0, height/2 - heightT/2);
//    _inviteDialog->addChild(bgU);
//    
//    auto bgB = Sprite::create("ui/dropdownBottom.png");
//    auto heightB = bgB->getContentSize().height;
//    bgB->setScaleX(scaleX);
//    bgB->setPosition(0, -height/2 + heightB/2);
//    _inviteDialog->addChild(bgB);
//    
//    auto bgM = Sprite::create("ui/dropdownMid.png");
//    auto heigtM = bgM->getContentSize().height;
//    bgM->setScaleX(scaleX);
//    auto pos = bgU->getPosition().y - heightT/2;
//    while (true) {
//        if (pos < -height/2 + heightB/2) {
//            break;
//        }
//        auto clone = Sprite::createWithSpriteFrame(bgM->getSpriteFrame());
//        clone->setScaleX(scaleX);
//        clone->setPosition(0, pos - heigtM/2);
//        _inviteDialog->addChild(clone);
//        pos -= heigtM;
//    }
//    
//    int padding = 10;
//    pos = height/2;
//    
//    // invite title
//    pos -= padding * 2; //padding
//    label = Label::createWithSystemFont("Title:", "sans", 20);
//    label->setTextColor(Color4B(128, 128, 128, 255));
//    label->setPosition(-width/2 + label->getContentSize().width/2 + padding * 2, pos);
//    _inviteDialog->addChild(label);
//    pos -= label->getContentSize().height;
//    
//    pos -= padding;
//    label = Label::createWithSystemFont(_inviteTitle, "sans", 20);
//    label->setTextColor(Color4B(64, 64, 64, 255));
//    label->setPosition(-width/2 + label->getContentSize().width/2 + padding * 2, pos);
//    _inviteDialog->addChild(label);
//    pos -= label->getContentSize().height;
//    
//    //    pos -= padding;
//    //    sp = Sprite::create("ui/grey_sliderHorizontal.png");
//    //    sp->setPosition(0, pos);
//    //    sp->setScaleX((width - padding * 6)/sp->getContentSize().width);
//    //    _inviteDialog->addChild(sp);
//    //    pos -= sp->getContentSize().height;
//    
//    // invite text
//    pos -= padding;
//    label = Label::createWithSystemFont("Text:", "sans", 20);
//    label->setTextColor(Color4B(128, 128, 128, 255));
//    label->setPosition(-width/2 + label->getContentSize().width/2 + padding * 2, pos);
//    _inviteDialog->addChild(label);
//    pos -= label->getContentSize().height;
//    
//    pos -= padding;
//    label = Label::createWithSystemFont(_inviteText, "sans", 20);
//    label->setTextColor(Color4B(64, 64, 64, 255));
//    label->setPosition(-width/2 + label->getContentSize().width/2 + padding * 2, pos);
//    _inviteDialog->addChild(label);
//    pos -= label->getContentSize().height;
//    
//    pos -= padding;
//    sp = Sprite::create("ui/grey_sliderHorizontal.png");
//    sp->setPosition(0, pos);
//    sp->setScaleX((width - padding * 6)/sp->getContentSize().width);
//    _inviteDialog->addChild(sp);
//    pos -= sp->getContentSize().height;
//    
//    //invite friend
//    pos -= padding;
//    label = Label::createWithSystemFont("To:", "sans", 20);
//    label->setTextColor(Color4B(128, 128, 128, 255));
//    label->setPosition(-width/2 + label->getContentSize().width/2 + padding * 2, pos);
//    _inviteDialog->addChild(label);
//    pos -= label->getContentSize().height;
//    
//    int buttonH = 50;
//    // scrollview for friends
//    pos -= padding;
//    ui::ScrollView* sv = ui::ScrollView::create();
//    Size svSize = Size(width - padding * 2, pos + height/2 - padding * 2 - buttonH);
//    sv->setContentSize(svSize);
//    sv->setPosition(Vec2(-width/2 + padding, -height/2 + padding + buttonH));
//    _inviteDialog->addChild(sv);
//    sv->setTag(TAG_SCROLLVIEW);
//    pos -= svSize.height;
//    
//    //cancel ok button
//    auto button = cocos2d::ui::Button::create("ui/red_button04.png", "ui/red_button05.png");
//    button->setPosition(Vec2(- width/4, pos - buttonH/2));
//    button->setScale9Enabled(true);
//    button->setTitleText("Cancel");
//    button->setTitleFontSize(15);
//    auto btnSize = button->getContentSize();
//    btnSize.width = width/4;
//    button->setContentSize(btnSize);
//    button->addClickEventListener(CC_CALLBACK_1(CFacebookManager::onButtonCancel, this));
//    _inviteDialog->addChild(button);
//    
//    button = cocos2d::ui::Button::create("ui/green_button07.png", "ui/green_button08.png");
//    button->setPosition(Vec2(width/4, pos - buttonH/2));
//    button->setScale9Enabled(true);
//    button->setTitleText("Invite");
//    button->setTitleFontSize(15);
//    btnSize = button->getContentSize();
//    btnSize.width = width/4;
//    button->setContentSize(btnSize);
//    button->addClickEventListener(CC_CALLBACK_1(CFacebookManager::onButtonInvite, this));
//    _inviteDialog->addChild(button);
//    
//    //add friends to srollview
//    int column = 3;
//    int x = 0, y = 0;
//    int i = 0;
//    pos = 0;
//    int userHeight = 60;
//    Size innerSize = svSize;
//    innerSize.height = _inviteableUsers.size()/column;
//    if (0 != _inviteableUsers.size()%column) {
//        innerSize.height += 1;
//    }
//    innerSize.height *= userHeight;
//    sv->setInnerContainerSize(innerSize);
//    for (auto user : _inviteableUsers) {
//        x = i % column;
//        y = i / column;
//        
//        Node* n = new Node();
//        n->setPosition(x * innerSize.width/column, innerSize.height - userHeight * (y + 0.5));
//        sv->addChild(n);
//        int posX = 0;
//        
//        cocos2d::ui::CheckBox* cb = cocos2d::ui::CheckBox::create("ui/green_button06.png", "ui/green_checkmark.png");
//        posX += cb->getContentSize().width/2;
//        cb->setPosition(Vec2(posX, 0));
//        n->addChild(cb);
//        posX += cb->getContentSize().width/2;
//        cb->setTag(TAG_CHECKBOX + i);
//        cb->addEventListener(std::bind(&CFacebookManager::onCheckBoxState, this, std::placeholders::_1, std::placeholders::_2));
//        
//        //head image
//        sp = Sprite::create("ui/red_circle.png");
//        sp->setPosition(posX + sp->getContentSize().width/2, 0);
//        posX += sp->getContentSize().width;
//        n->addChild(sp);
//        
//        //user name
//        label = Label::createWithSystemFont(user.getName(), "sans", 20);
//        label->setTextColor(Color4B(16, 16, 16, 255));
//        label->setPosition(posX + label->getContentSize().width/2, 0);
//        n->addChild(label);
//        
//        i++;
//    }
//}
//
//void CFacebookManager::showInviteableFriends() {
//    cocos2d::ui::ScrollView* sv = dynamic_cast<cocos2d::ui::ScrollView*>(_inviteDialog->getChildByTag(TAG_SCROLLVIEW));
//    if (nullptr == sv) {
//        return;
//    }
//    sv->removeAllChildren();
//    
//    int column = 2;
//    int x = 0, y = 0;
//    int i = 0;
//    int userHeight = 60;
//    SpriteEx* sp = nullptr;
//    cocos2d::Label* label = nullptr;
//    Size innerSize = sv->getContentSize();
//    innerSize.height = _inviteableUsers.size()/column;
//    if (0 != _inviteableUsers.size()%column) {
//        innerSize.height += 1;
//    }
//    innerSize.height *= userHeight;
//    if (innerSize.height < sv->getContentSize().height) {
//        innerSize.height = sv->getContentSize().height;
//    }
//    sv->setInnerContainerSize(innerSize);
//    for (auto user : _inviteableUsers) {
//        x = i % column;
//        y = i / column;
//        
//        Node* n = new Node();
//        n->setPosition(x * innerSize.width/column, innerSize.height - userHeight * (y + 0.5));
//        sv->addChild(n);
//        int posX = 0;
//        
//        cocos2d::ui::CheckBox* cb = cocos2d::ui::CheckBox::create("ui/green_button06.png", "ui/green_checkmark.png");
//        posX += cb->getContentSize().width/2;
//        cb->setPosition(Vec2(posX, 0));
//        n->addChild(cb);
//        posX += cb->getContentSize().width/2;
//        cb->setTag(TAG_CHECKBOX + i);
//        cb->addEventListener(std::bind(&CFacebookManager::onCheckBoxState, this, std::placeholders::_1, std::placeholders::_2));
//        
//        //head image
//        //sp = Sprite::create("ui/red_circle.png");
//        sp = SpriteEx::createWithUrl(user.getPictureURL());
//        sp->setPosition(posX + 50/2, 0);
//        sp->setSize(Vec2(50, 50));
//        posX += 50;
//        n->addChild(sp);
//        //CCLOG("user head image: %s", user.getPictureURL().c_str());
//        
//        //user name
//        label = Label::createWithSystemFont(user.getName(), "sans", 20);
//        label->setTextColor(Color4B(16, 16, 16, 255));
//        label->setPosition(posX + label->getContentSize().width/2, 0);
//        n->addChild(label);
//        
//        i++;
//    }
//}
//
//
//void CFacebookManager::onLoginClick(cocos2d::Ref *sender)
//{
//    CCLOG("##FB %s", __FUNCTION__);
//    if (PluginFacebook::isLoggedIn())
//    {
//        PluginFacebook::logout();
//        _loginItem->setString("Login");
//    }
//    else
//    {
//        std::vector<std::string> permissions;
//        permissions.push_back(sdkbox::FB_PERM_READ_EMAIL);
//        permissions.push_back(sdkbox::FB_PERM_READ_USER_FRIENDS);
//        PluginFacebook::login(permissions);
//        //        PluginFacebook::login();
//    }
//}
//
//void CFacebookManager::onCheckStatus(cocos2d::Ref* sender)
//{
//    CCLOG("##FB %s", __FUNCTION__);
//    checkFaceBookStatus();
//}
//
//void CFacebookManager::onGetMyInfo(cocos2d::Ref* sender)
//{
//    CCLOG("##FB %s", __FUNCTION__);
//    
//    sdkbox::FBAPIParam params;
//    PluginFacebook::api("me", "GET", params, "me");
//}
//
//void CFacebookManager::onGetMyFriends(cocos2d::Ref* sender)
//{
//    CCLOG("##FB %s", __FUNCTION__);
//    
//    sdkbox::PluginFacebook::fetchFriends();
//}
//
//void CFacebookManager::onCaptureScreen(cocos2d::Ref *sender)
//{
//    CCLOG("##FB %s", __FUNCTION__);
//    
//    utils::captureScreen(CC_CALLBACK_2(CFacebookManager::afterCaptureScreen, this), "screen.png");
//}
//
//void CFacebookManager::afterCaptureScreen(bool yes, const std::string &outputFilename)
//{
//    CCLOG("##FB afterCaptureScreen: %s", outputFilename.c_str());
//    if (yes)
//    {
//        _captureFilename = outputFilename;
//    }
//}
//
//void CFacebookManager::onShareLink(cocos2d::Ref* sender)
//{
//    CCLOG("##FB %s", __FUNCTION__);
//    
//    FBShareInfo info;
//    info.type  = FB_LINK;
//    info.link  = "http://www.cocos2d-x.org";
//    info.title = "cocos2d-x";
//    info.text  = "Best Game Engine";
//    info.image = "http://cocos2d-x.org/images/logo.png";
//    PluginFacebook::share(info);
//}
//
//void CFacebookManager::onSharePhoto(cocos2d::Ref* sender)
//{
//    CCLOG("##FB %s", __FUNCTION__);
//    
//    if (!_captureFilename.empty() && FileUtils::getInstance()->isFileExist(_captureFilename))
//    {
//        CCLOG("##FB dialog photo: %s", _captureFilename.c_str());
//        
//        FBShareInfo info;
//        info.type  = FB_PHOTO;
//        info.title = "capture screen";
//        info.image = _captureFilename;
//        PluginFacebook::share(info);
//    }
//    else
//    {
//        CCLOG("##FB capture screen first");
//    }
//}
//
//void CFacebookManager::onDialogLink(cocos2d::Ref* sender)
//{
//    CCLOG("##FB %s", __FUNCTION__);
//    
//    FBShareInfo info;
//    info.type  = FB_LINK;
//    info.link  = "http://www.cocos2d-x.org";
//    info.title = "cocos2d-x";
//    info.text  = "Best Game Engine";
//    info.image = "http://cocos2d-x.org/images/logo.png";
//    PluginFacebook::dialog(info);
//}
//
//void CFacebookManager::onDialogPhoto(cocos2d::Ref* sender)
//{
//    CCLOG("##FB %s", __FUNCTION__);
//    
//    if (!_captureFilename.empty() && FileUtils::getInstance()->isFileExist(_captureFilename))
//    {
//        CCLOG("dialog photo: %s", _captureFilename.c_str());
//        FBShareInfo info;
//        info.type  = FB_PHOTO;
//        info.title = "capture screen";
//        info.image = _captureFilename;
//        PluginFacebook::dialog(info);
//    }
//    else
//    {
//        CCLOG("##FB capture screen first");
//    }
//}
//
//void CFacebookManager::onRequestReadPermission(cocos2d::Ref *sender)
//{
//    CCLOG("##FB %s", __FUNCTION__);
//    
//    PluginFacebook::requestReadPermissions({FB_PERM_READ_USER_FRIENDS});
//}
//
//void CFacebookManager::onRequestPublishPermission(cocos2d::Ref *sender)
//{
//    CCLOG("##FB %s", __FUNCTION__);
//    
//    PluginFacebook::requestPublishPermissions({FB_PERM_PUBLISH_POST});
//}
//
//void CFacebookManager::onInviteFriends(cocos2d::Ref* sender )
//{
//    PluginFacebook::inviteFriends(
//                                  //"https://fb.me/322164761287181",
//                                  "https://fb.me/402104549959868",
//                                  "http://www.cocos2d-x.org/attachments/802/cocos2dx_landscape.png");
//}
//
//void CFacebookManager::onInviteFriendsCustomUI(cocos2d::Ref* sender ) {
//    showInviteDialog();
//}
//
///*********************
// * Facebook callbacks
// *********************/
//void CFacebookManager::onLogin(bool isLogin, const std::string& error)
//{
//    CCLOG("##FB isLogin: %d, error: %s", isLogin, error.c_str());
//    
//    if (isLogin)
//    {
//        _loginItem->setString("Logout");
//    }
//    
//    std::string title = "login ";
//    title.append((isLogin ? "success" : "failed"));
//    //MessageBox(error.c_str(), title.c_str());
//}
//
//void CFacebookManager::onAPI(const std::string& tag, const std::string& jsonData)
//{
//    CCLOG("##FB onAPI: tag -> %s, json -> %s", tag.c_str(), jsonData.c_str());
//    if (tag == "__fetch_picture_tag__") {
//        //        picojson::value v;
//        //        picojson::parse(v, jsonData);
//        //        std::string url = v.get("data").get("url").to_str();
//        
//        //        Json::Value  root;
//        //        Json::Reader reader;
//        //        std::string  jsonDataStr = jsonData;
//        //
//        //        jsonDataStr              = jsonDataStr.substr(0, jsonDataStr.rfind("}") + 1);
//        //        bool parsingSuccessful   = reader.parse(jsonDataStr, root);
//        //        if (!parsingSuccessful)
//        //        {
//        //            CCASSERT(false, MakeString("parser failed : \n %s", jsonDataStr.c_str()).c_str());
//        //            return;
//        //        }
//        //        std::string  url         = root
//        
//        //        CCLOG("picture's url = %s", url.data());
//        //
//        //        _iconSprite->updateWithUrl(url);
//    }
//}
//
//void CFacebookManager::onSharedSuccess(const std::string& message)
//{
//    CCLOG("##FB onSharedSuccess:%s", message.c_str());
//    
//    MessageBox(message.c_str(), "share success");
//}
//
//void CFacebookManager::onSharedFailed(const std::string& message)
//{
//    CCLOG("##FB onSharedFailed:%s", message.c_str());
//    
//    MessageBox(message.c_str(), "share failed");
//}
//
//void CFacebookManager::onSharedCancel()
//{
//    CCLOG("##FB onSharedCancel");
//    
//    MessageBox("", "share cancel");
//}
//
//void CFacebookManager::onPermission(bool isLogin, const std::string& error)
//{
//    CCLOG("##FB onPermission: %d, error: %s", isLogin, error.c_str());
//    
//    std::string title = "permission ";
//    title.append((isLogin ? "success" : "failed"));
//    MessageBox(error.c_str(), title.c_str());
//}
//
//void CFacebookManager::onFetchFriends(bool ok, const std::string& msg)
//{
//    CCLOG("##FB %s: %d = %s", __FUNCTION__, ok, msg.data());
//    
//    MenuItemFont::setFontSize(20);
//    static Menu *menu = Menu::create();
//    menu->setPositionY(20);
//    menu->cleanup();
//    
//    const std::vector<sdkbox::FBGraphUser>& friends = PluginFacebook::getFriends();
//    for (int i = 0; i < friends.size(); i++)
//    {
//        const sdkbox::FBGraphUser& user = friends.at(i);
//        CCLOG("##FB> -------------------------------");
//        CCLOG("##FB>> %s", user.uid.data());
//        CCLOG("##FB>> %s", user.firstName.data());
//        CCLOG("##FB>> %s", user.lastName.data());
//        CCLOG("##FB>> %s", user.name.data());
//        CCLOG("##FB>> %s", user.isInstalled ? "app is installed" : "app is not installed");
//        CCLOG("##FB");
//        
//        MenuItemFont *item = MenuItemFont::create(user.name, [=](Ref*) {
//            sdkbox::FBAPIParam params;
//            params["redirect"] = "false";
//            params["type"] = "small";
//            std::string url(user.uid + "/picture");
//            PluginFacebook::api(url, "GET", params, "__fetch_picture_tag__");
//        });
//        menu->addChild(item);
//    }
//    if (!menu->getParent()) {
//        menu->alignItemsHorizontally();
//        addChild(menu);
//    }
//    
//    MessageBox("", "fetch friends");
//}
//
//void CFacebookManager::onRequestInvitableFriends( const FBInvitableFriendsInfo& friends )
//{
//    CCLOG("Request Inviteable Friends Begin");
//    for (auto it = friends.begin(); it != friends.end(); ++it) {
//        _inviteableUsers.push_back(*it);
//        CCLOG("Invitable friend: %s", it->getName().c_str());
//    }
//    CCLOG("Request Inviteable Friends End");
//    if (0 == _inviteableUsers.size()) {
//        CCLOG("WARNING! Your Invitable Friends number is 0");
//    }
//    showInviteableFriends();
//}
//
//void CFacebookManager::onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg )
//{
//    CCLOG("on invite friends with invite ids %s= '%s'", result?"ok":"error", msg.c_str());
//}
//
//void CFacebookManager::onInviteFriendsResult( bool result, const std::string& msg )
//{
//    CCLOG("on invite friends %s= '%s'", result?"ok":"error", msg.c_str());
//}
//
//void CFacebookManager::onGetUserInfo( const sdkbox::FBGraphUser& userInfo )
//{
//    CCLOG("Facebook id:'%s' name:'%s' last_name:'%s' first_name:'%s' email:'%s' installed:'%d'",
//          userInfo.getUserId().c_str(),
//          userInfo.getName().c_str(),
//          userInfo.getFirstName().c_str(),
//          userInfo.getLastName().c_str(),
//          userInfo.getEmail().c_str(),
//          userInfo.isInstalled ? 1 : 0
//          );
//}
