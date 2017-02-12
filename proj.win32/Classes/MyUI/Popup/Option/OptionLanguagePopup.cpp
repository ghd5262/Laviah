#include "OptionLanguagePopup.hpp"
#include "../../MyButton.h"
#include "../../../Scene/GameScene.h"
#include "../../../DataManager/UserDataManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;

COptionLanguagePopup* COptionLanguagePopup::create()
{
    COptionLanguagePopup *pRet = new(std::nothrow) COptionLanguagePopup();
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

bool COptionLanguagePopup::init()
{
    if (!CPopup::init()) return false;
    
    this->setContentSize(Size(1080.f, 1500.f));
    auto layer = LayerColor::create(COLOR::TRANSPARENT_ALPHA,
                                    getContentSize().width * 0.9f,
                                    getContentSize().height * 0.5f);
    layer->setIgnoreAnchorPointForPosition(false);
    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    layer->setPosition(this->getContentSize() / 2);
    this->addChild(layer);
    
    auto dpSize = Size(getContentSize().width * 0.266f,
                       getContentSize().height * 0.07f);
    auto paddingSize = Size(getContentSize().width * 0.05f,
                            getContentSize().height * 0.034f);
    
    auto translateMNG = CTranslateManager::Instance();
    auto languageKeyList = translateMNG->getLanguageKeyList();
    
    // 이곳은 일부러 이렇게 구현하도록한다.
    // 만약 이곳을 수정할 일이 생긴다면 게임이 성공한게 아닐까
    for(int index = 0, posY = 4; posY >= 0; posY--)
    {
        for(int posX = 0; posX < 3; posX++)
        {
            if(languageKeyList.size() <= index) break;

            auto languageKey = translateMNG->getContentByLanguageKey("LANGUAGE",
                                                                     languageKeyList.at(index));
            auto btn = this->createLanguageDP(languageKey,
                                   Vec2((dpSize.width * 0.5f) + (posX * (dpSize.width  + paddingSize.width )),
                                        (dpSize.height * 0.5f) + (posY * (dpSize.height + paddingSize.height))));
            btn->show(layer);
            btn->setTag(index);
            index++;
        }
    }
    
    return true;
}

CMyButton* COptionLanguagePopup::createLanguageDP(std::string content, Vec2 pos)
{
    return CMyButton::create()
    ->addEventListener([=](Node* sender){
        auto key = CTranslateManager::Instance()->getLanguageKeyList();
        auto index = sender->getTag();
        if(index >= key.size()) return;
        
        CTranslateManager::Instance()->setCurrentSelectLanguage(key.at(index));
        CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::CURRENT_LANGUAGE, index);
        
        // fade out and in
        CGameScene::getGameScene()->ScreenFade([=](){
//            this->popupClose();
//            CGameScene::getGameScene()->OpenOptionPopup(2);
        });
    })
    ->setLayer(LayerColor::create(COLOR::DARKGRAY_ALPHA,
                                  getContentSize().width * 0.26f,
                                  getContentSize().height * 0.08f))
    ->setContents(content)
    ->setButtonPosition(pos)
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE);
}
