#include "CharacterCostumePopup.hpp"
#include "CharacterCostumePopupDP.hpp"
#include "SharePopup.hpp"
#include "../MyButton.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/CostumeDataManager.hpp"
#include "../../DataManager/CharacterDataManager.h"
#include "../../DataManager/UserDataManager.h"
#include "../../GameObject/ObjectManager.h"
#include "../../GameObject/BackGround.h"
#include "../../SDKBOX/SDKBoxHeaders.h"

using namespace cocos2d;
using namespace cocos2d::ui;

CCharacterCostumePopup* CCharacterCostumePopup::create()
{
    CCharacterCostumePopup *pRet = new(std::nothrow) CCharacterCostumePopup();
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

CPopup* CCharacterCostumePopup::show(Node* parent/* = nullptr*/, int zOrder/* = 0*/)
{
    Size layerSize = this->getContentSize();
    
    auto bg = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080.f, 1920.f);
    if (bg != nullptr){
        bg->setIgnoreAnchorPointForPosition(false);
        bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        bg->setPosition(layerSize / 2);
        this->addChild(bg);
    }

    // create the main sroll view
    m_ScrollView = createPageView(Size(layerSize.width, layerSize.height * 0.9f),
                                  Vec2(layerSize.width * 0.5f, layerSize.height * 0.5f));
    bg->addChild(m_ScrollView);

    m_CurrentData    = CCostumeDataManager::Instance()->getCurCostumeByCharacter(m_CharacterIndex);
    auto costumeList = CCostumeDataManager::Instance()->getCostumeListByCharacter(m_CharacterIndex);
    auto jumpIndex   = 0;
    auto index       = 0;
    
    typedef std::pair<int, const COSTUME*> PAIR;
    auto sortedList  = std::vector<PAIR>(costumeList.begin(), costumeList.end());
    // sort
    {
        auto manager = CUserDataManager::Instance();
        // sort by clear (clear achievements locate to the top.)
        std::stable_sort(sortedList.begin(), sortedList.end(), [=](PAIR dataA, PAIR dataB){
            
            auto indexA = dataA.second->_index;
            auto indexB = dataB.second->_index;
            auto existA = manager->getUserData_IsItemExist(USERDATA_KEY::COSTUME_LIST, indexA);
            auto existB = manager->getUserData_IsItemExist(USERDATA_KEY::COSTUME_LIST, indexB);
            return (existA > existB);
        });
    }

    for (auto iter : sortedList)
    {
        auto costume = iter.second;
        if(!costume->_enable) continue;
        
        auto contentBG = Widget::create();
        contentBG->setContentSize(Size(layerSize.width, layerSize.height * 0.5f));
        contentBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        contentBG->setCascadeOpacityEnabled(true);
        m_ScrollView->pushBackCustomItem(contentBG);
        
        auto costumeDP = CCharacterCostumePopupDP::create(costume);
        costumeDP->setPosition(contentBG->getContentSize() / 2);
        costumeDP->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        costumeDP->setCascadeOpacityEnabled(true);
        contentBG->addChild(costumeDP, 0, 100);
        
        if(costume->_index == m_CurrentData->_index){
            jumpIndex = index;
        }
        index++;
    }
    
    m_NameLabel = Label::createWithSystemFont("", FONT::MALGUNBD, 80,
                                              Size(layerSize.width * 0.8f, layerSize.height),
                                              TextHAlignment::CENTER,
                                              TextVAlignment::CENTER);
    m_NameLabel->setPosition(Vec2(layerSize.width * 0.5f, layerSize.height * 0.8f));
    m_NameLabel->setColor(Color3B::WHITE);
    bg->addChild(m_NameLabel);
    
    auto btnEnd = CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->end();
    })
    ->setButtonSingleUse(true)
    ->setButtonNormalImage("endIcon.png")
    ->setButtonPosition(Vec2(layerSize.width * 0.92f, layerSize.height * 0.05f))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(bg);
    
    m_ShareButton = CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->share();
    })
    ->setButtonNormalImage("shareIcon_1.png")
    ->setButtonPosition(Vec2(layerSize.width * 0.08f, layerSize.height * 0.05f))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(bg);
    
    // costume button disable.
    CMyButton::create()
    ->addEventListener([=](Node* sender){
        m_SelectButton->setTouchEnable(false);
        m_ShareButton->setTouchEnable(false);
        m_FingerIcon->setVisible(false);
    }, eMYBUTTON_STATE::BEGIN)
    ->setEnableSound(false)
    ->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, layerSize.width, layerSize.height * 0.8f))
    ->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(Vec2(layerSize.width * 0.5f, layerSize.height * 0.5f))
    ->show(this)
    ->setSwallowTouches(false);
    
    m_SelectButton = CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->select();
    })
    ->setLayer(LayerColor::create(COLOR::DARKGRAY_ALPHA, 430, 150))
    ->setContents(TRANSLATE("CHARACTER_SELECT_BUTTON"))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(Vec2(layerSize.width * 0.5f, layerSize.height * 0.2f))
    ->show(this);
    
    m_FingerIcon = Sprite::create("fingerIcon.png");
    m_FingerIcon->setPosition(Vec2(layerSize.width * 0.8f, layerSize.height * 0.3f));
    m_FingerIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_FingerIcon->setOpacity(0);
    m_FingerIcon->setVisible(m_CurrentData->_index == 0);

    this->addChild(m_FingerIcon);
    
    m_CapturedNode = Sprite::create("empty_150x150.png");
    m_CapturedNode->setPosition(Vec2(-500, -500));
    this->addChild(m_CapturedNode);
    
    auto delay1  = DelayTime::create(3.5f);
    auto fadeIn  = FadeTo::create(0.5f, 255 * .8f);
    auto delay2  = DelayTime::create(0.6f);
    auto fadeOut = FadeTo::create(0.5f, 0);
    auto seq1    = Sequence::create(delay1, fadeIn, delay2, fadeOut, NULL);
    
    auto delay3  = DelayTime::create(3.5f);
    auto moveTo  = MoveTo::create(1.6f, Vec2(layerSize.width * 0.2f, layerSize.height * 0.3f));
    auto ease    = EaseSineInOut::create(moveTo);
    auto moveFrom= MoveTo::create(0.f, Vec2(layerSize.width * 0.8f, layerSize.height * 0.3f));
    auto seq2    = Sequence::create(delay3, ease, moveFrom, NULL);
    auto spawn   = Spawn::create(seq1, seq2, NULL);
    
    m_FingerIcon->runAction(RepeatForever::create(spawn));
    
    this->setOpenAnimation([=](Node* sender){
        auto action = [=](Node* owner){
            auto fade  = FadeIn::create(0.5f);
            owner->setOpacity(0);
            owner->runAction(fade);
        };
//        action(m_ScrollView);
//        action(m_SelectButton);
//        action(m_NameLabel);
//        action(btnEnd);
//        action(bg);
        action(m_ShareButton);

        m_ScrollView->jumpToItem(jumpIndex, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
    }, 1.f);
    
    this->setCloseAnimation([=](Node* sender){
        m_ScrollView->runAction(FadeTo::create(0.1f, 0));
//        m_SelectButton->runAction(FadeTo::create(0.3f, 0));
//        m_NameLabel->runAction(FadeTo::create(0.3f, 0));
//        btnEnd->runAction(FadeTo::create(0.3f, 0));
//        bg->runAction(FadeTo::create(0.5f, 0));
        m_ShareButton->runAction(FadeTo::create(0.3f, 0));
    });
    
    this->setDefaultCallback([=](Node* sender){
        this->end();
    });
    
    return CPopup::show(parent, zOrder);
}

