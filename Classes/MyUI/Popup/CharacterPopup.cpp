#include "CharacterPopup.hpp"
#include "CharacterPopupDP.hpp"
#include "../MyButton.h"
#include "../UserCoinButton.h"
#include "../../GameObject/ObjectManager.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"

#include <array>

using namespace cocos2d;
using namespace cocos2d::ui;

CCharacterPopup* CCharacterPopup::create()
{
    CCharacterPopup *pRet = new(std::nothrow) CCharacterPopup();
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

bool CCharacterPopup::init()
{
    if (!CPopup::init()) return false;
    
    auto bg = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080.f, 1920.f);
    if (bg != nullptr){
        bg->setIgnoreAnchorPointForPosition(false);
        bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        bg->setPosition(Vec2(this->getContentSize().width * 0.5f,
                             this->getContentSize().height * 1.5f));
        this->addChild(bg);
    }
    
    Size layerSize = bg->getContentSize();
    
    // create the main sroll view
    m_ContentScrollView = createPageView(Size(layerSize.width, layerSize.height * 0.9f),
                                         Vec2(layerSize.width * 0.5f, layerSize.height * 0.5f));
    bg->addChild(m_ContentScrollView);
    
    // create the top scroll view
    m_TitleScrollView = createListView(Size(layerSize.width, layerSize.height * 0.2f), layerSize.width / 5,
                                       Vec2(layerSize.width * 0.5f, layerSize.height * 0.8f));
    bg->addChild(m_TitleScrollView);
    

    // create dp
    int  index         = 0;
    auto characterList = CCharacterDataManager::Instance()->getCharacterList();
    for(auto data : characterList)
    {
        auto character = data.second;

        auto contentBG = Widget::create();
        contentBG->setContentSize(Size(layerSize.width, layerSize.height * 0.5f));
        contentBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_ContentScrollView->pushBackCustomItem(contentBG);
        
        auto characterDP = CCharacterPopupDP::create(character);
        characterDP->setPosition(contentBG->getContentSize() / 2);
        characterDP->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        contentBG->addChild(characterDP, 0, 100);
        
        auto topBtn = Button::create("empty_150x150.png");
        topBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        topBtn->addClickEventListener([=](Ref* sender){
            
            auto btn = dynamic_cast<Button*>(sender);
            m_TitleScrollView->scrollToItem(btn->getTag(), Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE, .5f);
            m_ContentScrollView->scrollToItem(btn->getTag());
        });
        m_TitleScrollView->addChild(topBtn, 0, index++);
        
        auto label = Label::createWithSystemFont(TRANSLATE(character->_name), FONT::MALGUNBD, 80);
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        label->setPosition(topBtn->getContentSize() / 2);
        label->setCascadeOpacityEnabled(true);
        label->setCascadeColorEnabled(true);
        topBtn->addChild(label);
    }
    
    // user coin ui
    auto btnUserCoin = CUserCoinButton::create();
    btnUserCoin->setPosition(Vec2(layerSize.width * 0.5f, layerSize.height * 0.05f));
    btnUserCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(btnUserCoin);

    
    auto createButton = [=](const std::function<void(Node*)> &callback, std::string imageName, Vec2 pos)->CMyButton*{
        return CMyButton::create()
        ->addEventListener(callback)
        ->setButtonSingleUse(true)
        ->setButtonNormalImage(imageName)
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->setButtonPosition(pos)
        ->show(this);
    };
    
    auto btnEnd = createButton([=](Node* sender){
        this->end();
    }, "endIcon.png", Vec2(layerSize.width * 0.92f, layerSize.height * 0.05f));
    
    
    m_SelectButton = CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->select();
    })
    ->setLayer(LayerColor::create(COLOR::DARKGRAY_ALPHA, 430, 150))
    ->setContents("Select")
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(Vec2(layerSize.width * 0.5f, layerSize.height * 0.2f))
    ->show(this);
    
    // create explain bg
//    auto explainBG = LayerColor::create(COLOR::BRIGHTGRAY_ALPHA, layerSize.width * 0.9f, layerSize.height * 0.2f);
//    explainBG->setIgnoreAnchorPointForPosition(false);
//    explainBG->setPosition(Vec2(layerSize.width * 0.5f, layerSize.height * 0.225f));
//    explainBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//
//    bg->addChild(explainBG);
//    
//    m_ExplainLabel = Label::createWithSystemFont("", FONT::MALGUNBD, 50,
//                                                 Size(explainBG->getContentSize().width * 0.9f,
//                                                      explainBG->getContentSize().height * 0.8f),
//                                                 TextHAlignment::LEFT, TextVAlignment::TOP);
//    m_ExplainLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    m_ExplainLabel->setPosition(explainBG->getContentSize() / 2);
//    explainBG->addChild(m_ExplainLabel);
    
    
    
    this->setOpenAnimation([=](Node* sender){
        auto action = [=](Node* owner){
            auto delay = DelayTime::create(1.f);
            auto fade  = FadeIn::create(0.5f);
            auto sequence = Sequence::createWithTwoActions(delay, fade);
            owner->setOpacity(0);
            owner->runAction(sequence);
        };
        
        action(btnEnd);
        action(btnUserCoin);
        action(m_SelectButton);
        
        auto moveAction = MoveTo::create(1.2f, Vec2(layerSize.width * 0.5f, layerSize.height * 0.5f));
        auto easeAction = EaseExponentialInOut::create(moveAction);
        
        bg->runAction(easeAction);
        
        auto characterIdx = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::CHARACTER);
        m_CurrentData     = CCharacterDataManager::Instance()->getCharacterByIndex(characterIdx);
        m_TitleScrollView->jumpToItem(characterIdx,   Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
        m_ContentScrollView->jumpToItem(characterIdx, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
//        m_ExplainLabel->setString(TRANSLATE(characterData->_name));
    }, 1.2f);
    
    this->setCloseAnimation([=](Node* sender){
        btnEnd->runAction(FadeTo::create(0.3f, 0));
        btnUserCoin->runAction(FadeTo::create(0.3f, 0));
        m_SelectButton->runAction(FadeTo::create(0.3f, 0));

        bg->runAction(EaseExponentialInOut::create(MoveTo::create(1.2f, Vec2(layerSize.width * 0.5f,
                                                                             layerSize.height * 1.5f))));
    });
    
    this->setDefaultCallback([=](Node* sender){
        this->end();
    });
    
    return true;
}

