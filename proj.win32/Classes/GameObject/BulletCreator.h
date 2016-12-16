#pragma once
#include "Planet.h"
#include "Bullet/Bullet.h"
#include "../Common/HSHUtility.h"
#include "../DataManager/BulletPatternDataManager.h"
#include "../DataManager/CharacterDataManager.h"


namespace BULLETCREATOR{
	const static float PATTERN_PADDING_LIMIT	= 2.f; 
	const static float CREATE_DISTANCE			= 2700.f;
	const static float ROTATION_SPEED			= 90.f;
	const static float BULLET_STANDARD_SPEED	= 400.f;
	const static float BULLET_STANDARD_PADDING  = 60.f;
	const static float BULLET_STANDARD_DELAY	= (CREATE_DISTANCE - PLANET_DEFINE::BOUNDING_RADIUS) / BULLET_STANDARD_SPEED;
};

class CBulletDataManager;
class CBulletCreator : public cocos2d::Node{
public:
    static CBulletCreator* create();
    
    virtual bool init() override;
    
    void Update(float delta);
    
    void Clear();
    
    void setRotationAngle(float dir, float delta);
    
    void setPattern(std::string patternName);
    
	void setPattern(const sBULLET_PATTERN* data);

	bool getIsRunning() const { return m_Running; }
    
    void CreateImmediately(std::string patternName,
                                  float angle,
                                  float distance);
    
    static CBullet* createBullet(char symbol,
                                 float angle,
                                 float distance,
								 bool isDelay = true);
    CC_SYNTHESIZE(sCHARACTER_PARAM, m_CharacterInfo, CharacterInfo);
private:
	void setData(const sBULLET_PATTERN* data);
    void createOneLine(const sBULLET_PATTERN* data,
                       int currentHeight,
                       float distance,
					   float angle,
					   bool isDalay);
    
    void setBulletDataByUserData(sBULLET_PARAM& data, char symbol);
    
    CBulletCreator();
    virtual ~CBulletCreator();
    
private:
    const sBULLET_PATTERN* m_CurrentPattern;
    CBulletDataManager* m_BulletDataManager;
    float m_RotationAngle;
	float m_Time;
	float m_LineIntervalLimit;
    int m_CurrentHeight;
    bool m_Running;
	bool m_IsFlip;
};