CCharacterCostumePopup* CCharacterCostumePopup::setCharacter(int index)
{
    m_CharacterIndex = index;
    return this;
}

CCharacterCostumePopup* CCharacterCostumePopup::setExitCallback(const VOID_CALLBACK &callback)
{
    m_ExitCallback = callback;
    return this;
}

void CCharacterCostumePopup::end(){
    CCLOG("format popup End");
    if(m_ExitCallback){
        this->retain();
        m_ExitCallback();
        this->release();
    }
    this->popupClose();
}

// When touch the select or buy button
void CCharacterCostumePopup::select()
{
    auto manager = CUserDataManager::Instance();
    auto index   = m_CurrentData->_index;
    
    if(manager->getUserData_IsItemExist(USERDATA_KEY::COSTUME_LIST, index)) {
        manager->setUserData_ItemParam(USERDATA_KEY::CHARACTER_LIST, m_CharacterIndex,
                                       PARAM_CHARACTER::COSTUME_INDEX, index);
        CObjectManager::Instance()->ChangeCostume();
    }
    else{
        auto costumeName = TRANSLATE(m_CurrentData->_name);
        CGameScene::getGameScene()->CreateAlertPopup()
        ->setPositiveButton([=](Node* sender){
        }, TRANSLATE("BUTTON_YES"))
        ->setNegativeButton([=](Node* sender){
        }, TRANSLATE("BUTTON_NO"))
        ->setMessage(StringUtils::format(TRANSLATE("CHARACTER_BUY_CHECK").c_str(), costumeName.c_str()))
        ->show(CGameScene::getPopupLayer(), ZORDER::POPUP);
    }
    
    this->end();
}