void CCharacterPopup::end(){
    CCLOG("format popup End");
    CObjectManager::Instance()->ZoomOutRank();
    CGameScene::getGameScene()->MenuFadeIn();
    
    this->popupClose(1.3f);
}

void CCharacterPopup::select()
{
    auto manager = CUserDataManager::Instance();
    auto index   = m_CurrentData->_index;
    
    if(manager->getUserData_Number(USERDATA_KEY::CHARACTER) != index) {
        manager->setUserData_Number(USERDATA_KEY::CHARACTER, index);
        CObjectManager::Instance()->ChangeCharacter();
    }
    this->end();
}

void CCharacterPopup::TitleScrollCallback(cocos2d::Ref* ref, ScrollView::EventType type)
{
    ListView* listView = dynamic_cast<ListView*>(ref);
    
    if (listView == nullptr) return;
    if (type != ScrollView::EventType::CONTAINER_MOVED) return;
    
    // Get center dp
    auto center         = listView->getCenterItemInCurrentView();
    auto centerIdx      = listView->getIndex(center);
    auto centerChild    = listView->getChildren().at(centerIdx);
    auto centerIcon     = dynamic_cast<Button*>(centerChild);
    if (centerIcon == nullptr) return;
    
    // Center dp color change
    centerIcon->setOpacity(255);
    centerIcon->setScale(1.5f);
    
    // touch disable the other dp
    for (auto otherIcon : listView->getChildren())
    {
        if (otherIcon != nullptr && otherIcon != center)
        {
            dynamic_cast<Button*>(otherIcon)->setOpacity(255 * 0.4f);
            dynamic_cast<Button*>(otherIcon)->setScale(1.f);
        }
    }
}

void CCharacterPopup::ContentScrollCallback(cocos2d::Ref* ref, PageView::EventType type)
{
    PageView* pageView = dynamic_cast<PageView*>(ref);
    if (pageView == nullptr || type != PageView::EventType::TURNING) return;
    
    // Get center dp
    auto center        = pageView->getCenterItemInCurrentView();
    auto centerIdx     = pageView->getIndex(center);
    auto centerChild   = pageView->getChildren().at(centerIdx);
    auto centerBG      = dynamic_cast<Widget*>(centerChild);
    auto centerContent = dynamic_cast<CCharacterPopupDP*>(centerBG->getChildByTag(100));
    if (centerContent == nullptr) return;

    // update button
    auto currentLevel  = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::LEVEL);
    m_CurrentData      = centerContent->getCharacterParam();
    bool selectable    = currentLevel >= m_CurrentData->_level;
    std::string text   = "Select";
    if(!selectable)text = StringUtils::format("LV.%d", m_CurrentData->_level);
    
    m_SelectButton->setTouchEnable(selectable);
    m_SelectButton->changeContents(text);

    // scroll title view
    m_TitleScrollView->scrollToItem(centerIdx, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE, .5f);
}

cocos2d::ui::ListView* CCharacterPopup::createListView(Size size, size_t distance, Vec2 pos)
{
    auto listView = ListView::create();
    listView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    listView->setContentSize(size);
    listView->setTouchEnabled(false);
    listView->setScrollBarEnabled(false);
    listView->setItemsMargin(distance);
    listView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    listView->setPosition(pos);
    listView->setMagneticType(ListView::MagneticType::CENTER);
    listView->ScrollView::addEventListener((ListView::ccScrollViewCallback)CC_CALLBACK_2(CCharacterPopup::TitleScrollCallback, this));
    return listView;
}

cocos2d::ui::PageView* CCharacterPopup::createPageView(Size size, Vec2 pos)
{
    auto pageView = PageView::create();
    pageView->setDirection(cocos2d::ui::PageView::Direction::HORIZONTAL);
    pageView->setBounceEnabled(true);
    pageView->setContentSize(size);
    pageView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    pageView->setPosition(pos);
    pageView->addEventListener((PageView::ccPageViewCallback)CC_CALLBACK_2(CCharacterPopup::ContentScrollCallback, this));

    return pageView;
}
