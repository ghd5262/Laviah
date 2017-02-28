#pragma once
#include "../Common/HSHUtility.h"
#include "../json/json.h"
#include <vector>
#include <map>

namespace MOREGAMES_DEFINE {
    static const std::string TEXTURE_NAME = "more_games_image_%d.png";
}

struct sMOREGAMES_PARAM{
    int _idx;
    std::string _textureName;
    std::string _url;
    
    sMOREGAMES_PARAM()
    : _idx(0)
    , _textureName("")
    , _url(""){}
};

typedef std::map<int, const sMOREGAMES_PARAM*> MOREGAMES_LIST;
class CMoreGamesDataManager
{
public:
    static CMoreGamesDataManager* Instance();
    
    //getter & setter
    const sMOREGAMES_PARAM* getMoreGamesByIndex(int index) const;
    MOREGAMES_LIST getMoreGamesList(){ return m_MoreGamesList; };
    
private:
    void initWithJson(MOREGAMES_LIST &list, std::string fileName);
    void addMoreGamesToList(const Json::Value& data);
    
    CMoreGamesDataManager();
    virtual ~CMoreGamesDataManager();
    
private:
    MOREGAMES_LIST m_MoreGamesList;
};