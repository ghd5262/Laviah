#include "OptionMusicPopup.hpp"
#include "../../MyButton.h"
#include "../../../Common/AudioManager.h"
#include "../../../Scene/GameScene.h"
#include "../../../DataManager/UserDataManager.h"
#include "../../../SDKBOX/SDKBoxHeaders.h"
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
    
    this->setContentSize(Size(1080.f, 1500.f));

    auto layer = LayerColor::create(COLOR::BRIGHTGRAY_ALPHA,
                                    getContentSize().width * 0.9f,
                                    getContentSize().height * 0.5f);
    layer->setIgnoreAnchorPointForPosition(false);
    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    layer->setPosition(this->getContentSize() / 2);
    this->addChild(layer);
    
    auto layerSize = layer->getContentSize();
    
    auto createSlider = [=](std::function<void(float)> listener,
                            std::string content,
                            Vec2 contentPos,
                            float percent,
                            Vec2 pos){
        
        auto label = Label::createWithSystemFont(content, FONT::MALGUNBD, 40);
        label->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        label->setPosition(contentPos);
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
    
    m_BGMVolume     = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::BGM_VOLUME);
    m_EffectVolume  = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::EFFECT_VOLUME);
    
    
    createSlider([&](float percent){
        m_BGMVolume = percent;
        CAudioManager::Instance()->setBGMVolume(m_BGMVolume);
//        CAudioManager::Instance()->PlayEffectSound("sounds/Click_4-1.mp3", false, m_BGMVolume);
    }, TRANSLATE("OPTION_MUSIC_BGM"),
                 Vec2(layerSize.width * 0.135f, layerSize.height * 0.8f), m_BGMVolume,
                 Vec2(layerSize.width * 0.5f,   layerSize.height * 0.7f));
    
    createSlider([&](float percent){
        m_EffectVolume = percent;
        CAudioManager::Instance()->setEffectSoundVolume(m_EffectVolume);
//        CAudioManager::Instance()->PlayEffectSound("sounds/Click_4-1.mp3", false, m_EffectVolume);
    }, TRANSLATE("OPTION_MUSIC_EFFECT"),
                 Vec2(layerSize.width * 0.135f, layerSize.height * 0.5f), m_EffectVolume,
                 Vec2(layerSize.width * 0.5f,   layerSize.height * 0.4f));
    
    this->setOnExitCallback([=](){
        CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::BGM_VOLUME,    m_BGMVolume);
        CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::EFFECT_VOLUME, m_EffectVolume);
        CGoogleAnalyticsManager::LogDimension(GA_DIMENSION::OPTION_BGM, m_BGMVolume);
        CGoogleAnalyticsManager::LogDimension(GA_DIMENSION::OPTION_EFFECT_SOUND, m_EffectVolume);

    });
    
    auto alertCreate = [=](std::function<void(Node*)> positive, std::string content){
        return CGameScene::getGameScene()->CreateAlertPopup()
        ->setPositiveButton(positive, TRANSLATE("BUTTON_YES"))
        ->setNegativeButton([](Node* sender){}, TRANSLATE("BUTTON_NO"))
        ->setMessage(content)
        ->show(CGameScene::getPopupLayer(), ZORDER::POPUP);
    };
    
    auto buttonCreate = [=](std::function<void(Node*)> callback, std::string content, Vec2 pos){
        auto btn = CMyButton::create()
        ->addEventListener(callback)
        ->setLayer(LayerColor::create(COLOR::DARKGRAY_ALPHA, 600, 150))
        ->setContents(content)
        ->setButtonPosition(pos)
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->show(layer);
        btn->setSwallowTouches(false);
        
        return btn;
    };
    
    buttonCreate([=](Node* sender){
        
        alertCreate([](Node* sender){
            CGameScene::getGameScene()->setNeedTutorial(true);
            CGameScene::getGameScene()->ScreenFade([=](){
                //option popup close
                CPopup::DefaultCallback();
                CGameScene::getGameScene()->GameStart();
            });
        }, TRANSLATE("OPTION_TUTORIAL_START_CHECK"));
        
    }, TRANSLATE("OPTION_TUTORIAL"), Vec2(layer->getContentSize().width * 0.5f,
                                          layer->getContentSize().height * 0.2f));
    
    return true;
}
