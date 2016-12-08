#pragma once
#include "cocos2d.h"

class CPlayer;

class CMultipleScore : public cocos2d::Node
{
	const float MULTIPLE_TIME_LIMIT = 5.f;

public:
	static CMultipleScore* Instance();
	void AddScore(unsigned score);
    void UpdateScore();
    virtual void update(float delta) override;


protected:
	virtual bool init() override;

private:
	void calculateScore();
	void multipleScoreReset();

	CMultipleScore();
	virtual ~CMultipleScore();

private:
    static CMultipleScore* m_Instance;
	cocos2d::Label* m_MultipleNumberLabel;
	CPlayer* m_Player;
	unsigned m_MultipleNumber; //현재 스코어 배수( MULTIPLE_TIME_LIMIT 동안 유지된 후 1로 변경됨 )
	unsigned m_SavedScore;	   //현재까지 저장된 점수( MULTIPLE_TIME_LIMIT 동안 유지된 후 0으로 변경됨 )
	bool m_isAbleToMultiple;   //마지막 점수 획득 시간으로 부터 MULTIPLE_TIME_LIMIT동안 true로 유지된다.
	float m_Time;
};