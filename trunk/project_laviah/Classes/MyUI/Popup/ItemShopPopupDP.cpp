#include "ItemShopPopupDP.hpp"
#include "../MyButton.h"
#include "../LevelProgressBar.h"
#include "../Popup.h"
#include "../../DataManager/UserDataManager.h"
#include "../../Scene/GameScene.h"
#include "../../GameObject/ObjectManager.h"
#include "../../SDKBOX/SDKBoxHeaders.h"

CItemShopPopupDP* CItemShopPopupDP::create(const WORKSHOPITEM_PARAM* workshopItem)
{
    CItemShopPopupDP *pRet = new(std::nothrow) CItemShopPopupDP(workshopItem);
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

bool CItemShopPopupDP::init()
{
    if (!Widget::init()) return false;
    
    //    auto currentLevel = CUserDataManager::Instance()->getUserData_Number(m_WorkshopItem->_userDataKey);
    auto currentLevel = CUserDataManager::Instance()->getUserData_ParamData(USERDATA_KEY::ITEM_LEVEL,
                                                                            m_WorkshopItem->_index,
                                                                            PARAM_WORKSHOP::ITEM_LEVEL,
                                                                            0);
    
    auto itemName     = TRANSLATE(m_WorkshopItem->_name);
    auto dpBack = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080.f, 200.f);
    if (dpBack != nullptr){
        this->setContentSize(dpBack->getContentSize());
        
        dpBack->setIgnoreAnchorPointForPosition(false);
        dpBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        dpBack->setPosition(this->getContentSize() / 2);
        this->addChild(dpBack);
    }
    
    auto dpItemBack = LayerColor::create(COLOR::BRIGHTGRAY_ALPHA, 805.f, 200.f);
    if (dpItemBack != nullptr){
        dpItemBack->setIgnoreAnchorPointForPosition(false);
        dpItemBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        dpItemBack->setPosition(Vec2(dpItemBack->getContentSize().width * 0.5f,
                                     dpItemBack->getContentSize().height * 0.5f));
        dpBack->addChild(dpItemBack);
    }
    
    m_BtnUse = CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->use();
    })
    ->setLayer(LayerColor::create(COLOR::DARKGRAY_ALPHA, 260, 200))
    ->setContents("사용")
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(dpBack);
    
    m_BtnUse->setPosition(Vec2(dpBack->getContentSize().width
                               - (m_BtnUse->getContentSize().width * 0.5f),
                               dpBack->getContentSize().height * 0.5f));
    m_BtnUse->setSwallowTouches(false);
    
    if (currentLevel >= m_WorkshopItem->_maxLevel){
        m_BtnUse->changeContents(TRANSLATE("WORKSHOP_BUTTON_MAX_LEVEL"));
        m_BtnUse->setTouchEnable(false);
    }
    else{
        m_BtnUse->changeContents(MakeString(TRANSLATE("WORKSHOP_BUTTON_BUY_LEVEL").c_str(),
                                            m_WorkshopItem->_costPerLevel.at(currentLevel)));
        m_BtnUse->setTouchEnable(true);
    }
    
    
    auto workshopItemName = Label::createWithSystemFont(itemName, FONT::MALGUNBD, 40);
    if (workshopItemName != nullptr)
    {
        workshopItemName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        workshopItemName->setPosition(Vec2(dpItemBack->getContentSize().width * 0.25f,
                                           dpItemBack->getContentSize().height * 0.8f));
        workshopItemName->setColor(COLOR::BRIGHTGRAY);
        dpItemBack->addChild(workshopItemName);
    }
    
    auto workshopItemExplain = Label::createWithSystemFont(TRANSLATE(m_WorkshopItem->_explain),
                                                           FONT::MALGUN,
                                                           30,
                                                           Size(dpItemBack->getContentSize().width * 0.7f, dpItemBack->getContentSize().height * 0.45f),
                                                           TextHAlignment::LEFT,
                                                           TextVAlignment::CENTER);
    if (workshopItemExplain != nullptr)
    {
        workshopItemExplain->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        workshopItemExplain->setPosition(Vec2(dpItemBack->getContentSize().width * 0.25f,
                                              dpItemBack->getContentSize().height * 0.5f));
        workshopItemExplain->setColor(COLOR::BRIGHTGRAY);
        dpItemBack->addChild(workshopItemExplain);
    }
    
