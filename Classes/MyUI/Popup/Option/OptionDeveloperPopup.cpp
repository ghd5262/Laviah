#include "OptionDeveloperPopup.hpp"
#include "../CouponPopup.hpp"
#include "../../MyButton.h"
#include "../../../Scene/GameScene.h"
#include "../../../DataManager/UserDataManager.h"
#include "../../../SDKBOX/SDKBoxHeaders.h"
#include "../../../Download/DownloadManager.h"
#include "ui/UICheckBox.h"
#include <array>

using namespace cocos2d;
using namespace cocos2d::ui;

COptionDeveloperPopup* COptionDeveloperPopup::create()
{
    COptionDeveloperPopup *pRet = new(std::nothrow) COptionDeveloperPopup();
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

bool COptionDeveloperPopup::init()
{
    if (!CPopup::init()) return false;
    
    this->setContentSize(Size(1080.f, 1500.f));
    auto popupSize = this->getContentSize();

    auto layer = LayerColor::create(COLOR::BRIGHTGRAY_ALPHA,
                                    getContentSize().width * 0.9f,
                                    getContentSize().height * 0.2f);
    layer->setIgnoreAnchorPointForPosition(false);
    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    layer->setPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.45f));
    this->addChild(layer);
    
    struct MEMBER {
        std::string _name;
        std::string _job;
        
        MEMBER(std::string name, std::string job)
        : _name(name)
        , _job(job){}
    };

    std::array<MEMBER, 2> members = {
        MEMBER(TRANSLATE("OPTION_DEVELOPER_0"), TRANSLATE("OPTION_JOB_0")),
        MEMBER(TRANSLATE("OPTION_DEVELOPER_1"), TRANSLATE("OPTION_JOB_1"))
    };
    
    Size dpSize = Size(popupSize.width * 0.9f, popupSize.height * 0.08f);
    float margin = 20.f;
    
    int index = 0;
    for(auto member : members)
    {
        auto posX = popupSize.width * 0.5f;
        auto posY = popupSize.height * 0.7f - (index * (dpSize.height + margin));
        this->createDeveloperDP(member._name, member._job, Vec2(posX, posY));
        index++;
    }
    
    auto createLabel = [=](std::string text){
        auto label = Label::createWithTTF(text, FONT::MALGUN, 40);
        layer->addChild(label);
        
        return label;
    };
    

    auto gaLogSend = [=](std::string action, bool state){
        CGoogleAnalyticsManager::LogEventValue(GA_CATEGORY::OPTION, action, (int)state);
    };
    
    auto createOnOff = [=](std::function<void(Ref* sender, bool)> listener, bool state){
        auto checkbox = cocos2d::ui::CheckBox::create("onOffIcon_0.png", "onOffIcon_1.png");
        checkbox->setSelected(state);
        checkbox->addEventListener([=](Ref* sender, cocos2d::ui::CheckBox::EventType event){
            listener(sender, event == ui::CheckBox::EventType::SELECTED);
        });
        layer->addChild(checkbox);
        
        return checkbox;
    };
    
    auto getLoginOriginValue = [=](){
        return CPlayManager::Instance()->IsLoggedIn();
    };
    auto loginListener = [=](Ref* sender, bool isEnabled){
        auto btn = dynamic_cast<ui::CheckBox*>(sender);
        if(!btn) return;
        
        if(isEnabled){
            // login
            CPlayManager::Instance()->Login([=](){
                btn->setSelected(true);
                gaLogSend(GA_ACTION::OPTION_LOGIN, true);
            });
        }
        else {
            CGameScene::getGameScene()->CreateAlertPopup()
            ->setPositiveButton([=](Node* sender){
                // logout
                CPlayManager::Instance()->Logout([=](){
                    btn->setSelected(false);
                    gaLogSend(GA_ACTION::OPTION_LOGIN, false);
                });
            }, TRANSLATE("BUTTON_YES"))
            ->setNegativeButton([=](Node* sender){
                btn->setSelected(true);
            }, TRANSLATE("BUTTON_NO"))
            ->setMessage(TRANSLATE("OPTION_ALERT_LOGOUT"))
            ->show(CGameScene::getPopupLayer(), ZORDER::POPUP);
        }
    };
    
    
    auto userDataMng = CUserDataManager::Instance();
    auto getAutoSaveOriginValue = [=](){
        return userDataMng->getUserData_Number(USERDATA_KEY::DATA_SAVE_AUTO);
    };
    auto autoSaveListener = [=](Ref* sender, bool isEnabled){
        auto btn = dynamic_cast<ui::CheckBox*>(sender);
        if(!btn) return;
        
        if(isEnabled){
            CGameScene::getGameScene()->CreateAlertPopup()
            ->setPositiveButton([=](Node* sender){
                userDataMng->setUserData_Number(USERDATA_KEY::DATA_SAVE_AUTO, true);
                btn->setSelected(true);
                gaLogSend(GA_ACTION::OPTION_AUTO_SAVE, true);
            }, TRANSLATE("BUTTON_YES"))
            ->setNegativeButton([=](Node* sender){
                btn->setSelected(false);
            }, TRANSLATE("BUTTON_NO"))
            ->setMessage(TRANSLATE("OPTION_ALERT_AUTO_SAVE"))
            ->show(CGameScene::getPopupLayer(), ZORDER::POPUP);
        }
        else{
            userDataMng->setUserData_Number(USERDATA_KEY::DATA_SAVE_AUTO, false);
            btn->setSelected(false);
            gaLogSend(GA_ACTION::OPTION_AUTO_SAVE, false);
        }
    };
    
    auto layerSize = layer->getContentSize();

    
    std::array<Node*, 3> info = {
        createLabel(TRANSLATE("OPTION_LOGIN")),
        createLabel(TRANSLATE("OPTION_AUTO_SAVE")),
//        createLabel(TRANSLATE("OPTION_VERSION")),
        createLabel(TRANSLATE("OPTION_SUPPORT")),
    };
    
    std::array<Node*, 3> data = {
        createOnOff(loginListener,    getLoginOriginValue()),
        createOnOff(autoSaveListener, getAutoSaveOriginValue()),
//        createLabel(Application::getInstance()->getVersion()),
        createLabel(TRANSLATE("OPTION_EMAIL"))
    };
    
    std::array<float, 3> posY = {
//        layerSize.height * 0.8f,
        layerSize.height * 0.75f,
        layerSize.height * 0.5f,
        layerSize.height * 0.25f,
    };
    
    for(int index = 0; index < 3; index++)
    {
        info[index]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        info[index]->setPosition(Vec2(layerSize.width * 0.05, posY[index]));
        
        data[index]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        data[index]->setPosition(Vec2(layerSize.width * 0.95, posY[index]));
    }
    
    auto createButton = [=](std::function<void(Node*)> callback, std::string name, Vec2 pos){
        return CMyButton::create()
        ->addEventListener([=](Node* sender){
            callback(sender);
        })
        ->setLayer(LayerColor::create(COLOR::DARKGRAY_ALPHA,
                                      popupSize.width * 0.275f, popupSize.height * 0.08f))
        ->setContents(name)
        ->setButtonPosition(pos)
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->show(this);
    };
    
    std::array<std::string, 3> btnName = {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || DEBUGING)
        TRANSLATE("OPTION_COUPON"),