void CCharacterCostumePopup::share()
{
    if(m_CapturedNode == nullptr) return;
    
    auto characterData = CCharacterDataManager::Instance()->getCharacterByIndex(m_CharacterIndex);
    auto character = Sprite::createWithSpriteFrameName(characterData->_texture_600);
    character->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    character->setPosition(this->getContentSize() / 2);
    character->setScale(1.5f);
    this->addChild(character);
    
    auto costume = Sprite::createWithSpriteFrameName(m_CurrentData->_texture_600);
    costume->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    costume->setPosition(this->getContentSize() / 2);
    costume->setScale(1.5f);
    this->addChild(costume);
    
    auto renderTexture = RenderTexture::create(1080, 1920);
    renderTexture->begin();
    
    CObjectManager::Instance()->getBackground()->visit();
    character->visit();
    costume->visit();
    
    renderTexture->end();
    
    character->removeFromParent();
    costume->removeFromParent();
    
    auto texture = renderTexture->getSprite()->getTexture();
    m_CapturedNode->removeAllChildren();
    m_CapturedNode->setTexture(texture);
    
    CShareManager::Instance()->setShareTitle(TRANSLATE("COSTUME_SHARE_TITLE"));
    CShareManager::Instance()->setShareText(TRANSLATE("COSTUME_SHARE_TEXT"));
    
    CGoogleAnalyticsManager::LogScreen(GA_SCREEN::SHARE_COSTUME);
    
    CGameScene::getGameScene()->OpenSharePopup([=](){
        CGoogleAnalyticsManager::LogEvent(GA_CATEGORY::SHARE, GA_ACTION::SHARE_COSTUME,
                                          GA_ACTION::SHARE_COSTUME, m_CurrentData->_index);
    }, m_CapturedNode->getTexture(), SIZE_TYPE::HALF_SIZE, true);
}

cocos2d::ui::PageView* CCharacterCostumePopup::createPageView(Size size, Vec2 pos)
{
    auto pageView = PageView::create();
    pageView->setDirection(cocos2d::ui::PageView::Direction::HORIZONTAL);
    pageView->setBounceEnabled(true);
    pageView->setContentSize(size);
    pageView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    pageView->setPosition(pos);
    pageView->setCascadeOpacityEnabled(true);
    pageView->addEventListener((PageView::ccPageViewCallback)CC_CALLBACK_2(CCharacterCostumePopup::scrollCallback, this));
    
    return pageView;
}

void CCharacterCostumePopup::scrollCallback(cocos2d::Ref* ref, PageView::EventType type)
{
    PageView* pageView = dynamic_cast<PageView*>(ref);
    if (pageView == nullptr || type != PageView::EventType::TURNING) return;
    
    // Get center dp
    auto center        = pageView->getCenterItemInCurrentView();
    auto centerIdx     = pageView->getIndex(center);
    auto centerChild   = pageView->getChildren().at(centerIdx);
    auto centerBG      = dynamic_cast<Widget*>(centerChild);
    auto centerContent = dynamic_cast<CCharacterCostumePopupDP*>(centerBG->getChildByTag(100));
    if (centerContent == nullptr) return;
    
    // update button
    m_CurrentData      = centerContent->getCostumeParam();
    bool selectable    = CUserDataManager::Instance()->getUserData_IsItemExist(USERDATA_KEY::COSTUME_LIST, m_CurrentData->_index);
    std::string text   = "Select";
    if(!selectable)text = "Locked";
    
    // update name label
    m_NameLabel->setString(TRANSLATE(m_CurrentData->_name));
    
    m_ShareButton->setTouchEnable(selectable);
    m_SelectButton->setTouchEnable(selectable);
    m_SelectButton->changeContents(text);
}
