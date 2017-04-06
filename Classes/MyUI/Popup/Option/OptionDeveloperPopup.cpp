#include "OptionDeveloperPopup.hpp"
#include "../../MyButton.h"
#include "../../../Scene/GameScene.h"
#include "../../../DataManager/UserDataManager.h"
#include <array>

using namespace cocos2d;
using namespace cocos2d::ui;

COptionDeveloperPopup* COptionDeveloperPopup::create()
{
    COptionDeveloperPopup *pRet = new(std::nothrow) COptionDeveloperPopup();
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

bool COptionDeveloperPopup::init()
{
    if (!CPopup::init()) return false;
    
    this->setContentSize(Size(1080.f, 1500.f));
    auto layerSize = getContentSize();
    
    struct MEMBER {
        std::string _name;
        std::string _job;
        
        MEMBER(std::string name, std::string job)
        : _name(name)
        , _job(job){}
    };

    std::array<MEMBER, 1> members = {
        MEMBER("Hong Seong Hee", "Developer"),
//        MEMBER("Tiramisu", "Artist")
    };
    
    Size dpSize = Size(layerSize.width * 0.9f, layerSize.height * 0.05f);
    float margin = 5;
    
    int index = 0;
    for(auto member : members)
    {
        this->createDeveloperDP(member._name, member._job, Vec2(layerSize.width * 0.5f,
                                                                layerSize.height * 0.6f - ((dpSize.height + margin) * index)));
        index++;
    }
    
    return true;
}

void COptionDeveloperPopup::createDeveloperDP(std::string name, std::string job, Vec2 pos)
{
    auto btn = CMyButton::create()
    ->addEventListener([=](Node* sender){
        
    })
    ->setLayer(LayerColor::create(COLOR::DARKGRAY_ALPHA,
                                  getContentSize().width * 0.9f,
                                  getContentSize().height * 0.05f))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(pos)
    ->setTouchEnable(false, Color3B::WHITE)
    ->show(this);
    btn->setSwallowTouches(false);

    auto nameLabel = Label::createWithTTF(name, FONT::MALGUNBD, 40);
    nameLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    nameLabel->setPosition(Vec2(btn->getContentSize().width * 0.05f,
                                btn->getContentSize().height * 0.5f));
    btn->addChild(nameLabel);
    
    auto jobLabel = Label::createWithTTF(job, FONT::MALGUNBD, 40);
    jobLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    jobLabel->setPosition(Vec2(btn->getContentSize().width * 0.95f,
                               btn->getContentSize().height * 0.5f));
    btn->addChild(jobLabel);
}



