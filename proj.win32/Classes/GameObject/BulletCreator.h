#pragma once
#include "Bullet/Bullet.h"
#include "../Common/HSHUtility.h"
#include "../DataManager/BulletPatternDataManager.h"
#include "../DataManager/CharacterDataManager.h"

class CBulletDataManager;
class CBulletCreator : public cocos2d::Node{
public:
    static CBulletCreator* create();
    
    virtual bool init() override;
    
    void Update(float delta);
    
    void setRotationAngle(float dir, float delta);
    
    void setPattern(std::string patternName, float speed);
    
    void Pause() { m_Running = false; };
    void Resume() { m_Running = true; };
    
    static void createImmediately(std::string patternName,
                                  float angle,
                                  float distance,
                                  float speed);
    
    static CBullet* createBullet(char symbol,
                                 float angle,
                                 float distance,
                                 float speed);
    
    
    CC_SYNTHESIZE(float, m_RotationSpeed, RotationSpeed);
    
private:
    void createOneLine(const sBULLET_PATTERN* data,
                       int currentHeight,
                       float distance,
                       float speed);
    
    void setBulletDataByUserData(sBULLET_PARAM& data, char symbol);
    void clear();
    
    CBulletCreator();
    virtual ~CBulletCreator();
    
private:
    const sBULLET_PATTERN* m_CurrentPattern;
    sCHARACTER_PARAM m_CharacterInfo;
    CBulletDataManager* m_BulletDataManager;
    float m_RotationAngle;
    float m_CreateDistance;
    float m_BulletSpeed;
    int m_CurrentHeight;
    bool m_Running;
};