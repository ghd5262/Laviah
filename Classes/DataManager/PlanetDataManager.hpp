#pragma once
#include "../Common/HSHUtility.h"
#include "../json/json.h"
#include <vector>
#include <map>

namespace PLANET_DEFINE {
    static const std::string NAME           = "PLANET_NAME_%d";
    static const std::string PLANET_TEXTURE = "planet_texture_%d.png";
    static const std::string NORMAL_BULLET  = "normal_bullet_%d.png";
    static const std::string NORMAL_MISSILE = "normal_missile_%d.png";
    static const std::string STAND_BULLET   = "stand_bullet_%d.png";
    static const std::string STICK_BULLET   = "stick_bullet_%d.png";
    static const std::string TEXTURE_PACK   = "planet_texture_pack_%d";
}

struct PLANET{
    int _index;
    int _openLevel;
    std::string _leaderboard;
    std::string _name;
    std::string _planetTexture;
    std::string _normalBulletTexture;
    std::string _normalMissileTexture;
    std::string _stickBulletTexture;
    std::string _standBulletTexture;
    std::string _texturePack;
    bool _enable;

    PLANET()
    : _index(-1)
    , _openLevel(0)
    , _leaderboard("")
    , _name("")
    , _planetTexture("")
    , _normalBulletTexture("")
    , _normalMissileTexture("")
    , _stickBulletTexture("")
    , _standBulletTexture("")
    , _texturePack("")
    , _enable(false){};
};

typedef std::map<int, const PLANET*> PLANET_LIST;
class CPlanetDataManager
{
public:
    static CPlanetDataManager* Instance();
    
    PLANET_LIST getPlanetList(){ return m_PlanetList; };
    const PLANET* getPlanetByIndex(int index) const;
    const PLANET* getCurPlanet() const;

private:
    void initWithJson(PLANET_LIST &list, std::string fileName);
    void addPlanetToList(const Json::Value& json);
    
    CPlanetDataManager();
    virtual ~CPlanetDataManager();
    
private:
    PLANET_LIST m_PlanetList;
};
