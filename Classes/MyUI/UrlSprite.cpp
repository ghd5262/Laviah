#include "UrlSprite.hpp"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"

using namespace cocos2d;
using namespace cocos2d::ui;

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
    if(m_Url != "")
        this->setTextureWithUrl(m_Url);
    
    this->setContentSize(m_Size);
    parent->addChild(this, zOrder);
    return this;
}

CUrlSprite* CUrlSprite::setUrl(std::string url)
{
    m_Url = url;
    return this;
}

CUrlSprite* CUrlSprite::setSize(cocos2d::Size size)
{
    m_Size = size;
    return this;
}

void CUrlSprite::setTextureWithUrl(std::string url)
{
    network::HttpRequest* request = new network::HttpRequest();
    request->setUrl(m_Url.data());
    request->setRequestType(network::HttpRequest::Type::GET);
    request->setResponseCallback([=](network::HttpClient* client, network::HttpResponse* response) {
        if (!response->isSucceed()) {
            CCLOG("ERROR, remote sprite load image failed");
            return ;
        }
        
        // request data
        std::vector<char> *buffer = response->getResponseData();
        std::string responseStr = "";
        responseStr.resize(buffer->size());
        std::copy(buffer->begin(), buffer->end(), responseStr.begin());
        
        Image img;
        img.initWithImageData((unsigned char*)responseStr.c_str(), responseStr.size());
        
        if (0)
        {
            // save image file to device.
            std::string path = FileUtils::getInstance()->getWritablePath()+"p.png";
            CCLOG("save image path = %s", path.data());
            bool ret = img.saveToFile(path);
            CCLOG("save file %s", ret ? "success" : "failure");
            
            this->initWithFile(path);
        } else {
            
            // create sprite with texture
            Texture2D *texture = new Texture2D();
            texture->autorelease();
            texture->initWithImage(&img);
            
            this->initWithTexture(texture);
//            this->setTexture(texture);
            if (0 != m_Size.width) {
                auto size = getContentSize();
                setScaleX(m_Size.width  / size.width);
                setScaleY(m_Size.height / size.height);
            }
        }
    });
    network::HttpClient::getInstance()->send(request);
    request->release();
}

