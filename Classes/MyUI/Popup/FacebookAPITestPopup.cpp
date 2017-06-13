#include "FacebookAPITestPopup.hpp"
#include "../MyButton.h"
#include "../UrlSprite.hpp"
#include "../../Scene/GameScene.h"
#include "../../SDKUtil/SDKUtil.h"
#include "../../GameObject/ObjectManager.h"
#include "../../Facebook/FacebookManager.hpp"
#include "../../json/json.h"
#include "ui/UIScrollView.h"
#include "ui/UIListView.h"
#include <array>

using namespace cocos2d;
using namespace cocos2d::ui;

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
    
    std::array<std::string, 17> titleArray = {
        sdkbox::PluginFacebook::isLoggedIn() ? "Logout" : "Login",
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
        "RankUpDialog"
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
    auto RankUpDialog       = createBtn([=](Node* s){ this->RankUpDialog(s);             }, titleArray[16] );

    struct buttonInfo {
        Node* _btn;
        Vec2  _pos;
        buttonInfo(Node* btn, Vec2 pos)
        : _btn(btn)
        , _pos(pos){};
    };
    
    std::array<buttonInfo, 17> startPos = {
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
        buttonInfo(RankUpDialog     , Vec2(popupSize.width * 0.5f, popupSize.height * -0.8f)),
    };
    
    std::array<buttonInfo, 17> targetPos = {
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
        buttonInfo(SetMyScore       , Vec2(popupSize.width * 0.5f, popupSize.height * 0.15f)),
        buttonInfo(RankUpDialog     , Vec2(popupSize.width * 0.5f, popupSize.height * 0.1f))
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
    
    
    CFacebookManager::Instance()->setLoginListener([=](){
        Login->changeContents("Logout");
        
        CFacebookManager::Instance()->ClearData();
        CFacebookManager::RequestFriendList();
        CFacebookManager::RequestMyInfo();
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
    }, 1.3f);
    
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
    CFacebookManager::RequestMyInfo();
}

void CFacebookAPITestPopup::GetMyFriends(cocos2d::Node* sender){
    CCLOG("##FB %s", __FUNCTION__);
    CFacebookManager::RequestFriendList();
}

void CFacebookAPITestPopup::CaptureScreen(cocos2d::Node* sender){
    CCLOG("##FB %s", __FUNCTION__);
    
    utils::captureScreen([=](bool yes, const std::string &outputFilename){
        m_FacebookCapture = outputFilename;
    }, "FBCapture.png");
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
    CFacebookManager::SaveScore(0);
}

void CFacebookAPITestPopup::RankUpDialog(cocos2d::Node* sender){
    CGameScene::getGameScene()->OpenRankUpPopup();
}

void CFacebookAPITestPopup::showInviteDialog(){
    this->createInviteDialog();
    
    sdkbox::FBAPIParam param;
    sdkbox::PluginFacebook::requestInvitableFriends(param);
}

void CFacebookAPITestPopup::createInviteDialog() {
    
    auto invitePopup = CPopup::create()
    ->setPopupPosition(this->getContentSize() / 2)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(this);
    
    // create bg
    auto bg = LayerColor::create(COLOR::DARKGRAY_ALPHA, 1080, 1500);
    bg->ignoreAnchorPointForPosition(false);
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bg->setPosition(invitePopup->getContentSize() / 2);
    invitePopup->addChild(bg);
    
    // create title
    {
        auto title = Label::createWithTTF("Invite Friends", FONT::MALGUNBD, 50);
        title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        title->setPosition(Vec2(bg->getContentSize().width * 0.5f,
                                bg->getContentSize().height * 0.95f));
        bg->addChild(title);
    }
    
    // create content
    {
        auto content = Label::createWithTTF("Come Come", FONT::MALGUNBD, 50);
        content->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        content->setPosition(Vec2(bg->getContentSize().width * 0.5f,
                                bg->getContentSize().height * 0.85f));
        bg->addChild(content);
    }
    
    // create friends list
    {
        auto createDP = [=](sdkbox::FBGraphUser user, int index){
            auto dpBack = Widget::create();
            dpBack->setContentSize(Size(900.f, 100.f));
            dpBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            
            auto contentBG = LayerColor::create(COLOR::BRIGHTGRAY_ALPHA, 790.f, 100);
            contentBG->setIgnoreAnchorPointForPosition(false);
            contentBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            contentBG->setPosition(Vec2(0 + contentBG->getContentSize().width * 0.5f, 0));
            dpBack->addChild(contentBG);
            
            auto checkbox = cocos2d::ui::CheckBox::create("ui/green_button06.png",
                                                          "ui/green_checkmark.png");
            checkbox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            checkbox->setPosition(Vec2(dpBack->getContentSize().width -
                                       (checkbox->getContentSize().width * 0.5f),
                                       dpBack->getContentSize().height * 0.5f));
            checkbox->addEventListener(std::bind(&CFacebookAPITestPopup::onCheckBoxState, this, std::placeholders::_1, std::placeholders::_2));
            checkbox->setTag(index);
            dpBack->addChild(checkbox);
            
            auto name = Label::createWithTTF(user.name, FONT::MALGUNBD, 50);
            name->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            name->setPosition(Vec2(contentBG->getContentSize().width * 0.4f,
                                   contentBG->getContentSize().height * 0.9f));
            contentBG->addChild(name);
            
            CUrlSprite::create()
            ->setUrl(user.getPictureURL())
            ->setSize(Size(contentBG->getContentSize() * 0.35f))
            ->build(contentBG)
            ->setPosition(Vec2(contentBG->getContentSize().width * 0.175f,
                               contentBG->getContentSize().height * 0.5f));
            
            return dpBack;
        };
        
        Size layerSize = bg->getContentSize();
        Size dpSize = Size(900, 100);
        size_t dpDistance = 15;
        float spawnCount = 8;
        
        // Create the list view
        auto listView = cocos2d::ui::ListView::create();
        if (listView != nullptr){
            listView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
            listView->setBounceEnabled(true);
            listView->setBackGroundImageScale9Enabled(true);
            listView->setContentSize(Size(bg->getContentSize().width,
                                          (dpSize.height + dpDistance) * spawnCount));
            listView->setScrollBarPositionFromCorner(Vec2(7, 7));
            listView->setItemsMargin(dpDistance);
            listView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            listView->setPosition(layerSize / 2);
            listView->setMagneticType(ListView::MagneticType::CENTER);
            bg->addChild(listView);
            
            unsigned dpIdx = 0;
            for (auto user : m_InviteableUsers)
                listView->pushBackCustomItem(createDP(user, dpIdx++));
            
        }
    }
    
    // create button
    {
        auto createButton = [=](const std::function<void(Node*)> &callback,
                                Color4B btnColor, std::string name) {
            
            return CMyButton::create()
            ->addEventListener(callback)
            ->setButtonSingleUse(true)
            ->setLayer(LayerColor::create(btnColor, 430, 150))
            ->setContents(name)
            ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
            ->show(bg);
        };
        
        // cancel
        createButton([=](Node* sender){
            // popup close
            invitePopup->popupClose();
        }, COLOR::DARKGRAY_ALPHA, "Cancel")
        ->setPosition(Vec2(bg->getContentSize().width * 0.25f,
                           bg->getContentSize().height * 0.15f));
        
        // invite
        createButton([=](Node* sender){
            if (m_InviteUserList.size() < 1) {
                CCLOG("Must select one friend at least");
                return;
            }
            std::vector<std::string> invite_ids;
            for(auto i : m_InviteUserList)
                invite_ids.push_back(m_InviteableUsers[i].getUserId());
            
            // invite selected friends
            sdkbox::PluginFacebook::inviteFriendsWithInviteIds(invite_ids,
                                                               m_InviteTitle,
                                                               m_InviteContent);
            // popup close
            invitePopup->popupClose();
        }, COLOR::BRIGHTRED_ALPHA, "Invite")
        ->setPosition(Vec2(bg->getContentSize().width * 0.75f,
                           bg->getContentSize().height * 0.15f));
    }
}

void CFacebookAPITestPopup::onCheckBoxState(Ref* sender, cocos2d::ui::CheckBox::EventType event){
    auto cb = dynamic_cast<cocos2d::ui::CheckBox*>(sender);
    
    int idx = cb->getTag();
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