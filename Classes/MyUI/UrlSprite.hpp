#pragma once
#include "../Common/HSHUtility.h"

class CUrlSprite : public cocos2d::Sprite
{
public:
    static CUrlSprite* create();
    CUrlSprite* setUrl(std::string url);
    CUrlSprite* setSize(cocos2d::Size size);
    CUrlSprite* build(cocos2d::Node* parent, int zOrder = 0);
    
    void setTextureWithUrl(std::string url);
    
protected:
    virtual bool init() override;

private:
    CUrlSprite()
    : m_Size(cocos2d::Size::ZERO)
    , m_Url(""){};
    virtual ~CUrlSprite(){};
    
private:
    cocos2d::Size m_Size;
    std::string m_Url;
};
