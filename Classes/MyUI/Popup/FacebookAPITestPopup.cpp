#include "FacebookAPITestPopup.hpp"
#include "../MyButton.h"
#include "../UrlSprite.hpp"
#include "../../Scene/GameScene.h"
#include "../../SDKUtil/SDKUtil.h"
#include "../../GameObject/ObjectManager.h"
#include "../../Facebook/FacebookManager.hpp"
#include "../../json/json.h"
#include "ui/UIScrollView.h"
#include <array>

#define TAG_MENU 1
#define TAG_SCROLLVIEW 2
#define TAG_CHECKBOX 1000

CFacebookAPITestPopup* CFacebookAPITestPopup::create()
{
    CFacebookAPITestPopup *pRet = new(std::nothrow) CFacebookAPITestPopup();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool CFacebookAPITestPopup::init()
{
    if (!CPopup::init()) return false;
    
    auto popupSize = this->getContentSize();
    
    
    auto createBtn = [=](const std::function<void(Node*)> &callback, std::string content){
        auto button = CMyButton::create()
        ->addEventListener(callback)
        ->setButtonNormalImage("resultPopup_1.png")
        ->setContents(content)
        ->setFont(Color3B::WHITE, 50)
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->show(this);
        button->setOpacity(0);
        
        return button;
    };
    
    std::array<std::string, 16> titleArray = {
        "Login",
        "CheckStatus",
        "GetMyInfo",
        "GetMyFriends",
        "CaptureScreen",
        "ShareLink",
        "SharePhoto",
        "DialogLink",
        "DialogPhoto",
        "RequestReadPermission",
        "RequestPublishPermission",
        "InviteFriends",
        "InviteFriendsCustomUI",
        "GetMyScore",
        "GetFriendsScore",
        "SetMyScore",
    };
    
    
    auto Login              = createBtn([=](Node* s){ this->Login(s);                    }, titleArray[0] );
    auto CheckStatus        = createBtn([=](Node* s){ this->CheckStatus(s);              }, titleArray[1] );
    auto GetMyInfo          = createBtn([=](Node* s){ this->GetMyInfo(s);                }, titleArray[2] );
    auto GetMyFriends       = createBtn([=](Node* s){ this->GetMyFriends(s);             }, titleArray[3] );
    auto CaptureScreen      = createBtn([=](Node* s){ this->CaptureScreen(s);            }, titleArray[4] );
    auto ShareLink          = createBtn([=](Node* s){ this->ShareLink(s);                }, titleArray[5] );
    auto SharePhoto         = createBtn([=](Node* s){ this->SharePhoto(s);               }, titleArray[6] );
    auto DialogLink         = createBtn([=](Node* s){ this->DialogLink(s);               }, titleArray[7] );
    auto DialogPhoto        = createBtn([=](Node* s){ this->DialogPhoto(s);              }, titleArray[8] );
    auto ReqReadPermiss     = createBtn([=](Node* s){ this->RequestReadPermission(s);    }, titleArray[9] );
    auto ReqPublishPermiss  = createBtn([=](Node* s){ this->RequestPublishPermission(s); }, titleArray[10] );
    auto InviteFriends      = createBtn([=](Node* s){ this->InviteFriends(s);            }, titleArray[11] );
    auto InviteFriendsUI    = createBtn([=](Node* s){ this->InviteFriendsCustomUI(s);    }, titleArray[12] );
    auto GetMyScore         = createBtn([=](Node* s){ this->GetMyScore(s);               }, titleArray[13] );
    auto GetFriendsScore    = createBtn([=](Node* s){ this->GetFriendsScore(s);          }, titleArray[14] );
    auto SetMyScore         = createBtn([=](Node* s){ this->SetMyScore(s);               }, titleArray[15] );

    struct buttonInfo {
        Node* _btn;
        Vec2  _pos;
        buttonInfo(Node* btn, Vec2 pos)
        : _btn(btn)
        , _pos(pos){};
    };
    
    std::array<buttonInfo, 16> startPos = {
        buttonInfo(Login            , Vec2(popupSize.width * 0.5f, popupSize.height * 0.8f)),
        buttonInfo(CheckStatus      , Vec2(popupSize.width * 0.5f, popupSize.height * 0.7f)),
        buttonInfo(GetMyInfo        , Vec2(popupSize.width * 0.5f, popupSize.height * 0.6f)),
        buttonInfo(GetMyFriends     , Vec2(popupSize.width * 0.5f, popupSize.height * 0.5f)),
        buttonInfo(CaptureScreen    , Vec2(popupSize.width * 0.5f, popupSize.height * 0.4f)),
        buttonInfo(ShareLink        , Vec2(popupSize.width * 0.5f, popupSize.height * 0.3f)),
        buttonInfo(SharePhoto       , Vec2(popupSize.width * 0.5f, popupSize.height * 0.2f)),
        buttonInfo(DialogLink       , Vec2(popupSize.width * 0.5f, popupSize.height * 0.1f)),
        buttonInfo(DialogPhoto      , Vec2(popupSize.width * 0.5f, popupSize.height * 0.0f)),
        buttonInfo(ReqReadPermiss   , Vec2(popupSize.width * 0.5f, popupSize.height * -0.1f)),
        buttonInfo(ReqPublishPermiss, Vec2(popupSize.width * 0.5f, popupSize.height * -0.2f)),
        buttonInfo(InviteFriends    , Vec2(popupSize.width * 0.5f, popupSize.height * -0.3f)),
        buttonInfo(InviteFriendsUI  , Vec2(popupSize.width * 0.5f, popupSize.height * -0.4f)),
        buttonInfo(GetMyScore       , Vec2(popupSize.width * 0.5f, popupSize.height * -0.5f)),
        buttonInfo(GetFriendsScore  , Vec2(popupSize.width * 0.5f, popupSize.height * -0.6f)),
        buttonInfo(SetMyScore       , Vec2(popupSize.width * 0.5f, popupSize.height * -0.7f)),

    };
    
    std::array<buttonInfo, 16> targetPos = {
        buttonInfo(Login            , Vec2(popupSize.width * 0.5f, popupSize.height * 0.9f)),
        buttonInfo(CheckStatus      , Vec2(popupSize.width * 0.5f, popupSize.height * 0.85f)),
        buttonInfo(GetMyInfo        , Vec2(popupSize.width * 0.5f, popupSize.height * 0.8f)),
        buttonInfo(GetMyFriends     , Vec2(popupSize.width * 0.5f, popupSize.height * 0.75f)),
        buttonInfo(CaptureScreen    , Vec2(popupSize.width * 0.5f, popupSize.height * 0.7f)),
        buttonInfo(ShareLink        , Vec2(popupSize.width * 0.5f, popupSize.height * 0.65f)),
        buttonInfo(SharePhoto       , Vec2(popupSize.width * 0.5f, popupSize.height * 0.6f)),
        buttonInfo(DialogLink       , Vec2(popupSize.width * 0.5f, popupSize.height * 0.55f)),
        buttonInfo(DialogPhoto      , Vec2(popupSize.width * 0.5f, popupSize.height * 0.5f)),
        buttonInfo(ReqReadPermiss   , Vec2(popupSize.width * 0.5f, popupSize.height * 0.45f)),
        buttonInfo(ReqPublishPermiss, Vec2(popupSize.width * 0.5f, popupSize.height * 0.4f)),
        buttonInfo(InviteFriends    , Vec2(popupSize.width * 0.5f, popupSize.height * 0.35f)),
        buttonInfo(InviteFriendsUI  , Vec2(popupSize.width * 0.5f, popupSize.height * 0.3f)),
        buttonInfo(GetMyScore       , Vec2(popupSize.width * 0.5f, popupSize.height * 0.25f)),
        buttonInfo(GetFriendsScore  , Vec2(popupSize.width * 0.5f, popupSize.height * 0.2f)),
        buttonInfo(SetMyScore       , Vec2(popupSize.width * 0.5f, popupSize.height * 0.15f))
    };
    
    auto btnEnd = CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->End(sender);
    })
    ->setButtonSingleUse(true)
    ->setButtonNormalImage("endIcon.png")
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(Vec2(popupSize.width * 0.92f, popupSize.height * 0.05f))
    ->show(this);
    
    btnEnd->setOpacity(0);
    btnEnd->setCascadeOpacityEnabled(true);
    
    
    CFacebookManager::Instance()->setLoginListener([=](bool isLogin, std::string error){
        if (isLogin)
            Login->changeContents("Logout");
        
        std::string title = "login ";
        title.append((isLogin ? "success" : "failed"));
        MessageBox(error.c_str(), title.c_str());
    });
    
    CFacebookManager::Instance()->setInvitableFriendsListener([=](const sdkbox::FBInvitableFriendsInfo &friends){
        CCLOG("Request Inviteable Friends Begin");
        for (auto it = friends.begin(); it != friends.end(); ++it) {
            m_InviteableUsers.push_back(*it);
            CCLOG("Invitable friend: %s", it->getName().c_str());
        }
        CCLOG("Request Inviteable Friends End");
        
        if (!m_InviteableUsers.size()) {
            CCLOG("WARNING! Your Invitable Friends number is 0");
        }
    });
    
    this->setOpenAnimation([=](Node* sender){
        auto action = [=](Node* btn, Vec2 pos){
            auto move        = MoveTo::create(1.3f, pos);
            auto exponential = EaseExponentialOut::create(move);
            auto fadein      = FadeIn::create(1.f);
            auto spawn       = Spawn::createWithTwoActions(exponential, fadein);
            
            btn->runAction(spawn);
        };
        
        for(int index = 0; index < targetPos.size(); index++){
            targetPos.at(index)._btn->setPosition(startPos.at(index)._pos);
            action(targetPos.at(index)._btn, targetPos.at(index)._pos);
        }
        btnEnd->runAction(FadeIn::create(0.5f));
    });
    
    this->setCloseAnimation([=](Node* sender){
        
        auto action = [=](Node* sprite, Vec2 pos){
            auto move       = MoveTo::create(0.35f, pos);
            auto sine       = EaseSineIn::create(move);
            auto fadeout    = FadeTo::create(0.2f, 0);
            auto spawn      = Spawn::createWithTwoActions(sine, fadeout);
            
            sprite->runAction(spawn);
        };
        
        for(int index = 0; index < startPos.size(); index++){
            action(startPos.at(index)._btn, startPos.at(index)._pos);
        }
        
        btnEnd->runAction(FadeTo::create(0.5f, 0));
    });
    
    this->setDefaultCallback([=](Node* sender){
        this->End(sender);
    });
    
    return true;
}

