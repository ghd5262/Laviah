#include "WorkshopPopupDP.h"
#include "../MyButton.h"
#include "../LevelProgressBar.h"
#include "../Popup.h"
#include "../../DataManager/UserDataManager.h"
#include "../../Scene/GameScene.h"

CWorkshopPopupDP* CWorkshopPopupDP::create(sWORKSHOPITEM_PARAM workshopItem)
{
	CWorkshopPopupDP *pRet = new(std::nothrow) CWorkshopPopupDP(workshopItem);
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

bool CWorkshopPopupDP::init()
{
    if (!Widget::init()) return false;
    
    unsigned currentLevel = CUserDataManager::Instance()->getUserData_Number(m_WorkshopItem._userDataKey);
    
    auto dpBack = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080.f, 200.f);
    if (dpBack != nullptr){
        this->setContentSize(dpBack->getContentSize());
        
        dpBack->setIgnoreAnchorPointForPosition(false);
        dpBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        dpBack->setPosition(this->getContentSize() / 2);
        this->addChild(dpBack);
    }
    
    auto dpItemBack = LayerColor::create(COLOR::BRIGHTGRAY_ALPHA, 805.f, 200.f);
    if (dpItemBack != nullptr){
        dpItemBack->setIgnoreAnchorPointForPosition(false);
        dpItemBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        dpItemBack->setPosition(Vec2(0 + dpItemBack->getContentSize().width * 0.5f, 0));
        dpBack->addChild(dpItemBack);
    }
    
    m_BtnBuy = CMyButton::create()
    ->addEventListener([=](Node* sender){
        CPopup::create()
        ->setPositiveButton([=](Node* sender){
                    this->Buy(sender);
        }, TRANSLATE("BUTTON_YES"))
        ->setNegativeButton([=](Node* sender){
        }, TRANSLATE("BUTTON_NO"))
        ->setDefaultAnimation(ePOPUP_ANIMATION::OPEN_CENTER, ePOPUP_ANIMATION::CLOSE_CENTER)
        ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
        ->setMessage(TRANSLATE("CHARACTER_BUY_CHECK"))
        ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
		->setPopupPosition(CGameScene::getGameScene()->getContentSize() / 2)
        ->show(CGameScene::getGameScene(), ZORDER::POPUP);
    })
    ->setLayer(LayerColor::create(COLOR::DARKGRAY_ALPHA, 260, 200))
    ->setContents(TRANSLATE("WORKSHOP_BUTTON_BUY"))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(dpBack);
    
    m_BtnBuy->setPosition(Vec2(dpBack->getContentSize().width - (m_BtnBuy->getContentSize().width * 0.5f), 0));
    m_BtnBuy->setSwallowTouches(false);
    
    if (currentLevel >= m_WorkshopItem._maxLevel){
        m_BtnBuy->changeContents(TRANSLATE("WORKSHOP_BUTTON_MAX_LEVEL"));
        m_BtnBuy->setTouchEnable(false);
    }
    else{
        m_BtnBuy->changeContents(MakeString(TRANSLATE("WORKSHOP_BUTTON_BUY_LEVEL").c_str(), m_WorkshopItem._costPerLevel.at(currentLevel)));
        m_BtnBuy->setTouchEnable(true);
    }
    
    
    auto workshopItemName = Label::createWithTTF(TRANSLATE(m_WorkshopItem._name), FONT::MALGUNBD, 40);
    if (workshopItemName != nullptr)
    {
        workshopItemName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        workshopItemName->setPosition(Vec2(dpItemBack->getContentSize().width * 0.25f, dpItemBack->getContentSize().height * 0.8f));
        workshopItemName->setColor(COLOR::BRIGHTGRAY);
        dpItemBack->addChild(workshopItemName);
    }
    
	auto workshopItemExplain = Label::createWithTTF(TRANSLATE(m_WorkshopItem._explain),
		"fonts/malgun.ttf",
		23,
		Size(dpItemBack->getContentSize().width * 0.7f, dpItemBack->getContentSize().height * 0.45f),
		TextHAlignment::LEFT,
		TextVAlignment::TOP);
    if (workshopItemExplain != nullptr)
    {
        workshopItemExplain->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        workshopItemExplain->setPosition(Vec2(dpItemBack->getContentSize().width * 0.25f, dpItemBack->getContentSize().height * 0.45f));
        workshopItemExplain->setColor(COLOR::BRIGHTGRAY);
        dpItemBack->addChild(workshopItemExplain);
    }
    
    m_LevelProgressBar = CLevelProgressBar::create(Size(dpItemBack->getContentSize().width * 0.7f, dpItemBack->getContentSize().height * 0.1f),
                                                   m_WorkshopItem._maxLevel, currentLevel);
    if (m_LevelProgressBar != nullptr)
    {
        m_LevelProgressBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        m_LevelProgressBar->setPosition(Vec2(dpItemBack->getContentSize().width * 0.6f, dpItemBack->getContentSize().height * 0.1f));
        dpItemBack->addChild(m_LevelProgressBar);
    }
    
    auto workshopItemImg = Sprite::create(m_WorkshopItem._textureName);
    if (workshopItemImg != nullptr)
    {
        workshopItemImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        workshopItemImg->setPosition(Vec2((dpItemBack->getContentSize().width * 0.075f) + (workshopItemImg->getContentSize().width * 0.5f), dpItemBack->getContentSize().height * 0.5f));
        dpItemBack->addChild(workshopItemImg);
    }
    
    return true;
}

void CWorkshopPopupDP::Buy(Node* sender)
{
	CCLOG("Buy Item %s", TRANSLATE(m_WorkshopItem._name).c_str());
    
    unsigned value = CUserDataManager::Instance()->getUserData_Number(m_WorkshopItem._userDataKey);
    if (CUserDataManager::Instance()->CoinUpdate(-m_WorkshopItem._costPerLevel.at(value))){
		value += 1;
        CUserDataManager::Instance()->setUserData_Number(m_WorkshopItem._userDataKey, value);
        
        // Update button ui
        if(value >= m_WorkshopItem._maxLevel){
            m_BtnBuy->changeContents(TRANSLATE("WORKSHOP_BUTTON_MAX_LEVEL"));
            m_BtnBuy->setTouchEnable(false);
            m_BtnBuy->changeFontColor(Color3B::BLACK);
        }
        else{
            m_BtnBuy->changeContents(MakeString(TRANSLATE("WORKSHOP_BUTTON_BUY_LEVEL").c_str(), m_WorkshopItem._costPerLevel.at(value)));
        }
        
        // Update level progress
        m_LevelProgressBar->setCurrentLevel(value);
        m_LevelProgressBar->UpdateProgress();
        
        // set current selected item idx
        CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::SELECT_ITEM, m_WorkshopItem._idx);
    }
}