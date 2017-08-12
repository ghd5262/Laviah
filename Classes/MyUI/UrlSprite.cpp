#include "UrlSprite.hpp"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "../Common/StringUtility.h"
#include <regex>

namespace URLSPRITE_DEFINE {
    const static std::string REMOTE_PATH = "remoteImage/";
}

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace URLSPRITE_DEFINE;

CUrlSprite* CUrlSprite::create()
{
    CUrlSprite *pRet = new(std::nothrow) CUrlSprite();
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

bool CUrlSprite::init() {
    return Sprite::init();
}

CUrlSprite* CUrlSprite::build(cocos2d::Node* parent, int zOrder/* = 0*/)
{
    this->setContentSize(m_Size);
    
    if(this->isDownloaded(m_Tag)){
        auto texture = Director::getInstance()->getTextureCache()->getTextureForKey(m_Tag);
        this->initWithTexture(texture);
    }
    else if(FileUtils::getInstance()->isFileExist(m_Tag)){
        this->initWithFile(m_Tag);
    }
    else if(m_Url != ""){
        this->downloadAsUrl(m_Url);
    }
    
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->setScaleBySize();
    parent->addChild(this, zOrder);

    return this;
}

CUrlSprite* CUrlSprite::setUrl(std::string url, std::string tag)
{
    m_Url = url;
    m_Tag = std::regex_replace(tag, std::regex(".png"), "");
    m_Tag = std::regex_replace(m_Tag, std::regex("[^a-zA-Z0-9_-]"), "");
    m_Tag += ".png";
    return this;
}

CUrlSprite* CUrlSprite::setSize(cocos2d::Size size)
{
    m_Size = size;
    return this;
}

CUrlSprite* CUrlSprite::setSaveToFileEnable(bool enable)
{
    m_SaveToFile = enable;
    return this;
}

CUrlSprite* CUrlSprite::setRetryLimit(int count)
{
    m_RetryLimit = count;
    return this;
}

void CUrlSprite::downloadAsUrl(std::string url)
{
    CCLOG("\n\nDownload request url : %s", url.c_str());
    m_RetryLimit--;
    this->retain();
    
    network::HttpRequest* request = new network::HttpRequest();
    request->setUrl(m_Url.data());
    request->setRequestType(network::HttpRequest::Type::GET);
    request->setResponseCallback([=](network::HttpClient* client,
                                     network::HttpResponse* response) {
        if (!response->isSucceed() || response->getResponseCode() != 200) {
            CCLOG("\n\nDownload failed response code is %ld", response->getResponseCode());
            CCLOG("Download failed response is %s", response->getErrorBuffer());
            this->downloadFailed();
            this->release();
            return;
        }
        this->downloadSucceed(client, response);
        this->release();
    });
    network::HttpClient::getInstance()->send(request);
    request->release();
}

void CUrlSprite::downloadSucceed(network::HttpClient* client,
                                 network::HttpResponse* response)
{
    // request data
    std::vector<char> *buffer = response->getResponseData();
    std::string responseStr   = "";
    responseStr.resize(buffer->size());
    std::copy(buffer->begin(), buffer->end(), responseStr.begin());
    
    // create new image with data
    auto image   = new Image();
    image->autorelease();
    
    // check is the image data downloaded completly.
    auto succeed = image->initWithImageData((unsigned char*)responseStr.c_str(), responseStr.size());
    if(!succeed){
        CCLOG("Image initializing with data failed. Check the data of Image");
//        MessageBox("Remote image download failed", "Error");
        this->downloadFailed();
        return;
    }
    
    // remove cached image before.
    auto textureCache = cocos2d::Director::getInstance()->getTextureCache();
    if(textureCache->getTextureForKey(m_Tag))
        textureCache->removeTextureForKey(m_Tag);
    
    if(m_SaveToFile)
    {
        auto path        = FileUtils::getInstance()->getWritablePath() + REMOTE_PATH + m_Tag;
        auto saveSucceed = image->saveToFile(path);
        
        this->initWithFile(path);
        CCLOG("\n\nRemote image saved %s path : %s", saveSucceed?"succeed" : "failed", path.c_str());
    } else {
        // add image to cache.
        auto texture     = textureCache->addImage(image, m_Tag);
        this->initWithTexture(texture);
    }
    
    this->setScaleBySize();
}

void CUrlSprite::downloadFailed()
{
    if(m_RetryLimit <= 0){
        CCLOG("Download failed try to load default image");
//        MessageBox("Remote image download failed", "Error");
        return;
    }

    CCLOG("Download failed count - %d", m_RetryLimit);
    this->downloadAsUrl(m_Url);
    return ;
}

bool CUrlSprite::isDownloaded(std::string tag)
{
    return (Director::getInstance()->getTextureCache()->getTextureForKey(tag) != nullptr);
}

void CUrlSprite::setScaleBySize()
{
    if (0 != m_Size.width) {
        auto size = this->getContentSize();
        this->setScaleX(m_Size.width  / size.width);
        this->setScaleY(m_Size.height / size.height);
    }
}