void CFacebookAPITestPopup::End(Node* sender){
    this->popupClose();
}

void CFacebookAPITestPopup::Login(cocos2d::Node* sender){
    CCLOG("##FB %s", __FUNCTION__);
    if (sdkbox::PluginFacebook::isLoggedIn())
    {
        sdkbox::PluginFacebook::logout();
        dynamic_cast<CMyButton*>(sender)->changeContents("Login");
    }
    else
    {
        std::vector<std::string> permissions;
        permissions.push_back(sdkbox::FB_PERM_READ_EMAIL);
        permissions.push_back(sdkbox::FB_PERM_READ_USER_FRIENDS);
        sdkbox::PluginFacebook::login(permissions);
    }
}

void CFacebookAPITestPopup::CheckStatus(cocos2d::Node* sender){
    CCLOG("##FB %s", __FUNCTION__);
    CFacebookManager::CheckFacebookStatus();
}

void CFacebookAPITestPopup::GetMyInfo(cocos2d::Node* sender){
    CCLOG("##FB %s", __FUNCTION__);
    
    sdkbox::FBAPIParam params;
    sdkbox::PluginFacebook::api("me", "GET", params, "me");
}

void CFacebookAPITestPopup::GetMyFriends(cocos2d::Node* sender){
    CCLOG("##FB %s", __FUNCTION__);
    
    sdkbox::PluginFacebook::fetchFriends();
}

