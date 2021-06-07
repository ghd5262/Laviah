#include "NetworkManager.hpp"
#include "network/HttpClient.h"

using namespace cocos2d;
using namespace cocos2d::network;

CNetworkManager::CNetworkManager(){}
CNetworkManager::~CNetworkManager(){}

CNetworkManager* CNetworkManager::Instance()
{
    static CNetworkManager instance;
    return &instance;
}

void CNetworkManager::Request(const NETWORK_CALLBACK& callback, std::string key, std::string url/* = ""*/)
{
    auto assertion = [=](std::string content){
        CCLOG("%s", content.c_str());
        CCASSERT(false, content.c_str());
    };

    if(url.empty()) {
        url = NETWORK_DEFINE::URL_PHP + key;
    }
    auto tag = key;
    tag.replace(tag.find(std::string(".php")), std::string(".php").size(), std::string(""));
    
    HttpRequest* request = new (std::nothrow) HttpRequest();
    request->setUrl(url);
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback([=](HttpClient *sender, HttpResponse *response) {
        if (!response) {
            assertion("response data couldn't be null.");
            return;
        }
        
        // You can get original request type from: response->request->reqType
        if (0 != strlen(response->getHttpRequest()->getTag()))
            CCLOG("Server response completed : %s", response->getHttpRequest()->getTag());
        
        
        long statusCode = response->getResponseCode();
        char statusString[64] = {};
        sprintf(statusString, "HTTP Status Code: %ld, tag = %s", statusCode,
                response->getHttpRequest()->getTag());
        CCLOG("response code : %ld", statusCode);
        
        if (!response->isSucceed())
        {
            CCLOG("response failed error buffer : %s", response->getErrorBuffer());
            return;
        }
        
        // request data
        std::vector<char> *buffer = response->getResponseData();
        std::string responseStr = "";
        responseStr.resize(buffer->size());
        std::copy(buffer->begin(), buffer->end(), responseStr.begin());
        
        CCLOG("=======================Server request=========================");
        CCLOG("Server request data : %s", responseStr.c_str());
        CCLOG("==============================================================");
        
        Json::Value  root;
        Json::Reader reader;
        std::string  jsonData   = responseStr;
        
        jsonData                = jsonData.substr(0, jsonData.rfind("}") + 1);
        bool parsingSuccessful  = reader.parse(jsonData, root);
        if (!parsingSuccessful)
        {
            assertion(StringUtils::format("server request parser failed : \n %s", jsonData.c_str()));
            return;
        }
        
        // call listener
        callback(root);
        
        if (response->getHttpRequest()->getReferenceCount() != 2)
        {
            CCLOG("request ref count not 2, is %d", response->getHttpRequest()->getReferenceCount());
        }
    });
    request->setTag(tag);
    HttpClient::getInstance()->send(request);
    request->release();
}
