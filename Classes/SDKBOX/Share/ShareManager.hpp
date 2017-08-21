#pragma once
#include <iostream>
#include "cocos2d.h"
#include "PluginShare/PluginShare.h"

class CShareManager : public sdkbox::ShareListener
{
public:
    static CShareManager* Instance();
    void Initialize();
    virtual bool init();
    
    CC_SYNTHESIZE(std::string, m_CapturePath, CapturePath);
    
    static void Share(bool dialog, sdkbox::SocialPlatform platform);
    static void ShareNative();
    static bool SaveNodeToFile(cocos2d::Node* node);
private:
    
    // Callbacks
    virtual void onShareState(const sdkbox::SocialShareResponse& response);
    
    CShareManager();
    virtual ~CShareManager();
};