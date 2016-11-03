#pragma once
#include "cocos2d.h"

USING_NS_CC;

class CTextField;
class CGoogleCloudTestAddKeyDP : public cocos2d::Layer
{
public:
    static CGoogleCloudTestAddKeyDP* create();
    
protected:
    virtual bool init() override;
    bool initVariable();
    
private:
	void Buy(Node* sender);
    
    CGoogleCloudTestAddKeyDP()
    : m_TextField(nullptr){};
    virtual ~CGoogleCloudTestAddKeyDP(){};
    
private:
    CTextField* m_TextField;
};

