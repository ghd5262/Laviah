#include "OptionDataSavePopup.hpp"
#include "../../MyButton.h"
#include "../../../Scene/GameScene.h"
#include "../../../DataManager/UserDataManager.h"

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
    auto content = TRANSLATE("OPTION_DATASAVE_CONTENT") +
    std::string("\n") +
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
    label->setColor(COLOR::DARKGRAY);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->setPosition(Vec2(layer->getContentSize().width * 0.5f,
                            layer->getContentSize().height * 0.65f));
    layer->addChild(label);
    
    auto saveBtn = CMyButton::create()
    ->addEventListener([=](Node* sender){
        CGameScene::getGameScene()->CreateAlertPopup()
        ->setPositiveButton([](Node* sender){
            CUserDataManager::Instance()->SaveUserData(true, true);
        }, TRANSLATE("BUTTON_YES"))
        ->setNegativeButton([](Node* sender){}, TRANSLATE("BUTTON_NO"))
        ->setMessage(TRANSLATE("OPTION_DATASAVE_SAVE_CHECK"))
        ->show(CGameScene::getGameScene(), ZORDER::POPUP);
    })
    ->setLayer(LayerColor::create(COLOR::DARKGRAY_ALPHA, 400, 150))
    ->setContents(TRANSLATE("OPTION_DATASAVE_SAVE"))
    ->setButtonPosition(Vec2(layer->getContentSize().width * 0.265f,
                             layer->getContentSize().height * 0.2f))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(layer);
    saveBtn->setSwallowTouches(false);
    
    auto resetBtn = CMyButton::create()
    ->addEventListener([=](Node* sender){
        CGameScene::getGameScene()->CreateAlertPopup()
        ->setPositiveButton([](Node* sender){
            CUserDataManager::Instance()->setUserData_Reset();
            CUserDataManager::Instance()->SaveUserData(true, true);
        }, TRANSLATE("BUTTON_YES"))
        ->setNegativeButton([](Node* sender){}, TRANSLATE("BUTTON_NO"))
        ->setMessage(TRANSLATE("OPTION_DATASAVE_RESET_CHECK"))
        ->show(CGameScene::getGameScene(), ZORDER::POPUP);
    })
    ->setLayer(LayerColor::create(COLOR::BRIGHTRED_ALPHA, 400, 150))
    ->setContents(TRANSLATE("OPTION_DATASAVE_RESET"))
    ->setButtonPosition(Vec2(layer->getContentSize().width * 0.735f,
                             layer->getContentSize().height * 0.2f))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(layer);
    resetBtn->setSwallowTouches(false);
    
    return true;
}
