#include "GoogleCloudDataInfoPopup.h"
#include "../MyButton.h"
#include "../TextField.h"

CGoogleCloudDataInfoPopup* CGoogleCloudDataInfoPopup::create()
{
    CGoogleCloudDataInfoPopup *pRet = new(std::nothrow) CGoogleCloudDataInfoPopup();
    if (pRet)
    {
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool CGoogleCloudDataInfoPopup::initVariable()
{
    try{
        auto dpBack = LayerColor::create(Color4B(0, 0, 0, 255 * 0.8f), 900.f, 1500.f);
        if (dpBack != nullptr){
            dpBack->ignoreAnchorPointForPosition(false);
            dpBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            dpBack->setPosition(Vec2::ZERO);
            m_Popup->addChild(dpBack);
        }
        
        auto keyBack = LayerColor::create(Color4B(255, 255, 255, 255 * 0.8f), 800.f, 120.f);
        if (keyBack != nullptr){
            keyBack->ignoreAnchorPointForPosition(false);
            keyBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            keyBack->setPosition(Vec2(dpBack->getContentSize().width * 0.5f, dpBack->getContentSize().height * 0.9f));
            dpBack->addChild(keyBack);
        }
        
        auto valueBack = LayerColor::create(Color4B(255, 255, 255, 255 * 0.8f), 800.f, 1000.f);
        if (valueBack != nullptr){
            valueBack->ignoreAnchorPointForPosition(false);
            valueBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            valueBack->setPosition(Vec2(dpBack->getContentSize().width * 0.5f, dpBack->getContentSize().height * 0.5f));
            dpBack->addChild(valueBack);
        }
        
        auto btnSave = CMyButton::createWithLayerColor(Size(200, 100), Color4B(255, 48, 48, 255 * 0.8f), "Save", 40, Color3B::WHITE, END,                                         std::bind(&CGoogleCloudDataInfoPopup::Save, this), EFFECT_ALPHA);
        
        if (btnSave != nullptr)
        {
            btnSave->setPosition(Vec2(dpBack->getContentSize().width * 0.5f,                                   dpBack->getContentSize().height * 0.08f ));
            btnSave->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            dpBack->addChild(btnSave);
            btnSave->setCascadeOpacityEnabled(true);
        }

        
        auto btnEnd = CMyButton::create("endIcon.png",
                                        END,
                                        std::bind(&CGoogleCloudDataInfoPopup::End, this),
                                        EFFECT_ALPHA);
        
        if (btnEnd != nullptr)
        {
            btnEnd->setPosition(Vec2(dpBack->getContentSize().width - btnEnd->getContentSize().width * 0.5f,
                                     dpBack->getContentSize().height - btnEnd->getContentSize().height * 0.5f ));
            btnEnd->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            dpBack->addChild(btnEnd);
            btnEnd->setCascadeOpacityEnabled(true);
        }
        
        std::string keyPlaceHolder = "Key is ...";
        if(1) // If Data isn't null
           keyPlaceHolder = "Key is ...";
        
        auto keyTextField = CTextField::create(keyPlaceHolder, "fonts/malgunbd.ttf", 40,
                                            Vec2(keyBack->getContentSize().width * 0.5f,
                                                 keyBack->getContentSize().height * 0.5f));
        if(keyTextField != nullptr){
            keyTextField->setTextColor(Color4B::BLACK);
            keyBack->addChild(keyTextField);
        }
        std::string valuePlaceHolder = "Value is ...";
        if(1) // If Data isn't null
            valuePlaceHolder = "Value is ...";
        
        auto valueTextField = CTextField::create(valuePlaceHolder, "fonts/malgunbd.ttf", 40,
                                               Vec2(valueBack->getContentSize().width * 0.5f,
                                                    valueBack->getContentSize().height * 0.9f));
        if(valueTextField != nullptr){
            valueTextField->setTextColor(Color4B::BLACK);
            valueBack->addChild(valueTextField);
        }
        m_Popup->setPopupOpenEffectFunc([this](CPopup* pausePopup){
            
        });
        
        m_Popup->setPopupCloseEffectFunc([this](CPopup* pausePopup){
            m_Popup->scheduleOnce([this](float delta){
                CSpecificPopupBase::PopupRelease();
            }, 0.1f, "CGoogleCloudDataInfoPopupClose");
        });
    }
    catch (...){
        throw StringUtils::format("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
        return false;
    }
    return true;
}

void CGoogleCloudDataInfoPopup::Save(){
    CCLOG("format popup Save");
    
}

void CGoogleCloudDataInfoPopup::End(){
    CCLOG("format popup End");
    CSpecificPopupBase::PopupClose();
}

