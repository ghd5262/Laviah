#pragma once
#include "../Common/HSHUtility.h"

namespace cocos2d {
    namespace network {
        class HttpClient;
        class HttpResponse;
        class HttpRequest;
    }
}

class CUrlSprite : public cocos2d::Sprite
{
public:
    static CUrlSprite* create();
    CUrlSprite* setUrl(std::string url, std::string tag);
    CUrlSprite* setSize(cocos2d::Size size);
    CUrlSprite* setSaveToFileEnable(bool enable);
    CUrlSprite* setRetryLimit(int count);
    CUrlSprite* build(cocos2d::Node* parent, int zOrder = 0);
    
protected:
    virtual bool init() override;

private:
    void downloadAsUrl(std::string url);
    void downloadSucceed(cocos2d::network::HttpClient* client,
                         cocos2d::network::HttpResponse* response);
    void downloadFailed();
    bool isDownloaded(std::string tag);
    void setScaleBySize();
    
    CUrlSprite()
    : m_Size(cocos2d::Size::ZERO)
    , m_Url("")
    , m_Tag("")
    , m_RetryLimit(3)
    , m_SaveToFile(false){};
    virtual ~CUrlSprite(){};
    
private:
    cocos2d::Size m_Size;
    std::string m_Url;
    std::string m_Tag;
    int m_RetryLimit;
    bool m_SaveToFile;
};