void CFacebookAPITestPopup::CaptureScreen(cocos2d::Node* sender){
    CCLOG("##FB %s", __FUNCTION__);
    
    utils::captureScreen([=](bool yes, const std::string &outputFilename){}, "FBCapture.png");
}

void CFacebookAPITestPopup::ShareLink(cocos2d::Node* sender){
    CCLOG("##FB %s", __FUNCTION__);
    
    sdkbox::FBShareInfo info;
    info.type  = sdkbox::FB_LINK;
    info.link  = "http://www.cocos2d-x.org";
    info.title = "cocos2d-x";
    info.text  = "Best Game Engine";
    info.image = "http://cocos2d-x.org/images/logo.png";
    sdkbox::PluginFacebook::share(info);
}

void CFacebookAPITestPopup::SharePhoto(cocos2d::Node* sender){
    CCLOG("##FB %s", __FUNCTION__);
    
    if (!m_FacebookCapture.empty() && FileUtils::getInstance()->isFileExist(m_FacebookCapture))
    {
        CCLOG("##FB dialog photo: %s", m_FacebookCapture.c_str());
        
        sdkbox::FBShareInfo info;
        info.type  = sdkbox::FB_PHOTO;
        info.title = "capture screen";
        info.image = m_FacebookCapture;
        sdkbox::PluginFacebook::share(info);
    }
    else
    {
        CCLOG("##FB capture screen first");
    }
}

