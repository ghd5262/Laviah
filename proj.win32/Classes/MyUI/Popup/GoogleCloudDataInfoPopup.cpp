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

bool CGoogleCloudDataInfoPopup::init()
{
    if (!CPopup::init()) return false;
    
    auto dpBack = LayerColor::create(COLOR::DARKGRAY_ALPHA, 900.f, 1500.f);
    if (dpBack != nullptr){
        dpBack->setIgnoreAnchorPointForPosition(false);
        dpBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        dpBack->setPosition(Vec2::ZERO);
        this->addChild(dpBack);
    }
    
    auto keyBack = LayerColor::create(COLOR::WHITEGRAY_ALPHA, 800.f, 120.f);
    if (keyBack != nullptr){
        keyBack->setIgnoreAnchorPointForPosition(false);
        keyBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        keyBack->setPosition(Vec2(dpBack->getContentSize().width * 0.5f, dpBack->getContentSize().height * 0.9f));
        dpBack->addChild(keyBack);
    }
    
    auto valueBack = LayerColor::create(COLOR::WHITEGRAY_ALPHA, 800.f, 1000.f);
    if (valueBack != nullptr){
        valueBack->setIgnoreAnchorPointForPosition(false);
        valueBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        valueBack->setPosition(Vec2(dpBack->getContentSize().width * 0.5f, dpBack->getContentSize().height * 0.5f));
        dpBack->addChild(valueBack);
    }
    
    CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->Save(sender);
    })
    ->setLayer(LayerColor::create(COLOR::BRIGHTRED_ALPHA, 200, 100))
    ->setContents("Save")
    ->setFont(Color3B::WHITE, 40)
    ->setButtonPosition(Vec2(dpBack->getContentSize().width * 0.5f,
                             dpBack->getContentSize().height * 0.08f ))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(dpBack);
    
    
    auto btnEnd = CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->End(sender);
    })
    ->setButtonNormalImage( "endIcon.png" )
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(dpBack);
    
    btnEnd->setPosition(Vec2(dpBack->getContentSize().width - btnEnd->getContentSize().width * 0.5f,
                             dpBack->getContentSize().height - btnEnd->getContentSize().height * 0.5f ));
    
    
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
        valueLabel->setDimensions(valueBack->getContentSize().width * 0.9f,
                                  valueBack->getContentSize().height * 0.9f);
        valueBack->addChild(valueLabel);
    }
    return true;
}

void CGoogleCloudDataInfoPopup::Save(Node* sender){
    CCLOG("format popup Save");
    
}

void CGoogleCloudDataInfoPopup::End(Node* sender){
    CCLOG("format popup End");
    this->popupClose();
}

