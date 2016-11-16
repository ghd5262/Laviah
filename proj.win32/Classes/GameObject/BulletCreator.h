#pragma once
#include "Bullet/Bullet.h"
#include "../Common/HSHUtility.h"
#include "../DataManager/BulletPatternDataManager.h"

class CBulletDataManager;
class CBulletCreator{
public:
	static CBulletCreator* Instance();

	void Update(float delta);

	void setRotationAngle(float dir, float delta);

	void setPattern(const sBULLET_PATTERN* data, float distance);
	void createImmediately(const sBULLET_PATTERN* data, float angle, float distance);

	void Pause() { m_Running = false; };
	void Resume() { m_Running = true; };

	static CBullet* createBullet(sBULLET_PARAM data,
		float angle,
		float distance);

	CC_SYNTHESIZE(float, m_RotationSpeed, RotationSpeed);

private:
	void createOneLine(const sBULLET_PATTERN* data, int currentHeight, float distance);
	void clear();

	CBulletCreator();
	~CBulletCreator();

private:
	const sBULLET_PATTERN* m_CurrentPattern;
	CBulletDataManager* m_BulletDataManager;
	float m_RotationAngle;
	float m_CreateDistance;
	int m_CurrentHeight;
	bool m_Running;
};