void CFacebookAPITestPopup::DialogLink(cocos2d::Node* sender){
    CCLOG("##FB %s", __FUNCTION__);
    
    sdkbox::FBShareInfo info;
    info.type  = sdkbox::FB_LINK;
    info.link  = "http://www.cocos2d-x.org";
    info.title = "cocos2d-x";
    info.text  = "Best Game Engine";
    info.image = "http://cocos2d-x.org/images/logo.png";
    sdkbox::PluginFacebook::dialog(info);
}

void CFacebookAPITestPopup::DialogPhoto(cocos2d::Node* sender){
    CCLOG("##FB %s", __FUNCTION__);
    
    if (!m_FacebookCapture.empty() && FileUtils::getInstance()->isFileExist(m_FacebookCapture))
    {
        CCLOG("dialog photo: %s", m_FacebookCapture.c_str());
        sdkbox::FBShareInfo info;
        info.type  = sdkbox::FB_PHOTO;
        info.title = "capture screen";
        info.image = m_FacebookCapture;
        sdkbox::PluginFacebook::dialog(info);
    }
    else
    {
        CCLOG("##FB capture screen first");
    }
}

void CFacebookAPITestPopup::RequestReadPermission(cocos2d::Node* sender){
    CCLOG("##FB %s", __FUNCTION__);
    sdkbox::PluginFacebook::requestReadPermissions({sdkbox::FB_PERM_READ_USER_FRIENDS});
}

void CFacebookAPITestPopup::RequestPublishPermission(cocos2d::Node* sender){
    CCLOG("##FB %s", __FUNCTION__);
    sdkbox::PluginFacebook::requestPublishPermissions({sdkbox::FB_PERM_PUBLISH_POST});
}

void CFacebookAPITestPopup::InviteFriends(cocos2d::Node* sender){
    sdkbox::PluginFacebook::inviteFriends(//"https://fb.me/322164761287181",
                                  "https://fb.me/402104549959868",
                                  "http://www.cocos2d-x.org/attachments/802/cocos2dx_landscape.png");
}

void CFacebookAPITestPopup::InviteFriendsCustomUI(cocos2d::Node* sender){
    showInviteDialog();
}

void CFacebookAPITestPopup::GetMyScore(cocos2d::Node* sender){
    sdkbox::FBAPIParam params;
    sdkbox::PluginFacebook::api("/me/scores", "GET", params, "/me/scores");
}

void CFacebookAPITestPopup::GetFriendsScore(cocos2d::Node* sender){
    sdkbox::FBAPIParam params;
    sdkbox::PluginFacebook::api("/app/scores", "GET", params, "/app/scores");
}

void CFacebookAPITestPopup::SetMyScore(cocos2d::Node* sender){
    sdkbox::FBAPIParam params;
    
    Json::StyledWriter writer;
    Json::Value jsonObject;
    jsonObject["score"] = random<int>(0, 100);
    params["/me/scores"] = writer.write(jsonObject);
    
    sdkbox::PluginFacebook::api("/me/scores", "POST", params, "/me/scores");
}

