#include "RewardPopup.h"
#include "RewardPopupDP.hpp"
#include "../MyButton.h"
#include "../UserCoinButton.h"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/AchievementDataManager.hpp"
#include "../../DataManager/AchievementRewarder/AchievementRewarder.hpp"
#include "../../DataManager/CharacterDataManager.h"
#include "../../DataManager/CostumeDataManager.hpp"
#include "../../Scene/GameScene.h"
#include "../../GameObject/ObjectManager.h"
#include "../../DataManager/DataManagerUtils.h"
#include "../../Particle/Particles.h"
#include "../../Common/StringUtility.h"
#include "../../SDKBOX/SDKBoxHeaders.h"
#include <array>

CRewardPopup* CRewardPopup::create()
{
	CRewardPopup *pRet = new(std::nothrow) CRewardPopup();
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

CPopup* CRewardPopup::show(cocos2d::Node* parent/*  = nullptr*/, int zOrder/* = 0*/)
{
    
    auto popupSize = this->getContentSize();

    // touch disable
    auto createTouchDisable = [=](Node* parent, Size size, int zOrder = 0){
        CMyButton::create()
        ->addEventListener([](Node* sender){})
        ->setEnableSound(false)
        ->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
        ->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, size.width, size.height))
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->setButtonPosition(parent->getContentSize() / 2)
        ->show(parent, zOrder);
    };
    
    // disable the background
    createTouchDisable(this, Size(1080, 1920 * 3.f));
    
    CMyButton::create()
    ->addEventListener([=](Node* sender){
        if(m_IsPaidFeature) return;
        this->open();
    })
    ->setEnableSound(false)
    ->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
    ->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, popupSize.width, popupSize.height))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(popupSize / 2)
    ->show(this);
    
    
    std::string title    = m_Title;
    std::string subTitle = "";
    
    if(m_Title != ""){
        auto list    = StringUtility::split(m_Title, '-');
        if(list.size() > 1){
            title    = list.at(0);
            subTitle = list.at(1);
        }
    }
    
    m_TitleLabel = Label::createWithSystemFont(title, FONT::MALGUNBD, 80);
    if (m_TitleLabel != nullptr)
    {
        m_TitleLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_TitleLabel->setPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.8f));
        m_TitleLabel->setCascadeOpacityEnabled(true);
        m_TitleLabel->setOpacity(0);
        this->addChild(m_TitleLabel);
        
        if(subTitle != ""){
            auto subTitleLabel = Label::createWithSystemFont(subTitle, FONT::MALGUN, 40);
            subTitleLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            subTitleLabel->setPosition(Vec2(m_TitleLabel->getContentSize().width * 0.5f, 0));
            subTitleLabel->setCascadeOpacityEnabled(true);
            m_TitleLabel->addChild(subTitleLabel);
        }
    }
    
    m_GetButton = CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->open();
    })
    ->setLayer(LayerColor::create(COLOR::DARKGRAY_ALPHA, 430, 150))
    ->setContents(TRANSLATE("BUTTON_GET"))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.2f))
    ->show(this);
    
    
    m_PlayButton = CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->play();
    })
    ->setLayer(LayerColor::create(COLOR::BRIGHTRED_ALPHA, 430, 150))
    ->setContents(TRANSLATE("BUTTON_PLAY"))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(Vec2(popupSize.width * 0.275f, popupSize.height * 0.2f))
    ->show(this);
    m_PlayButton->setVisible(false);
    
    // disable the background of Get button
    createTouchDisable(m_GetButton, Size(1080, 1920 * 2.f), -1);
    
    auto btnEnd = CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->exit();
    })
    ->setButtonSingleUse(true)
    ->setButtonNormalImage("endIcon.png")
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(Vec2(popupSize.width * 0.92f, popupSize.height * 0.05f))
    ->show(this);
    if(!m_IsPaidFeature) btnEnd->setVisible(false);
    
    auto btnUserCoin = CUserCoinButton::create();
    btnUserCoin->setPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.05f));
    btnUserCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(btnUserCoin);
    
    // create UFO
    if(m_IsUFOEnable)
        this->createUFO();
    
    this->setOpenAnimation([=](Node* sender){
        auto action = [=](Node* owner){
            auto delay = DelayTime::create(1.f);
            auto fade  = FadeIn::create(0.5f);
            auto sequence = Sequence::createWithTwoActions(delay, fade);
            owner->setOpacity(0);
            owner->runAction(sequence);
        };
        
        action(btnUserCoin);
        action(btnEnd);
        action(m_TitleLabel);
        
        this->showButtons();
        
    }, 1.2f);
    
    this->setCloseAnimation([=](Node* sender){
        btnUserCoin->runAction(FadeTo::create(0.3f, 0));
        btnEnd->runAction(FadeTo::create(0.3f, 0));
        m_GetButton->runAction(FadeTo::create(0.3f, 0));
        m_PlayButton->runAction(FadeTo::create(0.3f, 0));
        m_TitleLabel->runAction(FadeTo::create(0.3f, 0));
    });
    
    if(m_IsPaidFeature) this->setDefaultCallback([=](Node* sender){ this->exit(); });
    else                this->setDefaultCallback([=](Node* sender){ this->open(); }, false);

    return CPopup::show(parent, zOrder);
}

