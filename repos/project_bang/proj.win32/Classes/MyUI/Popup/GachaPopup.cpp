#include "GachaPopup.h"
#include "../MyButton.h"
#include "../UserCoinButton.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/CharacterDataManager.h"
#include "../../DataManager/UserDataManager.h"
#include <vector>

const int g_coinToGacha = 1500;

CGachaPopup* CGachaPopup::create()
{
	CGachaPopup *pRet = new(std::nothrow) CGachaPopup();
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

bool CGachaPopup::init()
{
	if (!CPopup::init()) return false;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080.f, 1920.f);
	if (bg != nullptr){
		bg->setIgnoreAnchorPointForPosition(false);
		bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bg->setPosition(this->getContentSize() / 2);
		this->addChild(bg);
	}

	auto gachaBG = LayerColor::create(COLOR::WHITEGRAY_ALPHA, 1080.f, 1500.f);
	if (gachaBG != nullptr){
		gachaBG->setIgnoreAnchorPointForPosition(false);
		gachaBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		gachaBG->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 1.5f));
		this->addChild(gachaBG);
	}

	auto btnExit = CMyButton::create()
		->addEventListener([=](Node* sender){
		this->End(sender);
	})
        ->setButtonSingleUse(true)
		->setButtonNormalImage("endIcon.png")
		->setButtonPosition(Vec2(bg->getContentSize().width * 0.92f, bg->getContentSize().height * 0.05f))
		->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
		->show(bg);
	btnExit->setOpacity(0);

	CMyButton::create()
		->addEventListener([=](Node* sender){
		this->PlayGacha(sender);
	})
		->setLayer(LayerColor::create(COLOR::DARKGRAY_ALPHA, 250, 150))
		->setContents(MakeString("Get -%d", g_coinToGacha))
		->setButtonPosition(Vec2(gachaBG->getContentSize().width * 0.5f, gachaBG->getContentSize().height * 0.15f))
		->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
		->show(gachaBG);

	auto btnUserCoin = CUserCoinButton::create();
	if (btnUserCoin != nullptr)
	{
		btnUserCoin->setPosition(Vec2(bg->getContentSize().width * 0.5f,
			bg->getContentSize().height * 0.05f));
		btnUserCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bg->addChild(btnUserCoin);
		btnUserCoin->setCascadeOpacityEnabled(true);
		btnUserCoin->setOpacity(0);
	}

	this->setOpenAnimation([=](Node* sender){
		btnExit->runAction(FadeIn::create(0.5f));
		btnUserCoin->runAction(FadeIn::create(0.5f));
		gachaBG->runAction(EaseExponentialOut::create(MoveTo::create(0.8f, Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.62f))));

	});

	this->setCloseAnimation([=](Node* sender){
		btnExit->runAction(FadeTo::create(0.5f, 0));
		btnUserCoin->runAction(FadeTo::create(0.5f, 0));
		gachaBG->runAction(	EaseSineIn::create(	MoveTo::create(0.4f, Vec2(visibleSize.width * 0.5f, visibleSize.height * 1.5f))));
	});
    
    this->setDefaultCallback([=](Node* sender){
        this->End(sender);
    });
	
	return true;
}

void CGachaPopup::End(Node* sender){
	CCLOG("format popup End");
	this->popupClose();
}

void CGachaPopup::PlayGacha(Node* sender){
    
    if (CUserDataManager::Instance()->CoinUpdate(-g_coinToGacha)){
        CCLOG("format popup PlayGacha");
        
        int allCharacterCount = static_cast<int>(CCharacterDataManager::Instance()->getCharacterList().size());
        auto haveCharacterList = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHARACTER_LIST);
        int currentCharacterCount = static_cast<int>(haveCharacterList.size());
        
        if (allCharacterCount < haveCharacterList.size())
            CCASSERT(false, "Error : Character count should be less than haveCharacterList count.");
        
        
        int randomIdx = 0;
        // 뽑을 캐릭터가 더이상 없을 때
        if (allCharacterCount <= currentCharacterCount){
            CCLOG("No more Character");
            return;
        }
        else
        {
            do{
                randomIdx = random<int>(0, allCharacterCount - 1);
            } while (CUserDataManager::Instance()->getUserData_IsItemHave(USERDATA_KEY::CHARACTER_LIST, randomIdx));
        }
        
        CCLOG("GET %d", randomIdx);
        CUserDataManager::Instance()->setUserData_ItemGet(USERDATA_KEY::CHARACTER_LIST, randomIdx);
        CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::CHARACTER, randomIdx);
    }
}