void CFacebookAPITestPopup::showInviteDialog(){
    if (nullptr == m_InviteDialog) {
        createInviteDialog();
        addChild(m_InviteDialog, 99);
    } else {
        m_InviteDialog->setVisible(true);
    }
    
    sdkbox::FBAPIParam param;
    sdkbox::PluginFacebook::requestInvitableFriends(param);
    
    //disable menu
    auto n = dynamic_cast<Menu*>(getChildByTag(TAG_MENU));
    if (nullptr != n) {
        n->setEnabled(false);
    }
}

void CFacebookAPITestPopup::createInviteDialog() {
    
    auto popupSize = this->getContentSize();
    auto width  = popupSize.width  * 4 / 5;
    auto height = popupSize.height * 3 / 5;
    
    m_InviteDialog = new Node();
    m_InviteDialog->setPosition(popupSize.width/2, popupSize.height/2);
    
    //title bg
    auto sp = Sprite::create("ui/blue_button03.png");
    auto size = sp->getContentSize();
    sp->setScaleX(width/size.width);
    sp->setScaleY(1.5);
    sp->setPosition(0, height/2 + size.height * 1.5 / 2 - 15);
    m_InviteDialog->addChild(sp);
    
    //title text
    auto label = Label::createWithSystemFont("Invite Friends", "sans", 35);
    label->setPosition(0, height/2 + 20);
    m_InviteDialog->addChild(label);
    
    //bg
    auto bgU = Sprite::create("ui/dropdownTop.png");
    auto heightT = bgU->getContentSize().height;
    auto scaleX = width/bgU->getContentSize().width;
    bgU->setScaleX(scaleX);
    bgU->setPosition(0, height/2 - heightT/2);
    m_InviteDialog->addChild(bgU);
    
    auto bgB = Sprite::create("ui/dropdownBottom.png");
    auto heightB = bgB->getContentSize().height;
    bgB->setScaleX(scaleX);
    bgB->setPosition(0, -height/2 + heightB/2);
    m_InviteDialog->addChild(bgB);
    
    auto bgM = Sprite::create("ui/dropdownMid.png");
    auto heigtM = bgM->getContentSize().height;
    bgM->setScaleX(scaleX);
    auto pos = bgU->getPosition().y - heightT/2;
    while (true) {
        if (pos < -height/2 + heightB/2) {
            break;
        }
        auto clone = Sprite::createWithSpriteFrame(bgM->getSpriteFrame());
        clone->setScaleX(scaleX);
        clone->setPosition(0, pos - heigtM/2);
        m_InviteDialog->addChild(clone);
        pos -= heigtM;
    }
    
    int padding = 10;
    pos = height/2;
    
    // invite title
    pos -= padding * 2; //padding
    label = Label::createWithSystemFont("Title:", "sans", 20);
    label->setTextColor(Color4B(128, 128, 128, 255));
    label->setPosition(-width/2 + label->getContentSize().width/2 + padding * 2, pos);
    m_InviteDialog->addChild(label);
    pos -= label->getContentSize().height;
    
    pos -= padding;
    label = Label::createWithSystemFont(m_InviteTitle, "sans", 20);
    label->setTextColor(Color4B(64, 64, 64, 255));
    label->setPosition(-width/2 + label->getContentSize().width/2 + padding * 2, pos);
    m_InviteDialog->addChild(label);
    pos -= label->getContentSize().height;
    
    //    pos -= padding;
    //    sp = Sprite::create("ui/grey_sliderHorizontal.png");
    //    sp->setPosition(0, pos);
    //    sp->setScaleX((width - padding * 6)/sp->getContentSize().width);
    //    m_InviteDialog->addChild(sp);
    //    pos -= sp->getContentSize().height;
    
    // invite text
    pos -= padding;
    label = Label::createWithSystemFont("Text:", "sans", 20);
    label->setTextColor(Color4B(128, 128, 128, 255));
    label->setPosition(-width/2 + label->getContentSize().width/2 + padding * 2, pos);
    m_InviteDialog->addChild(label);
    pos -= label->getContentSize().height;
    
    pos -= padding;
    label = Label::createWithSystemFont(m_InviteContent, "sans", 20);
    label->setTextColor(Color4B(64, 64, 64, 255));
    label->setPosition(-width/2 + label->getContentSize().width/2 + padding * 2, pos);
    m_InviteDialog->addChild(label);
    pos -= label->getContentSize().height;
    
    pos -= padding;
    sp = Sprite::create("ui/grey_sliderHorizontal.png");
    sp->setPosition(0, pos);
    sp->setScaleX((width - padding * 6)/sp->getContentSize().width);
    m_InviteDialog->addChild(sp);
    pos -= sp->getContentSize().height;
    
    //invite friend
    pos -= padding;
    label = Label::createWithSystemFont("To:", "sans", 20);
    label->setTextColor(Color4B(128, 128, 128, 255));
    label->setPosition(-width/2 + label->getContentSize().width/2 + padding * 2, pos);
    m_InviteDialog->addChild(label);
    pos -= label->getContentSize().height;
    
    int buttonH = 50;
    // scrollview for friends
    pos -= padding;
    ui::ScrollView* sv = ui::ScrollView::create();
    Size svSize = Size(width - padding * 2, pos + height/2 - padding * 2 - buttonH);
    sv->setContentSize(svSize);
    sv->setPosition(Vec2(-width/2 + padding, -height/2 + padding + buttonH));
    m_InviteDialog->addChild(sv);
    sv->setTag(TAG_SCROLLVIEW);
    pos -= svSize.height;
    
    //cancel ok button
    auto button = cocos2d::ui::Button::create("ui/red_button04.png", "ui/red_button05.png");
    button->setPosition(Vec2(- width/4, pos - buttonH/2));
    button->setScale9Enabled(true);
    button->setTitleText("Cancel");
    button->setTitleFontSize(50);
    auto btnSize = button->getContentSize();
    btnSize.width = width/4;
    button->setContentSize(btnSize);
    button->addClickEventListener([=](Ref* sneder){
        m_InviteDialog->setVisible(false);
        auto n = dynamic_cast<Menu*>(getChildByTag(TAG_MENU));
        if (nullptr != n) {
            n->setEnabled(true);
        }
    });
    m_InviteDialog->addChild(button);
    
    button = cocos2d::ui::Button::create("ui/green_button07.png", "ui/green_button08.png");
    button->setPosition(Vec2(width/4, pos - buttonH/2));
    button->setScale9Enabled(true);
    button->setTitleText("Invite");
    button->setTitleFontSize(50);
    btnSize = button->getContentSize();
    btnSize.width = width/4;
    button->setContentSize(btnSize);
    button->addClickEventListener([=](Ref* sneder){
        if (m_InviteUserList.size() < 1) {
            CCLOG("Must select one friend at least");
            return;
        }
        std::vector<std::string> invite_ids;
        for(auto i : m_InviteUserList) {
            invite_ids.push_back(m_InviteableUsers[i].getUserId());
        }
        sdkbox::PluginFacebook::inviteFriendsWithInviteIds(invite_ids,
                                                           m_InviteTitle,
                                                           m_InviteContent);
    });
    m_InviteDialog->addChild(button);
    
    //add friends to srollview
    int column = 3;
    int x = 0, y = 0;
    int i = 0;
    pos = 0;
    int userHeight = 60;
    Size innerSize = svSize;
    innerSize.height = m_InviteableUsers.size()/column;
    if (0 != m_InviteableUsers.size()%column) {
        innerSize.height += 1;
    }
    innerSize.height *= userHeight;
    sv->setInnerContainerSize(innerSize);
    for (auto user : m_InviteableUsers) {
        x = i % column;
        y = i / column;
        
        Node* n = new Node();
        n->setPosition(x * innerSize.width/column, innerSize.height - userHeight * (y + 0.5));
        sv->addChild(n);
        int posX = 0;
        
        cocos2d::ui::CheckBox* cb = cocos2d::ui::CheckBox::create("ui/green_button06.png", "ui/green_checkmark.png");
        posX += cb->getContentSize().width/2;
        cb->setPosition(Vec2(posX, 0));
        n->addChild(cb);
        posX += cb->getContentSize().width/2;
        cb->setTag(TAG_CHECKBOX + i);
        cb->addEventListener(std::bind(&CFacebookAPITestPopup::onCheckBoxState, this, std::placeholders::_1, std::placeholders::_2));
        
        //head image
        sp = Sprite::create("ui/red_circle.png");
        sp->setPosition(posX + sp->getContentSize().width/2, 0);
        posX += sp->getContentSize().width;
        n->addChild(sp);
        
        //user name
        label = Label::createWithSystemFont(user.getName(), "sans", 20);
        label->setTextColor(Color4B(16, 16, 16, 255));
        label->setPosition(posX + label->getContentSize().width/2, 0);
        n->addChild(label);
        
        i++;
    }
}

