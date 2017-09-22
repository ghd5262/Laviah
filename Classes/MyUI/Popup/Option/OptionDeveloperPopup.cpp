#include "OptionDeveloperPopup.hpp"
#include "../../MyButton.h"
#include "../../../Scene/GameScene.h"
#include "../../../DataManager/UserDataManager.h"
#include "../../../SDKBOX/SDKBoxHeaders.h"
#include "ui/UICheckbox.h"
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
                                    getContentSize().height * 0.3f);
    layer->setIgnoreAnchorPointForPosition(false);
    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    layer->setPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.4f));
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

    
    
    
    
    
    
    
    
//    auto userDataMng  = CUserDataManager::Instance();
//    auto buttonUpdate = [=](CMyButton* btn, std::function<bool(void)> getOriginValue){
//        auto imageName = StringUtils::format("onOffIcon_%d.png", int(getOriginValue()));
//        btn->changeButtonImage(imageName);
//    };
//    
//    auto createOnOff = [=](std::function<bool(Node*, bool)> listener,
//                           std::function<bool(void)> getOriginValue){
//        
//        auto bg = Sprite::create("onOffIconBG.png");
//        bg->setOpacity(255 * 0.8f);
//        layer->addChild(bg);
//        
//        CMyButton* button = CMyButton::create()
//        ->addEventListener([=](Node* sender){
//            auto origin = getOriginValue();
//            if(listener(sender, origin))
//                bg->setColor(COLOR::BRIGHTRED);
//            else
//                bg->setColor(COLOR::DARKGRAY);
//        })
//        ->setButtonNormalImage("autoSaveButton_0.png")
//        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
//        ->setButtonPosition(bg->getContentSize() / 2)
//        ->show(bg);
//        
//        button->setSwallowTouches(false);
//        buttonUpdate(button, getOriginValue);
//        
//        return bg;
//    };
    
//    auto layerSize = layer->getContentSize();
//    auto getLoginOriginValue = [=](){
//        return CPlayManager::Instance()->IsLoggedIn();
//    };
//    auto loginListener = [=](Node* sender, bool isEnabled){
//        auto btn = dynamic_cast<CMyButton*>(sender);
//        if(!btn) return;
//        
//        if(isEnabled){
//            // login
//            CPlayManager::Instance()->Login([=](){
//                buttonUpdate(btn, getLoginOriginValue);
//            });
//        }
//        else {
//            CGameScene::getGameScene()->CreateAlertPopup()
//            ->setPositiveButton([=](Node* sender){
//                // logout
//                CPlayManager::Instance()->Logout([=](){
//                    buttonUpdate(btn, getLoginOriginValue);
//                });
//            }, TRANSLATE("BUTTON_YES"))
//            ->setNegativeButton([](Node* sender){}, TRANSLATE("BUTTON_NO"))
//            ->setMessage("정말로 로그아웃 하시겠습니까? 클라우드 저장 및 다른기능이 제대로 동작하지 않을 수 있습니다.")
//            ->show(CGameScene::getPopupLayer(), ZORDER::POPUP);
//        }
//    };
//    
//    auto getAutoSaveOriginValue = [=](){
//        return userDataMng->getUserData_Number(USERDATA_KEY::DATA_SAVE_AUTO);
//    };
//    auto autoSaveListener = [=](Node* sender, bool isEnabled){
//        auto btn = dynamic_cast<CMyButton*>(sender);
//        if(!btn) return;
//        
//        if(isEnabled){
//            userDataMng->setUserData_Number(USERDATA_KEY::DATA_SAVE_AUTO, false);
//            buttonUpdate(btn, getAutoSaveOriginValue);
//        }
//        else{
//            CGameScene::getGameScene()->CreateAlertPopup()
//            ->setPositiveButton([=](Node* sender){
//                userDataMng->setUserData_Number(USERDATA_KEY::DATA_SAVE_AUTO, true);
//                buttonUpdate(btn, getAutoSaveOriginValue);
//            }, TRANSLATE("BUTTON_YES"))
//            ->setNegativeButton([](Node* sender){}, TRANSLATE("BUTTON_NO"))
//            ->setMessage("자동 저장 옵션을 켜면 네트워크가 연결되어 있는 동안 클라우드에 항상 저장됩니다.")
//            ->show(CGameScene::getPopupLayer(), ZORDER::POPUP);
//        }
//    };
    
    
    auto layerSize = layer->getContentSize();

    
    std::array<Node*, 4> info = {
        createLabel(TRANSLATE("OPTION_LOGIN")),
        createLabel(TRANSLATE("OPTION_AUTO_SAVE")),
        createLabel(TRANSLATE("OPTION_VERSION")),
        createLabel(TRANSLATE("OPTION_SUPPORT")),
    };
    
    std::array<Node*, 4> data = {
        createOnOff(loginListener,    getLoginOriginValue()),
        createOnOff(autoSaveListener, getAutoSaveOriginValue()),
        createLabel(Application::getInstance()->getVersion()),
        createLabel(TRANSLATE("OPTION_EMAIL"))
    };
    
    std::array<float, 4> posY = {
        layerSize.height * 0.8f,
        layerSize.height * 0.6f,
        layerSize.height * 0.4f,
        layerSize.height * 0.2f,
    };
    
    for(int index = 0; index < 4; index++)
    {
        info[index]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        info[index]->setPosition(Vec2(layerSize.width * 0.05, posY[index]));
        
        data[index]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        data[index]->setPosition(Vec2(layerSize.width * 0.95, posY[index]));
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



