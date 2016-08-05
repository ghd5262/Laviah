#include "GoogleCloudTestAddKeyDP.h"
#include "../MyButton.h"
#include "../TextField.h"
#include "../Popup.h"
#include "../../DataManager/UserDataManager.h"

CGoogleCloudTestAddKeyDP* CGoogleCloudTestAddKeyDP::create()
{
    CGoogleCloudTestAddKeyDP *pRet = new(std::nothrow) CGoogleCloudTestAddKeyDP();
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

bool CGoogleCloudTestAddKeyDP::init()
{
    if (!initVariable())
        return false;
    return true;
}

bool CGoogleCloudTestAddKeyDP::initVariable()
{
    try{
        auto dpBack = LayerColor::create(Color4B(0, 0, 0, 0), 1080.f, 200.f);
        if (dpBack != nullptr){
            dpBack->ignoreAnchorPointForPosition(false);
            dpBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            dpBack->setPosition(Vec2::ZERO);
            this->addChild(dpBack);
        }
        
        auto dpItemBack = LayerColor::create(Color4B(0, 0, 0, 255 * 0.4f), 805.f, 200.f);
        if (dpItemBack != nullptr){
            dpItemBack->ignoreAnchorPointForPosition(false);
            dpItemBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            dpItemBack->setPosition(Vec2(0 + dpItemBack->getContentSize().width * 0.5f, 0));
            dpBack->addChild(dpItemBack);
        }
        
        auto dpWhiteBack = LayerColor::create(Color4B(255, 255, 255, 255), dpItemBack->getContentSize().width * 0.9f, dpItemBack->getContentSize().height * 0.7f);
        
        if (dpWhiteBack != nullptr){
            dpWhiteBack->ignoreAnchorPointForPosition(false);
            dpWhiteBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            dpWhiteBack->setPosition(Vec2(dpItemBack->getContentSize().width * 0.5f, dpItemBack->getContentSize().height * 0.5f));
            dpItemBack->addChild(dpWhiteBack);
        }
        
        m_TextField = CTextField::create("add key", "fonts/malgunbd.ttf", 40, Vec2(dpWhiteBack->getContentSize().width * 0.5f, dpWhiteBack->getContentSize().height * 0.5f));
        if(m_TextField != nullptr)
        {
            m_TextField->setDimensions(Size(dpWhiteBack->getContentSize().width * 0.9f, dpWhiteBack->getContentSize().height * 0.9f));
            m_TextField->setTextColor(Color4B::BLACK);
            dpWhiteBack->addChild(m_TextField);
        }

        auto dpAddKeyBtn = CMyButton::createWithLayerColor(Size(260, 200), Color4B(0, 0, 0, 255 * 0.8f)
                                                        ,"ADD KEY" , 40, g_labelColor2, END, std::bind(&CGoogleCloudTestAddKeyDP::Buy, this), EFFECT_SIZEDOWN);
        if (dpAddKeyBtn != nullptr)
        {
            dpAddKeyBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            dpAddKeyBtn->setPosition(Vec2(dpBack->getContentSize().width - (dpAddKeyBtn->getContentSize().width * 0.5f), 0));
            dpBack->addChild(dpAddKeyBtn);
        }
    }
    catch (...){
        throw StringUtils::format("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
        return false;
    }
    return true;
}

void CGoogleCloudTestAddKeyDP::Buy()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto btnYes = CMyButton::createWithLayerColor(Size(430, 150),
                                                  Color4B(0, 0, 0, 255 * 0.8f),
                                                  "Yes",
                                                  40,
                                                  Color3B::WHITE,
                                                  END,
                                                  [this](){
//                                                      CUserDataManager::Instance()->addKey(m_TextField->getText());
                                                      m_TextField->setText("");
                                                  }, EFFECT_ALPHA);
    
    auto btnNo = CMyButton::createWithLayerColor(Size(430, 150),
                                                 Color4B(255, 48, 48, 255 * 0.8f),
                                                 "No",
                                                 40,
                                                 Color3B::WHITE,
                                                 END,
                                                 [](){}, EFFECT_ALPHA);
    
    auto popup = CPopup::createWithTwoButton("Are you sure you want to save the key?"
                                             , btnNo, btnYes, 40, Color3B::BLACK);
    popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
                            origin.x + visibleSize.height * 0.5f));
    popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    Director::getInstance()->getRunningScene()->addChild(popup, 102);
}