CRewardPopup* CRewardPopup::AddRewardToList(std::vector<sREWARD_DATA> list)
{
    DATA_MANAGER_UTILS::copyList(list, m_RewardList);
    return this;
}

CRewardPopup* CRewardPopup::setExitCallback(const EXIT_CALLBACK &callback)
{
    m_ExitCallback = callback;
    return this;
}

CRewardPopup* CRewardPopup::setTitle(std::string title)
{
    m_Title = title;
    return this;
}

CRewardPopup* CRewardPopup::setIsPaidFeature(int cost)
{
    m_Cost = cost;
    m_IsPaidFeature = (cost != 0);
    return this;
}

CRewardPopup* CRewardPopup::setIsUFOEnable(bool enable)
{
    m_IsUFOEnable = enable;
    return this;
}

void CRewardPopup::AddRewardToList(std::string key, int value)
{
	m_RewardList.emplace_back(sREWARD_DATA(key, value));
}

void CRewardPopup::createFlyAction(Node* sender, Vec2 targetPos1, Vec2 targetPos2)
{
    auto delay     = DelayTime::create(1.f);
    auto callFunc1 = CallFunc::create([=](){sender->setVisible(true);});
    auto moveDown1 = MoveTo::create(2.5f, targetPos1);
    auto sineOut1  = EaseExponentialOut::create(moveDown1);
    auto callFunc2 = CallFunc::create([=](){
        auto moveUp    = MoveTo::create(7.f,  targetPos2);
        auto sineOut2  = EaseSineInOut::create(moveUp);
        auto moveDown2 = MoveTo::create(7.f,  targetPos1);
        auto sineOut3  = EaseSineInOut::create(moveDown2);
        
        auto rotation1 = RotateTo::create(7.f, 5);
        auto sineOut4  = EaseSineInOut::create(rotation1);
        auto rotation2 = RotateTo::create(7.f, -5);
        auto sineOut5  = EaseSineInOut::create(rotation2);
        
        auto sizeDown  = ScaleTo::create(7.f, 1.f, 0.9f);
        auto sineOut6  = EaseSineInOut::create(sizeDown);
        auto sizeUp    = ScaleTo::create(7.f, 1.f, 1.f);
        auto sineOut7  = EaseSineInOut::create(sizeUp);
        
        auto repeat1   = (Sequence::createWithTwoActions(sineOut2, sineOut3));
        auto repeat2   = (Sequence::createWithTwoActions(sineOut4, sineOut5));
        auto repeat3   = (Sequence::createWithTwoActions(sineOut6, sineOut7));
        auto spawn     = Spawn::create(repeat1, repeat2, repeat3, nullptr);
        
        sender->runAction(RepeatForever::create(spawn));
    });
    auto sequence  = Sequence::create(delay, callFunc1, sineOut1, callFunc2, nullptr);
    sender->runAction(sequence);
    sender->setVisible(false);
}