#else
        TRANSLATE("OPTION_INSTAGRAM"),
#endif
        TRANSLATE("OPTION_FACEBOOK"),
        TRANSLATE("OPTION_TWITTER"),
    };
    
    std::array<std::function<void(Node*)>, 3> btnFunc = {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID|| DEBUGING)
        [=](Node* sender){ this->coupon(); },
#else
        [=](Node* sender){ this->instagram(); },
#endif
        [=](Node* sender){ this->facebook(); },
        [=](Node* sender){ this->twitter(); },
    };
    
    
    auto btnSize     = Size(popupSize.width * 0.275f, popupSize.height * 0.08f);
    auto paddingSize = Size(popupSize.width * 0.036f, 20.f);
    
    for(int posX = 0; posX < 3; posX++)
    {
        auto positionX = (popupSize.width * 0.05f) + (btnSize.width * 0.5f) +
        (posX * (btnSize.width + paddingSize.width));
        createButton(btnFunc[posX], btnName[posX], Vec2(positionX, popupSize.height * 0.29f));
    }
    
    return true;
}

void COptionDeveloperPopup::createDeveloperDP(std::string name, std::string job, Vec2 pos)
{
    auto btn = CMyButton::create()
    ->addEventListener([=](Node* sender){
        
    })
    ->setLayer(LayerColor::create(COLOR::BRIGHTGRAY_ALPHA,
                                  getContentSize().width * 0.9f,
                                  getContentSize().height * 0.08f))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(pos)
    ->setTouchEnable(false, Color3B::WHITE)
    ->show(this);
    btn->setSwallowTouches(false);

    auto nameLabel = Label::createWithTTF(name, FONT::MALGUN, 40);
    nameLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    nameLabel->setPosition(Vec2(btn->getContentSize().width * 0.95f,
                                btn->getContentSize().height * 0.5f));
    btn->addChild(nameLabel);
    
    auto jobLabel = Label::createWithTTF(job, FONT::MALGUN, 40);
    jobLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    jobLabel->setPosition(Vec2(btn->getContentSize().width * 0.05f,
                               btn->getContentSize().height * 0.5f));
    btn->addChild(jobLabel);
}

void COptionDeveloperPopup::coupon(){
    CCLOG("Input the coupon");
    CCouponPopup::create()
    ->setDefaultAnimation(ePOPUP_ANIMATION::OPEN_CENTER, ePOPUP_ANIMATION::CLOSE_CENTER)
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(CGameScene::getGameScene()->getContentSize() / 2)
    ->show(CGameScene::getPopupLayer(), ZORDER::POPUP);
}

void COptionDeveloperPopup::instagram(){
    CCLOG("instagram");
    auto url = CDownloadManager::Instance()->getInstagramPageLink();
    Application::getInstance()->openURL(url);
}

void COptionDeveloperPopup::facebook(){
    CCLOG("facebook");
    auto url = CDownloadManager::Instance()->getFacebookPageLink();
    Application::getInstance()->openURL(url);
}

void COptionDeveloperPopup::twitter(){
    CCLOG("twitter");
    auto url = CDownloadManager::Instance()->getTwitterPageLink();
    Application::getInstance()->openURL(url);
}

