#include "WorkshopPopupDP.h"
#include "../MyButton.h"
#include "../LevelProgressBar.h"
#include "../../DataManager/UserDataManager.h"

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
    if (!initVariable())
        return false;
    return true;
}

bool CWorkshopPopupDP::initVariable()
{
    try{
        unsigned currentLevel = CUserDataManager::Instance()->getUserData_Number(m_WorkshopItem._userDataKey);
        
        m_DPBack = LayerColor::create(Color4B(0, 0, 0, 0), 1080.f, 200.f);
        if (m_DPBack != nullptr){
            this->setContentSize(m_DPBack->getContentSize());
            
            m_DPBack->setIgnoreAnchorPointForPosition(false);
            m_DPBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            m_DPBack->setPosition(Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.5f));
            this->addChild(m_DPBack);
        }
        
        auto dpItemBack = LayerColor::create(Color4B(0, 0, 0, 255 * 0.4f), 805.f, 200.f);
        if (dpItemBack != nullptr){
            dpItemBack->setIgnoreAnchorPointForPosition(false);
            dpItemBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            dpItemBack->setPosition(Vec2(0 + dpItemBack->getContentSize().width * 0.5f, 0));
            m_DPBack->addChild(dpItemBack);
        }
        
        m_BuyBtn = CMyButton::createWithLayerColor(Size(260, 200), Color4B(0, 0, 0, 255 * 0.8f)
			, " ", 40, g_labelColor2, eMYBUTTON_STATE::END, [=](Node* sender){this->Buy(sender); }, EFFECT_SIZEDOWN)->show(m_DPBack);
        if (m_BuyBtn != nullptr)
        {
            m_BuyBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_BuyBtn->setPosition(Vec2(m_DPBack->getContentSize().width - (m_BuyBtn->getContentSize().width * 0.5f), 0));
            
            if(currentLevel >= m_WorkshopItem._maxLevel){
                m_BuyBtn->changeContents("MAX");
                m_BuyBtn->setBtnUnable(true);
                m_BuyBtn->changeFontColor(Color3B::BLACK);
            }
            else{
                m_BuyBtn->changeContents(MakeString("%d\nBuy", m_WorkshopItem._costPerLevel.at(currentLevel)));
            }
        }
        
		auto workshopItemName = Label::createWithTTF(m_WorkshopItem._name.c_str(), "fonts/malgunbd.ttf", 40);
        if (workshopItemName != nullptr)
        {
            workshopItemName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            workshopItemName->setPosition(Vec2(dpItemBack->getContentSize().width * 0.25f, dpItemBack->getContentSize().height * 0.8f));
            workshopItemName->setColor(g_labelColor2);
            dpItemBack->addChild(workshopItemName);
        }
        
		auto workshopItemExplain = Label::createWithTTF(m_WorkshopItem._explain.c_str(), "fonts/malgun.ttf", 35);
		if (workshopItemExplain != nullptr)
		{
			workshopItemExplain->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			workshopItemExplain->setPosition(Vec2(dpItemBack->getContentSize().width * 0.25f, dpItemBack->getContentSize().height * 0.5f));
			workshopItemExplain->setColor(g_labelColor2);
			dpItemBack->addChild(workshopItemExplain);
		}

		m_LevelProgressBar = CLevelProgressBar::create(
			Size(dpItemBack->getContentSize().width * 0.7f, dpItemBack->getContentSize().height * 0.15f),
			m_WorkshopItem._maxLevel, currentLevel);
		if (m_LevelProgressBar != nullptr)
		{
			m_LevelProgressBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			m_LevelProgressBar->setPosition(Vec2(dpItemBack->getContentSize().width * 0.6f, dpItemBack->getContentSize().height * 0.1f));
			dpItemBack->addChild(m_LevelProgressBar);
		}
		
		auto workshopItemImg = Sprite::create(m_WorkshopItem._textureName);
        if(workshopItemImg != nullptr)
        {
            workshopItemImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			workshopItemImg->setPosition(Vec2((dpItemBack->getContentSize().width * 0.075f) + (workshopItemImg->getContentSize().width * 0.5f), dpItemBack->getContentSize().height * 0.5f));
            dpItemBack->addChild(workshopItemImg);
        }
    }
    catch (...){
        throw StringUtils::format("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
        return false;
    }
    return true;
}

void CWorkshopPopupDP::Buy(Node* sender)
{
	CCLOG("Buy Item %s", m_WorkshopItem._name.c_str());
    
    unsigned value = CUserDataManager::Instance()->getUserData_Number(m_WorkshopItem._userDataKey);
    if (CUserDataManager::Instance()->CoinUpdate(-m_WorkshopItem._costPerLevel.at(value))){
		value += 1;
        CUserDataManager::Instance()->setUserData_Number(m_WorkshopItem._userDataKey, value);
        
        // Update button ui
        if(value >= m_WorkshopItem._maxLevel){
            m_BuyBtn->changeContents("MAX");
            m_BuyBtn->setBtnUnable(true);
            m_BuyBtn->changeFontColor(Color3B::BLACK);
        }
        else{
            m_BuyBtn->changeContents(MakeString("%d\nBuy", m_WorkshopItem._costPerLevel.at(value)));
        }
        
        // Update level progress
        m_LevelProgressBar->setCurrentLevel(value);
        m_LevelProgressBar->UpdateProgress();
        
        // set current selected item idx
        CUserDataManager::Instance()->setUserData_Number("USER_CUR_SELECT_ITEM", m_WorkshopItem._idx);
    }
}