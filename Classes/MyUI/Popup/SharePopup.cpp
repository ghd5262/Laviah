#include "SharePopup.hpp"
#include "../MyButton.h"
#include "../../GameObject/ObjectManager.h"
#include "../../Scene/GameScene.h"
#include "../../SDKBOX/SDKBoxHeaders.h"
#include "../../Common/StringUtility.h"

CSharePopup* CSharePopup::create()
{
    CSharePopup *pRet = new(std::nothrow) CSharePopup();
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

CPopup* CSharePopup::show(Node* parent, int zOrder)
{
    auto popupSize = this->getContentSize();
    m_ScreenNode = Node::create();
    m_ScreenNode->setContentSize(popupSize);
    m_ScreenNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_ScreenNode->setCascadeOpacityEnabled(true);
    m_ScreenNode->setPosition(popupSize / 2);
    this->addChild(m_ScreenNode);
    
    if(m_CapturedTexture != nullptr){
        auto copiedNode  = Sprite::createWithTexture(m_CapturedTexture);
        copiedNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        copiedNode->setScaleY(-1);
        copiedNode->setPosition(popupSize / 2);
        copiedNode->setCascadeOpacityEnabled(true);
        m_ScreenNode->addChild(copiedNode);
    }
    
    auto ui = Node::create();
    ui->setContentSize(popupSize);
    ui->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    ui->setPosition(popupSize / 2);
    ui->setCascadeOpacityEnabled(true);
    m_ScreenNode->addChild(ui);
    
    if(m_Score > 0){
        
        auto score = StringUtility::toCommaString(m_Score);
        auto label = Label::createWithTTF(score, FONT::MALGUNBD, 80);
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        label->setPosition(Vec2(popupSize.width * 0.03f, popupSize.height * 0.96f));
        label->setOpacity(255 * 0.1f);
        label->enableOutline(COLOR::BRIGHT_WHITEGRAY_ALPHA, 3);
        ui->addChild(label);
    }
    
    // save to file
    this->saveToFile();
    
    auto createBtn = [=](const std::function<void(Node*)> &callback, std::string icon, Vec2 pos, bool use){
        auto btn = CMyButton::create()
        ->addEventListener(callback)
        ->setButtonSingleUse(use)
        ->setButtonNormalImage(icon)
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->setButtonPosition(pos)
        ->show(this);
        btn->setCascadeOpacityEnabled(true);
        
        return btn;
    };
    
    auto btnEnd = createBtn([=](Node* sender){
        this->end();
    }, "endIcon.png", Vec2(popupSize.width * 0.92f, popupSize.height * 0.05f), true);
    
    auto btnShareFacebook = createBtn([=](Node* sender){
        CShareManager::Share(true, sdkbox::SocialPlatform::Platform_Facebook);
        //        CFacebookManager::Instance()->OpenPhotoShareDialog("");
    }, "shareFacebook.png", Vec2(popupSize.width * 0.08f, popupSize.height * 0.05f), false);
    
    auto btnShareTwitter  = createBtn([=](Node* sender){
        CShareManager::Share(true, sdkbox::SocialPlatform::Platform_Twitter);
    }, "shareTwitter.png", Vec2(popupSize.width * 0.2f, popupSize.height * 0.05f), false);
    
    auto btnShareNative   = createBtn([=](Node* sender){
        CShareManager::ShareNative();
    }, "shareNative.png", Vec2(popupSize.width * 0.32f, popupSize.height * 0.05f), false);
    
    this->setOpenAnimation([=](Node* sender){
        
        auto action = [=](Node* sender, float delayTime){
            sender->setOpacity(0);
            auto delay = DelayTime::create(delayTime);
            auto fade  = FadeIn::create(0.3f);
            auto sequence = Sequence::createWithTwoActions(delay, fade);
            sender->runAction(sequence);
        };
        
        action(m_ScreenNode, 0.f);
        action(btnEnd, 0.3f);
        action(btnShareFacebook, 0.3f);
        action(btnShareTwitter, 0.3f);
        action(btnShareNative, 0.3f);
        
        //        action(back, 0.f);
    }, 0.6f);
    
    this->setCloseAnimation([=](Node* sender){
        
        m_ScreenNode->runAction(FadeTo::create(0.3f, 0));
        btnEnd->runAction(FadeTo::create(0.3f, 0));
        btnShareFacebook->runAction(FadeTo::create(0.3f, 0));
        btnShareTwitter->runAction(FadeTo::create(0.3f, 0));
        btnShareNative->runAction(FadeTo::create(0.3f, 0));
        
        //        back->runAction(FadeTo::create(0.3f, 0));
    });
    
    this->setDefaultCallback([=](Node* sender){
        this->end();
    });

    return CPopup::show(parent, zOrder);
}

CSharePopup* CSharePopup::setSizeType(SIZE_TYPE type)
{
    m_Type = type;
    return this;
}

CSharePopup* CSharePopup::setCapturedTexture(cocos2d::Texture2D* texture)
{
    m_CapturedTexture = texture;
    return this;
}

CSharePopup* CSharePopup::setScore(int score)
{
    m_Score = score;
    return this;
}

CSharePopup* CSharePopup::setLogoEnable(bool enable)
{
    m_LogoEnable = enable;
    return this;
}

void CSharePopup::end(){
    this->popupClose();
}

void CSharePopup::saveToFile()
{
    // cut by size
    auto popupSize = this->getContentSize();
    auto renderTexture = RenderTexture::create(popupSize.width, popupSize.height);
    renderTexture->setPosition(popupSize / 2);
    renderTexture->begin();
    m_ScreenNode->visit();
    renderTexture->end();
    
    auto sizeY = (m_Type == SIZE_TYPE::FULL_SIZE) ? popupSize.height : popupSize.height * 0.7;
    auto saveNode = Node::create();
    saveNode->setContentSize(Size(popupSize.width, sizeY));
    saveNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    saveNode->setCascadeOpacityEnabled(true);
    saveNode->setPosition(Vec2(-1000, -2000));
    this->addChild(saveNode);
    
    auto texture = renderTexture->getSprite()->getTexture();
    auto copiedTexture = Sprite::createWithTexture(texture);
    copiedTexture->setPosition(saveNode->getContentSize() / 2);
    copiedTexture->setScaleY(-1);
    saveNode->addChild(copiedTexture);
    
    if(m_LogoEnable){
        auto logo = Sprite::create("background_0.png");
        logo->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        logo->setPosition(Vec2(popupSize.width - 40, 50));
        logo->setScale(0.5f);
        saveNode->addChild(logo);
    }
    
    // save to file
    CShareManager::SaveNodeToFile(saveNode);
}
