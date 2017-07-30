#include "OptionDataSavePopup.hpp"
#include "../../MyButton.h"
#include "../../../Scene/GameScene.h"
#include "../../../DataManager/UserDataManager.h"
#include "../../../SDKBOX/SDKBox.h"
#include "../../../Download/DownloadManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;

COptionDataSavePopup* COptionDataSavePopup::create()
{
    COptionDataSavePopup *pRet = new(std::nothrow) COptionDataSavePopup();
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

bool COptionDataSavePopup::init()
{
    if (!CPopup::init()) return false;
    
    this->setContentSize(Size(1080.f, 1500.f));
    auto layer = LayerColor::create(COLOR::BRIGHTGRAY_ALPHA,
                                    getContentSize().width * 0.9f,
                                    getContentSize().height * 0.5f);
    layer->setIgnoreAnchorPointForPosition(false);
    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    layer->setPosition(this->getContentSize() / 2);
    this->addChild(layer);
    
    auto savedTime = CUserDataManager::Instance()->getLastSavedTime();
    auto content   = TRANSLATE("OPTION_DATASAVE_CONTENT") + std::string("\n") +
    StringUtils::format(TRANSLATE("OPTION_DATASAVE_LAST_SAVED_TIME").c_str(),
                        savedTime->tm_year + 1900,
                        savedTime->tm_mon + 1,
                        savedTime->tm_mday,
                        savedTime->tm_hour,
                        savedTime->tm_min,
                        savedTime->tm_sec);
    
    auto label = Label::createWithSystemFont(content, FONT::MALGUNBD, 40,
                                      Size(layer->getContentSize().width * 0.85f,
                                           layer->getContentSize().height),
                                      TextHAlignment::CENTER,
                                      TextVAlignment::CENTER);
//    label->setColor(COLOR::DARKGRAY);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->setPosition(Vec2(layer->getContentSize().width * 0.5f,
                            layer->getContentSize().height * 0.65f));
    layer->addChild(label);
    
    auto alertCreate = [=](std::function<void(Node*)> positive, std::string content)
    {
        return CGameScene::getGameScene()->CreateAlertPopup()
        ->setPositiveButton(positive, TRANSLATE("BUTTON_YES"))
        ->setNegativeButton([](Node* sender){}, TRANSLATE("BUTTON_NO"))
        ->setMessage(content)
        ->show(CGameScene::getPopupLayer(), ZORDER::POPUP);
    };
    
    auto buttonCreate = [=](std::function<void(Node*)> callback, std::string content, Vec2 pos, Color4B color)
    {
        auto btn = CMyButton::create()
        ->addEventListener(callback)
        ->setLayer(LayerColor::create(color, 400, 150))
        ->setContents(content)
        ->setButtonPosition(pos)
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->show(layer);
        btn->setSwallowTouches(false);
        
        return btn;
    };
    
    auto loginFirstPopup = [=](std::function<void()> afterLogin){
        if(CPlayManager::Instance()->IsLoggedIn())
            afterLogin();
        else{
            alertCreate([=](Node* sender){
                CPlayManager::Instance()->Login([=](){ afterLogin(); });
            }, "로그인이 필요합니다.");
        }
    };
    
    auto networkPopup = [=](){
        alertCreate([=](Node* sender){}, "네트워크 연결을 확인해주세요.");
    };
    
    buttonCreate([=](Node* sender){
        alertCreate([=](Node* sender){
            CDownloadManager::IsNetworkConnected([=](bool isConnected){
                if(isConnected){
                    loginFirstPopup([=](){
                        CUserDataManager::Instance()->SaveUserData(true);
                    });
                }
                else networkPopup();
            });
            
        }, TRANSLATE("OPTION_DATASAVE_SAVE_CHECK"));
    }, TRANSLATE("OPTION_DATASAVE_SAVE"), Vec2(layer->getContentSize().width * 0.265f, layer->getContentSize().height * 0.2f), COLOR::BRIGHTRED_ALPHA);
    
    buttonCreate([=](Node* sender){
        alertCreate([=](Node* sender){
            CDownloadManager::IsNetworkConnected([=](bool isConnected){
                if(isConnected){
                    loginFirstPopup([=](){
                        CUserDataManager::Instance()->LoadUserDataFromCloud();
                    });
                }
                else networkPopup();
            });
        }, "서버에 저장된 데이터를 불러오시겠습니까? \n기존의 기기에 저장되었던 데이터는 삭제됩니다.");
    }, "불러오기", Vec2(layer->getContentSize().width * 0.735f, layer->getContentSize().height * 0.2f), COLOR::DARKGRAY_ALPHA);
    
    
    // create sign button
    {
        auto buttonUpdate = [=](CMyButton* btn){
            std::string imageName = "signButtonGameCenter";
#if(CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
            imageName = "signButtonGoogle";
#endif
            imageName += StringUtils::format("_%d.png", int(CPlayManager::Instance()->IsLoggedIn()));
            btn->changeButtonImage(imageName);
        };
        
        CMyButton* signButton = CMyButton::create()
        ->addEventListener([=](Node* sender){
            auto btn      = dynamic_cast<CMyButton*>(sender);
            auto loggedIn = CPlayManager::Instance()->IsLoggedIn();
            
            if(loggedIn)    CPlayManager::Instance()->Logout([=](){ buttonUpdate(btn); });
            else            CPlayManager::Instance()->Login([=](){ buttonUpdate(btn); });
        })
        ->setButtonNormalImage("signButtonGameCenter_0.png")
        ->setButtonPosition(Vec2(layer->getContentSize().width * 0.5f, 0))
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP)
        ->show(layer);
        signButton->setSwallowTouches(false);
        buttonUpdate(signButton);
        
        CMyButton::create()
        ->addEventListener([=](Node* sender){
            alertCreate([](Node* sender){
                CUserDataManager::Instance()->setUserData_Reset();
                CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::COIN, 1000000);
                CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::LEVEL, 10);
                
                CUserDataManager::Instance()->setUserData_ItemExist(USERDATA_KEY::COSTUME_LIST, 1);
                CUserDataManager::Instance()->setUserData_ItemExist(USERDATA_KEY::COSTUME_LIST, 5);

            }, TRANSLATE("OPTION_DATASAVE_RESET_CHECK"));
        })
        ->setButtonNormalImage("endIcon.png")
        ->setButtonPosition(Vec2(layer->getContentSize().width, 0))
        ->setButtonAnchorPoint(Vec2::ANCHOR_TOP_RIGHT)
        ->show(layer)
        ->setSwallowTouches(false);
    }
    
    // create auto save button
    {
        auto userDataMng  = CUserDataManager::Instance();
        auto buttonUpdate = [=](CMyButton* btn){
            auto imageName = StringUtils::format("autoSaveButton_%d.png",
                                                 int(userDataMng->getUserData_Number(USERDATA_KEY::DATA_SAVE_AUTO)));
            btn->changeButtonImage(imageName);
        };
        
        auto label = Label::createWithSystemFont("Auto Save", FONT::MALGUNBD, 40);
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        label->setPosition(Vec2(layer->getContentSize().width * 0.5f,
                                layer->getContentSize().height * 0.4f));
        layer->addChild(label);
        
        CMyButton* autoSaveButton = CMyButton::create()
        ->addEventListener([=](Node* sender){
            auto origin = (bool)userDataMng->getUserData_Number(USERDATA_KEY::DATA_SAVE_AUTO);
            auto btn    = dynamic_cast<CMyButton*>(sender);
            if(!origin){
                
                CGameScene::getGameScene()->CreateAlertPopup()
                ->setPositiveButton([=](Node* sender){
                    userDataMng->setUserData_Number(USERDATA_KEY::DATA_SAVE_AUTO, !origin);
                    buttonUpdate(btn);
                }, TRANSLATE("BUTTON_YES"))
                ->setNegativeButton([](Node* sender){}, TRANSLATE("BUTTON_NO"))
                ->setMessage("자동 저장 옵션을 켜면 네트워크가 연결되어 있는 동안 클라우드에 항상 저장됩니다.")
                ->show(CGameScene::getPopupLayer(), ZORDER::POPUP);
            }
            else{
                userDataMng->setUserData_Number(USERDATA_KEY::DATA_SAVE_AUTO, !origin);
                buttonUpdate(dynamic_cast<CMyButton*>(btn));
            }
        })
        ->setButtonNormalImage("autoSaveButton_0.png")
        ->setButtonPosition(Vec2(layer->getContentSize().width * 0.5f
                                 - (label->getContentSize().width * 0.5f + 40),
                                 layer->getContentSize().height * 0.4f))
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->show(layer);
        autoSaveButton->setSwallowTouches(false);
        buttonUpdate(autoSaveButton);
    }
    return true;
}