void CFacebookAPITestPopup::onCheckBoxState(Ref* sender, cocos2d::ui::CheckBox::EventType event){
    auto cb = dynamic_cast<cocos2d::ui::CheckBox*>(sender);
    
    int idx = cb->getTag() - TAG_CHECKBOX;
    if (cocos2d::ui::CheckBox::EventType::SELECTED == event) {
        bool bExist = false;
        for (auto i : m_InviteUserList) {
            if (i == idx) {
                bExist = true;
            }
        }
        if (!bExist) {
            m_InviteUserList.push_back(idx);
        }
    } else if (cocos2d::ui::CheckBox::EventType::UNSELECTED == event) {
        int pos = 0;
        for (auto i : m_InviteUserList) {
            if (i == idx) {
                m_InviteUserList.erase(m_InviteUserList.begin() + pos);
            }
            pos++;
        }
    }
}

void CFacebookAPITestPopup::showInviteableFriends(){
    cocos2d::ui::ScrollView* sv = dynamic_cast<cocos2d::ui::ScrollView*>(m_InviteDialog->getChildByTag(TAG_SCROLLVIEW));
    if (nullptr == sv) {
        return;
    }
    sv->removeAllChildren();
    
    int column = 2;
    int x = 0, y = 0;
    int i = 0;
    int userHeight = 60;

    cocos2d::Label* label = nullptr;
    Size innerSize = sv->getContentSize();
    innerSize.height = m_InviteableUsers.size()/column;
    if (0 != m_InviteableUsers.size()%column) {
        innerSize.height += 1;
    }
    innerSize.height *= userHeight;
    if (innerSize.height < sv->getContentSize().height) {
        innerSize.height = sv->getContentSize().height;
    }
    sv->setInnerContainerSize(innerSize);
    for (auto user : m_InviteableUsers) {
        x = i % column;
        y = i / column;
        
        Node* n = new Node();
        n->setPosition(x * innerSize.width/column, innerSize.height - userHeight * (y + 0.5));
        sv->addChild(n);
        int posX = 0;
        
        cocos2d::ui::CheckBox* cb = cocos2d::ui::CheckBox::create("ui/green_button06.png", "ui/green_checkmark.png");
        posX += cb->getContentSize().width/2;
        cb->setPosition(Vec2(posX, 0));
        n->addChild(cb);
        posX += cb->getContentSize().width/2;
        cb->setTag(TAG_CHECKBOX + i);
        cb->addEventListener(std::bind(&CFacebookAPITestPopup::onCheckBoxState, this, std::placeholders::_1, std::placeholders::_2));
        
        //head image
        CUrlSprite::create()
        ->setUrl(user.getPictureURL())
        ->setSize(Size(50, 50))
        ->build(n)
        ->setPosition(Vec2(posX + 50/2, 0));
        
        posX += 50;
        //CCLOG("user head image: %s", user.getPictureURL().c_str());
        
        //user name
        label = Label::createWithSystemFont(user.getName(), "sans", 20);
        label->setTextColor(Color4B(16, 16, 16, 255));
        label->setPosition(posX + label->getContentSize().width/2, 0);
        n->addChild(label);
        
        i++;
    }
}
