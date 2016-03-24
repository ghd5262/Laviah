#pragma once
#include "GameObject.h"
/* ----------------HealthCalculatorInNormal�� ���ؼ�----------------
* PlayerŬ������ virtual�� ��� �Լ��� �����ұ� ������
* ���� �Ȱ��� ĳ���Ͷ� ��Ȳ, ����, Ư�� �� 
* ������ ���� �ٸ��� �����Ű�� �; �ݹ��Լ��� ������
*-----------------------------------------------------------------*/

class CPlayer : public CGameObject {
public:
	static CPlayer* create(std::string textureName, float boundingRadius, float angle, float rotateSpeed, float maxLife);
	virtual void Execute(float delta = 0.f) override;
	void GotSomeHealth(float health);
	void LostSomeHealth(float loseHealth);
	void GiantMode();
	void Rotation(int dir);
	
	//callback
	float HealthCalculatorInNormal(float delta);	// ��ҿ� ����Ǵ� ����� ����Լ�
	float HealthCalculatorInBonusTime(float delta); // ���ʽ� Ÿ�ӿ� ����Ǵ� ����� ����Լ�
	
	//getter & setter
	void setOriginPos(cocos2d::Vec2 pos) { m_OriginPos = pos; }
	cocos2d::Vec2 getOriginPos(){ return m_OriginPos; }

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

	//getter & setter
	CC_SYNTHESIZE(float, m_fAngle, Angle);
	CC_SYNTHESIZE(float, m_fRotateSpeed, RotateSpeed);
	CC_SYNTHESIZE(float, m_fLife, Life);
	CC_SYNTHESIZE(float, m_fMaxLife, MaxLife);
	CC_SYNTHESIZE(float, m_fLevel, Level);
	CC_SYNTHESIZE(float, m_fMagnetLimitRadius, MagnetLimitRadius);

private:
	CPlayer(std::string textureName, float boundingRadius, float rotate, float rotateSpeed, float maxLife);
	virtual ~CPlayer(){}

private:
	cocos2d::Vec2 m_OriginPos;
	std::string m_TextureName;
	Sprite* m_pTexture;
};