#include "OptionMusicPopup.hpp"
#include "../../MyButton.h"
#include "../../../Common/AudioManager.h"
#include "../../../DataManager/UserDataManager.h"
#include "ui/UISlider.h"
#include <array>

using namespace cocos2d;
using namespace cocos2d::ui;

COptionMusicPopup* COptionMusicPopup::create()
{
    COptionMusicPopup *pRet = new(std::nothrow) COptionMusicPopup();
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

bool COptionMusicPopup::init()
{
    if (!CPopup::init()) return false;
    
    this->setContentSize(Size(1080.f, 750.f));

    auto layer = LayerColor::create(COLOR::BRIGHTGRAY_ALPHA,
                                    getContentSize().width * 0.9f,
                                    getContentSize().height);
    layer->setIgnoreAnchorPointForPosition(false);
    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    layer->setPosition(this->getContentSize() / 2);
    this->addChild(layer);
    
    auto createSlider = [=](std::function<void(int)> listener,
                            std::string content,
                            Vec2 contentPos,
                            float percent,
                            Vec2 pos){
        
        auto label = Label::createWithSystemFont(content, FONT::MALGUNBD, 40);
        label->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        label->setPosition(contentPos);
        label->setColor(COLOR::DARKGRAY);
        layer->addChild(label);
        
        auto volumeSlider = Slider::create();
        volumeSlider->loadBarTexture("volumeProgressBarBG.png");
        volumeSlider->loadSlidBallTextures("volumeSlidBall.png", "volumeSlidBall.png", "");
        volumeSlider->loadProgressBarTexture("volumeProgressBar.png");
        volumeSlider->setPercent(percent);
        volumeSlider->setMaxPercent(100);
        volumeSlider->addEventListener([=](Ref* sender, Slider::EventType event){
            if(event != Slider::EventType::ON_PERCENTAGE_CHANGED) return;
            
            Slider *slider = dynamic_cast<Slider*>(sender);
            auto percent = slider->getPercent();
            listener(percent);
        });
        volumeSlider->setPosition(pos);
        volumeSlider->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        layer->addChild(volumeSlider);
    };
    
    m_BGMVolume     = CAudioManager::Instance()->getBGMVolume() * 100.f;
    m_EffectVolume  = CAudioManager::Instance()->getEffectSoundVolume() * 100.f;
    CCLOG("COptionMusicPopup1 : BGM Volume %d", m_BGMVolume);
    CCLOG("COptionMusicPopup1 : EFFECT Volume %d", m_EffectVolume);
    
    
    createSlider([&](int percent){
        m_BGMVolume = percent;
        CAudioManager::Instance()->setBGMVolume(m_BGMVolume / 100.f);
        CAudioManager::Instance()->PlayEffectSound("sounds/Click_1-1.mp3", false, m_BGMVolume / 100.f);
    }, TRANSLATE("OPTION_MUSIC_BGM"),
                 Vec2(layer->getContentSize().width * 0.13f,
                      layer->getContentSize().height * 0.7f),
                 m_BGMVolume, Vec2(layer->getContentSize().width * 0.5f,
                                   layer->getContentSize().height * 0.6f));
    
    createSlider([&](int percent){
        m_EffectVolume = percent;
        CAudioManager::Instance()->setEffectSoundVolume(m_EffectVolume / 100.f);
        CAudioManager::Instance()->PlayEffectSound("sounds/Click_1-1.mp3", false, m_EffectVolume / 100.f);
    }, TRANSLATE("OPTION_MUSIC_EFFECT"),
                 Vec2(layer->getContentSize().width * 0.13f,
                      layer->getContentSize().height * 0.4f),
                 m_EffectVolume, Vec2(layer->getContentSize().width * 0.5f,
                                      layer->getContentSize().height * 0.3f));
    
    this->setOnExitCallback([=](){
        CCLOG("COptionMusicPopup2 : BGM Volume %d", m_BGMVolume);
        CCLOG("COptionMusicPopup2 : EFFECT Volume %d", m_EffectVolume);

        CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::BGM_VOLUME, m_BGMVolume);
        CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::EFFECT_VOLUME, m_EffectVolume);
    });
    return true;
}