//    m_LevelProgressBar = CLevelProgressBar::create(Size(dpItemBack->getContentSize().width * 0.7f,
//                                                        dpItemBack->getContentSize().height * 0.1f),
//                                                   m_WorkshopItem->_maxLevel, currentLevel);
//    if (m_LevelProgressBar != nullptr)
//    {
//        m_LevelProgressBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
//        m_LevelProgressBar->setPosition(Vec2(dpItemBack->getContentSize().width * 0.6f,
//                                             dpItemBack->getContentSize().height * 0.1f));
//        dpItemBack->addChild(m_LevelProgressBar);
//    }
    
    auto workshopItemImg = Sprite::create(m_WorkshopItem->_textureName);
    if (workshopItemImg != nullptr)
    {
        workshopItemImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        workshopItemImg->setPosition(Vec2(dpItemBack->getContentSize().width * 0.125f,
                                          dpItemBack->getContentSize().height * 0.5f));
        dpItemBack->addChild(workshopItemImg);
    }
    
    this->schedule([=](float delta){
        this->calculateRemainTime();
    }, 1.f, "RemainTime");
    
    return true;
}

void CItemShopPopupDP::calculateRemainTime()
{
    if(!m_BtnUse) return;
    
    auto startTime  = CGameScene::getGameScene()->getGameStartTime();
    auto curTime    = time_t(time(nullptr));
    auto playTime   = curTime - startTime;
    auto remainTime = CGameScene::getGameScene()->getDailyResetRemain();
    auto seconds    = remainTime - playTime;
    
    time_t sec_t = seconds;
    struct tm* time;
    time = gmtime(&sec_t);
    std::string result = "";
    
    result = StringUtils::format("%d : %d", time->tm_min, time->tm_sec);
    
    m_BtnUse->changeContents(result);
}

void CItemShopPopupDP::use()
{
    CCLOG("Buy Item %s", TRANSLATE(m_WorkshopItem->_name).c_str());
    
    //    unsigned value = CUserDataManager::Instance()->getUserData_Number(m_WorkshopItem->_userDataKey);
    
    auto value = CUserDataManager::Instance()->getUserData_ParamData(USERDATA_KEY::ITEM_LEVEL,
                                                                     m_WorkshopItem->_index,
                                                                     PARAM_WORKSHOP::ITEM_LEVEL,
                                                                     0);
    auto cost  = -m_WorkshopItem->_costPerLevel.at(value);
    if (CUserDataManager::Instance()->CoinUpdate(cost)){
        value += 1;
        CAudioManager::Instance()->PlayEffectSound("sounds/CoinUse.mp3", false);
        CGoogleAnalyticsManager::LogEventCoin(GA_ACTION::COIN_USE_UPGRADE, cost);
        
        auto gaLabel = StringUtils::format("%04d_%04d", m_WorkshopItem->_index, value);
        CGoogleAnalyticsManager::LogEventValue(GA_CATEGORY::GAME_PLAY, GA_ACTION::ITEM_UPGRADE, gaLabel);
        CGoogleAnalyticsManager::LogScreen(GA_SCREEN::WORKSHOP_BUY);
        
        CUserDataManager::Instance()->setUserData_ItemParam(USERDATA_KEY::ITEM_LEVEL,
                                                            m_WorkshopItem->_index,
                                                            PARAM_WORKSHOP::ITEM_LEVEL,
                                                            value);
        
        // Update button ui
        if(value >= m_WorkshopItem->_maxLevel){
            m_BtnUse->changeContents(TRANSLATE("WORKSHOP_BUTTON_MAX_LEVEL"));
            m_BtnUse->setTouchEnable(false);
            m_BtnUse->changeFontColor(Color3B::GRAY);
        }
        else{
            m_BtnUse->changeContents(MakeString(TRANSLATE("WORKSHOP_BUTTON_BUY_LEVEL").c_str(),
                                                m_WorkshopItem->_costPerLevel.at(value)));
        }
        
        // Update level progress
        m_LevelProgressBar->setCurrentLevel(value);
        m_LevelProgressBar->UpdateProgress();
        
        // set current selected item idx
        CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::SELECT_ITEM, m_WorkshopItem->_index);
        
        // set current player data by item level
        CObjectManager::Instance()->ChangeCharacter();
    }
}
