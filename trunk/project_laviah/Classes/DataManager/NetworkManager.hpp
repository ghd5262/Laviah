#pragma once
#include "../Common/HSHUtility.h"
#include "../json/json.h"

namespace NETWORK_DEFINE {
    static const std::string URL            = "http://www.hshgames.com/game/project_s/";
    static const std::string URL_PHP        = URL + "php/";
    static const std::string URL_DOWNLOAD   = URL + "download/";
    static const std::string URL_TIMESTAMP  = "https://worldtimeapi.org/api/timezone/Etc/GMT";
}

namespace SERVER_REQUEST_KEY {
    static const std::string TIMESTAMP_PHP = "timeStamp.php";
}

typedef std::function<void(Json::Value)> NETWORK_CALLBACK;
class CNetworkManager
{
public:
    static CNetworkManager* Instance();
    void Request(const NETWORK_CALLBACK& callback, std::string key, std::string url = "");

private:
    CNetworkManager();
    virtual ~CNetworkManager();
};


#define SERVER_REQUEST(callback, key) CNetworkManager::Instance()->Request(callback, key)
#define SERVER_REQUEST_URL(callback, key, url) CNetworkManager::Instance()->Request(callback, key, url)