CPopup* CRewardPopup::createRewardDP(sREWARD_DATA data)
{
    return CRewardPopupDP::create()
    ->setExitCallback([=](){
        if(this->isItemRemain()) this->open();
        else                     this->showButtons();
    })
    ->setRewardData(data)
    ->setIsUFOEnable(m_IsUFOEnable)
//    ->setDefaultCallbackEnable(false)
    ->setBackgroundVisible(false)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(this->getContentSize() / 2)
    ->show(this, ZORDER::POPUP);
}

void CRewardPopup::createUFO()
{
    auto popupSize    = this->getContentSize();
    
    m_UFO = Sprite::create("costumeUFO.png");
    m_UFO->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_UFO->setPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 1.5f));
    this->addChild(m_UFO, 10);
    
    m_UFOLight = Sprite::create("costumeUFOLight.png");
    m_UFOLight->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_UFOLight->setPosition(Vec2(m_UFO->getContentSize() / 2.f));
    m_UFOLight->setOpacity(0);
    m_UFO->addChild(m_UFOLight);
    
    this->createFlyAction(m_UFO, Vec2(popupSize.width * 0.5f, popupSize.height * 0.6f),
                          Vec2(popupSize.width * 0.5f, popupSize.height * 0.68f));
}

void CRewardPopup::lightOn()
{
    // Run the action of UFO light.
    {
        auto lightIn  = FadeIn::create(1.f);
        auto lightOut = FadeTo::create(2.f, 1);
        auto callFunc = CallFunc::create([=](){
            m_UFOLight->setOpacity(0);
        });
        auto lightSeq = Sequence::create(lightIn, lightOut, callFunc, nullptr);
        m_UFOLight->setOpacity(0);
        m_UFOLight->runAction(lightSeq);
    }
    
    auto particle = CParticle_BackGround::createWithTotalParticles(100, "whiteSquare.png");
    particle->setPosition(Vec2(m_UFO->getContentSize().width * 0.5f, m_UFO->getContentSize().height * 0.7f));
    particle->setPosVar(Vec2(100, 0));
    particle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    particle->setLife(1.5f);
    particle->setLifeVar(1.f);
    particle->setSpeed(100);
    particle->setSpeedVar(30);
    particle->setAngleVar(0);
    particle->setDuration(2.f);
    particle->setGravity(Vec2(0, 90));
    particle->setRotation(180);
    m_UFO->addChild(particle, -1);
}

void CRewardPopup::open()
{
	if (m_RewardDP != nullptr){
		m_RewardDP->popupClose();
		m_RewardDP = nullptr;
	}
    
    if (m_IsPaidFeature){ // 구매 상품인 경우
        // 더 이상 구매할 수 있는 상품이 없는 경우
        if(!CCostumeDataManager::Instance()->getNewRandomCostume()) {
            CGameScene::getGameScene()->CreateAlertPopup()
            ->setPositiveButton([=](Node* sender){}, TRANSLATE("BUTTON_OK"))
            ->setMessage(TRANSLATE("NO_MORE_COSTUME"))
            ->show(CGameScene::getPopupLayer(), ZORDER::POPUP);
        }
        // 구매할 수 있는 상품이 있는 경우
        // 보상 리스트에 추가
        else this->AddRewardToList(ACHIEVEMENT_REWARD_KEY::REWARD_COSTUME_RANDOM, 0);
    }
    
    // 보상 리스트에 아무 것도 없는 경우
    // 즉, 구매할 수 있는 상품이 없거나 보상을 모두 받은 경우
    // 또, 구매할 수 있는 코인이 없는 경우
    if (!this->isItemRemain() || !CUserDataManager::Instance()->CoinUpdate(m_Cost)){
        this->exit();
		return;
	}
    
    // 구매한 경우 GA 로그 저장
    if (m_IsPaidFeature){
        CGoogleAnalyticsManager::LogEventCoin(GA_ACTION::COIN_USE_COSTUME, m_Cost);
    }
    
	m_LastSavedData = m_RewardList.at(m_RewardIndex);
	m_LastSavedData = CAchievementDataManager::Instance()->RewardByKey(m_LastSavedData._key,
                                                                       m_LastSavedData._value);
	m_RewardDP      = this->createRewardDP(m_LastSavedData);
    
    if(m_IsUFOEnable)
        this->lightOn();
    m_GetButton->setVisible(false);
    m_PlayButton->setVisible(false);
    m_TitleLabel->runAction(FadeTo::create(0.3f, 0));

	m_RewardIndex++;
}

