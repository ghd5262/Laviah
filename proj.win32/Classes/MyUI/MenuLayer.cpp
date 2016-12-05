#include "MenuLayer.hpp"

using namespace cocos2d;
using namespace cocos2d::ui;

CMenuLayer* CMenuLayer::create()
{
    CMenuLayer *pRet = new(std::nothrow) CMenuLayer();
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
bool CMenuLayer::init()
{
    if (!CPopup::init()) return false;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();

    
    this->setOpenAnimation([=](Node* sender){
    });
    
    this->setCloseAnimation([=](Node* sender){
    });
    
    return true;
}