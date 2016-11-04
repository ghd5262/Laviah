#include "GoogleCloudDataInfoPopup.h"
#include "../MyButton.h"
#include "../TextField.h"
#include "../../DataManager/UserDataManager.h"

CGoogleCloudDataInfoPopup* CGoogleCloudDataInfoPopup::create(std::string dataKey)
{
    CGoogleCloudDataInfoPopup *pRet = new(std::nothrow) CGoogleCloudDataInfoPopup(dataKey);
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
            dpBack->setIgnoreAnchorPointForPosition(false);
            dpBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            dpBack->setPosition(Vec2::ZERO);
            m_Popup->addChild(dpBack);
        }
        
        auto keyBack = LayerColor::create(Color4B(255, 255, 255, 255 * 0.8f), 800.f, 120.f);
        if (keyBack != nullptr){
            keyBack->setIgnoreAnchorPointForPosition(false);
            keyBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            keyBack->setPosition(Vec2(dpBack->getContentSize().width * 0.5f, dpBack->getContentSize().height * 0.9f));
            dpBack->addChild(keyBack);
        }
        
        auto valueBack = LayerColor::create(Color4B(255, 255, 255, 255 * 0.8f), 800.f, 1000.f);
        if (valueBack != nullptr){
            valueBack->setIgnoreAnchorPointForPosition(false);
            valueBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            valueBack->setPosition(Vec2(dpBack->getContentSize().width * 0.5f, dpBack->getContentSize().height * 0.5f));
            dpBack->addChild(valueBack);
        }
        
		auto btnSave = CMyButton::createWithLayerColor(Size(200, 100), Color4B(255, 48, 48, 255 * 0.8f), "Save", 40, Color3B::WHITE, eMYBUTTON_STATE::END, [=](Node* sender){this->Save(sender); }, EFFECT_ALPHA)->show(dpBack);
        
        if (btnSave != nullptr)
        {
            btnSave->setPosition(Vec2(dpBack->getContentSize().width * 0.5f, dpBack->getContentSize().height * 0.08f ));
            btnSave->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            btnSave->setCascadeOpacityEnabled(true);
        }

        
        auto btnEnd = CMyButton::create("endIcon.png",
                                        eMYBUTTON_STATE::END,
										[=](Node* sender){this->End(sender); },
                                        EFFECT_ALPHA)->show(dpBack);
        
        if (btnEnd != nullptr)
        {
            btnEnd->setPosition(Vec2(dpBack->getContentSize().width - btnEnd->getContentSize().width * 0.5f,
                                     dpBack->getContentSize().height - btnEnd->getContentSize().height * 0.5f ));
            btnEnd->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            btnEnd->setCascadeOpacityEnabled(true);
        }
        
        
        auto keyLabel = Label::createWithTTF(m_UserDataKey, "fonts/malgunbd.ttf", 40);
        if(keyLabel != nullptr)
        {
            keyLabel->setColor(Color3B::BLACK);
            keyLabel->setPosition(keyBack->getContentSize() * 0.5f);
            keyLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            keyBack->addChild(keyLabel);
        }
        
        
        std::string valueStr = "";
        auto userDataList = CUserDataManager::Instance()->getKeyList();
        if(userDataList.find(m_UserDataKey) != userDataList.end()){
            if(userDataList.find(m_UserDataKey)->second == "userDefaultDatas_Number")
            {
                auto data = CUserDataManager::Instance()->getUserData_Number(m_UserDataKey);
                valueStr = MakeString("%u", data);
            }
            else if(userDataList.find(m_UserDataKey)->second == "userDefaultDatas_List")
            {
                auto dataList = CUserDataManager::Instance()->getUserData_List(m_UserDataKey);
                for(auto data : *dataList)
                {
                    auto dataStr = MakeString(" %u", data);
                    valueStr += dataStr;
                }
            }
        }
        
        auto valueLabel = Label::createWithTTF(valueStr, "fonts/malgunbd.ttf", 40);
        if(valueLabel != nullptr){
            valueLabel->setColor(Color3B::BLACK);
            valueLabel->setPosition(valueBack->getContentSize() * 0.5f);
            valueLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            valueLabel->setDimensions(valueBack->getContentSize().width * 0.9f, valueBack->getContentSize().height * 0.9f);
            valueBack->addChild(valueLabel);
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

void CGoogleCloudDataInfoPopup::Save(Node* sender){
    CCLOG("format popup Save");
    
}

void CGoogleCloudDataInfoPopup::End(Node* sender){
    CCLOG("format popup End");
    CSpecificPopupBase::PopupClose();
}

