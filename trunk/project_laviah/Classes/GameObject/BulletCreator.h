#pragma once
#include "Bullet/Bullet.h"
#include "../Common/HSHUtility.h"

namespace BULLETCREATOR{
	const static float PATTERN_PADDING_LIMIT	= 1.f;
	const static float ROTATION_SPEED			= 90.f;
	const static float BULLET_STANDARD_SPEED	= 381.f;
	const static float BULLET_STANDARD_PADDING  = 60.f;
	const static float COIN_CREATE_LIMIT_TIME   = 5.f;
	static bool COIN_CREATE						= false;
};

struct PLANET;
struct sBULLET_PATTERN;
class CBullet;
class CBulletDataManager;
class CBulletCreator : public cocos2d::Node{
public:
    static CBulletCreator* create();
    
    virtual bool init() override;
    
    void Update(float delta);
    
    void Clear();
    
	void Rotation(float speed);
    
	void setPattern(int index);
    
	void setPattern(const sBULLET_PATTERN* data);

	bool getIsRunning() const { return m_Running; }
    
	void CreateImmediately(int index,
						   float angle,
						   float distance);
    
    void CreateImmediately(const sBULLET_PATTERN* data,
                           float angle,
                           float distance,
                           eITEM_FLAG type = eITEM_FLAG_none);
    
    
    static CBullet* CreateBullet(char symbol,
                                 float angle,
                                 float distance,
								 bool isDelay = true);
    
    static void CreateConstellation(const sBULLET_PATTERN* data);
    
    CC_SYNTHESIZE(const PLANET*, m_PlanetInfo, PlanetInfo);
	CC_SYNTHESIZE(float, m_RotationAngle, RotationAngle);
    CC_SYNTHESIZE(float, m_LineIntervalLimit, LineIntervalLimit);
    CC_SYNTHESIZE(float, m_CreateDistance, CreateDistance);
    CC_SYNTHESIZE(float, m_StandardDelay, StandardDelay);
    CC_SYNTHESIZE(bool, m_IsFlip, IsFlip);
    CC_SYNTHESIZE(int, m_CurrentPatternIndex, CurrentPatternIndex);

private:
	void setData(const sBULLET_PATTERN* data);
    void createOneLine(const sBULLET_PATTERN* data,
                       int currentHeight,
                       float distance,
					   float angle,
					   bool isDalay);
    
    void createOneLine(std::string line,
                       int currentHeight,
                       float distance,
                       float angle,
                       bool isDalay,
                       float widthPadding);
    
    std::string getOneLineOfPattern(const char* pattern, int width, int curHeight);
  
    void setBulletDataByUserData(sBULLET_PARAM& data, char symbol);
    float getCalculatedDelayTime(float speed);

    CBulletCreator();
    virtual ~CBulletCreator();
    
private:
    const sBULLET_PATTERN* m_CurrentPattern;
    CBulletDataManager* m_BulletDataManager;
	float m_CoinTimer;
	float m_Time;
    int m_CurrentHeight;
    bool m_Running;
};