void CRewardPopup::play()
{
    this->exit(true);
    
    auto index = m_LastSavedData._value;
    
    if(m_LastSavedData._key == ACHIEVEMENT_REWARD_KEY::REWARD_CHARACTER_RANDOM ||
       m_LastSavedData._key == ACHIEVEMENT_REWARD_KEY::REWARD_CHARACTER){
        if(!CUserDataManager::Instance()->getUserData_IsItemExist(USERDATA_KEY::CHARACTER_LIST, index))
            return;
        
        CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::CHARACTER, index);
    }
    if(m_LastSavedData._key == ACHIEVEMENT_REWARD_KEY::REWARD_COSTUME_RANDOM ||
       m_LastSavedData._key == ACHIEVEMENT_REWARD_KEY::REWARD_COSTUME){
        if(!CUserDataManager::Instance()->getUserData_IsItemExist(USERDATA_KEY::COSTUME_LIST, index))
            return;
        
        auto data = CCostumeDataManager::Instance()->getCostumeByIndex(index);
        auto characterIndex = data->_characterIndex;
        CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::CHARACTER, characterIndex);
        CUserDataManager::Instance()->setUserData_ItemParam(USERDATA_KEY::CHARACTER_LIST, characterIndex,
                                                            PARAM_CHARACTER::COSTUME_INDEX, index);
    }
    
    CObjectManager::Instance()->ChangeCharacter();
    CGameScene::getGameScene()->GameStart();
}

void CRewardPopup::end()
{
    this->popupClose(1.3f);
}

void CRewardPopup::exit(bool isPlay/* = false*/)
{
    this->retain();
    if (m_ExitCallback){
        m_ExitCallback(isPlay);
        m_ExitCallback = nullptr;
    }
    this->end();
    this->release();
}

void CRewardPopup::showButtons()
{
    auto action = [=](Node* owner){
        auto delay = DelayTime::create(1.f);
        auto fade  = FadeIn::create(0.5f);
        auto sequence = Sequence::createWithTwoActions(delay, fade);
        owner->setVisible(true);
        owner->setOpacity(0);
        owner->runAction(sequence);
    };
    
    auto layerSize = this->getContentSize();
    
    if(m_LastSavedData._key == ACHIEVEMENT_REWARD_KEY::REWARD_CHARACTER_RANDOM ||
       m_LastSavedData._key == ACHIEVEMENT_REWARD_KEY::REWARD_CHARACTER ||
       m_LastSavedData._key == ACHIEVEMENT_REWARD_KEY::REWARD_COSTUME_RANDOM ||
       m_LastSavedData._key == ACHIEVEMENT_REWARD_KEY::REWARD_COSTUME){
        m_PlayButton->setPosition(Vec2(layerSize.width * 0.275f, layerSize.height * 0.2f));
        m_GetButton->setPosition(Vec2(layerSize.width * 0.725f, layerSize.height * 0.2f));
        action(m_PlayButton);
    }
    
    if(!isItemRemain()) m_GetButton->changeContents(TRANSLATE("BUTTON_CONFIRM"));
    if(m_IsPaidFeature) m_GetButton->changeContents(StringUtils::format(TRANSLATE("BUY_COSTUME").c_str(), m_Cost));
    
    action(m_GetButton);
}

bool CRewardPopup::isItemRemain()
{
    return (m_RewardIndex < m_RewardList.size());
}
