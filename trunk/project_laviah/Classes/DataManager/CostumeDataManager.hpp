#pragma once
#include "../Common/HSHUtility.h"
#include "../json/json.h"
#include <vector>
#include <map>

namespace COSTUME_DEFINE {
    static const std::string NAME         = "COSTUME_NAME_%d";
    static const std::string TEXTURE      = "costume_texture_%d.png";
    static const std::string TEXTURE_600  = "costume_texture_600_%d.png";
    static const std::string TEXTURE_PACK = "costume_texture_pack_%d";
}

struct COSTUME{
    int _index;
    int _characterIndex;
    std::string _name;
    std::string _texture;
    std::string _texture_600;
    std::string _texturePack;
    bool _enable;

    COSTUME()
    : _index(0)
    , _characterIndex(-1)
    , _name("")
    , _texture("")
    , _texture_600("")
    , _texturePack("")
    , _enable(false){};
};

typedef std::map<int, const COSTUME*> COSTUME_LIST;
class CCostumeDataManager
{
public:
    static CCostumeDataManager* Instance();
    
    COSTUME_LIST getCostumeList(){ return m_CostumeList; };
    COSTUME_LIST getCostumeListByCharacter(int index);
    const COSTUME* getCostumeByIndex(int index) const;
    const COSTUME* getNewRandomCostume();
    const COSTUME* getCurCostumeByCharacter(int index);
    
private:
    void initWithJson(COSTUME_LIST &list, std::string fileName);
    void addCostumeToList(const Json::Value& json);
    
    const COSTUME* getNewRandomCostumeFromList(COSTUME_LIST &list);
    COSTUME_LIST getNonCollectedCostumeList();
    
    CCostumeDataManager();
    virtual ~CCostumeDataManager();
    
private:
    COSTUME_LIST m_CostumeList;
};
